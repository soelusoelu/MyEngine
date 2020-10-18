#include "FBX.h"
#include "Material.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../System/World.h"
#include "../Utility/FileUtil.h"

FBX::FBX() = default;

FBX::~FBX() = default;

void FBX::perse(const std::string& fileName, std::vector<MeshVertex>& vertices) {
    //マネージャーを生成
    auto manager = FbxManager::Create();

    //IOSettingを生成
    FbxIOSettings* ioSettings = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ioSettings);

    //Importerを生成
    FbxImporter* importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(fileName.c_str(), -1, manager->GetIOSettings())) {
        Debug::windowMessage(fileName + ": ファイルは存在しません");
        return;
    }

    //SceneオブジェクトにFBXファイル内の情報を流し込む
    FbxScene* scene = FbxScene::Create(manager, "scene");
    importer->Import(scene);
    importer->Destroy(); //シーンを流し込んだらImporterは解放していい

    //四角ポリゴンを三角ポリゴンに変換
    FbxGeometryConverter geometryConverter(manager);
    geometryConverter.Triangulate(scene, true);

    //Scene解析
    FbxNode* root = scene->GetRootNode();
    if (root) {
        perse(vertices, root, 0);
    }

    //マネージャー解放
    manager->Destroy();
}

const std::vector<Vector3>& FBX::getPositions() const {
    return mPositions;
}

const std::vector<Vector3>& FBX::getNormals() const {
    return mNormals;
}

const std::vector<Vector2>& FBX::getUVs() const {
    return mUVs;
}

const std::vector<unsigned short>& FBX::getIndices() const {
    return mIndices;
}

const Material& FBX::getMaterial(unsigned index) const {
    return mMaterials[index];
}

bool FBX::isUseMaterial() const {
    return (mMaterials.size() > 0);
}

void FBX::perse(std::vector<MeshVertex>& vertices, FbxNode* node, int indent) {
    int attrCount = node->GetNodeAttributeCount();

    for (size_t i = 0; i < attrCount; i++) {
        FbxNodeAttribute* attr = node->GetNodeAttributeByIndex(i);
        FbxNodeAttribute::EType type = attr->GetAttributeType();
        if (i + 1 == attrCount) {
            FbxMesh* mesh = nullptr;
            switch (type) {
            case fbxsdk::FbxNodeAttribute::eMesh:
                mesh = node->GetMesh();
                loadPosition(mesh);
                loadNormal(mesh);
                loadUV(mesh);
                loadFace(vertices, mesh);
                loadMaterial(mesh);
                break;
            default:
                break;
            }
        }
    }

    int childCount = node->GetChildCount();
    for (int i = 0; i < childCount; ++i) {
        perse(vertices, node->GetChild(i), indent + 1);
    }
}

void FBX::loadPosition(FbxMesh* mesh) {
    //頂点数
    auto polygonVertexCount = mesh->GetPolygonVertexCount();
    mPositions.resize(polygonVertexCount);
    auto indices = mesh->GetPolygonVertices();

    //頂点座標配列
    FbxVector4* src = mesh->GetControlPoints();
    for (size_t i = 0; i < mPositions.size(); i++) {
        int index = indices[i];

        mPositions[i].x = static_cast<float>(src[index][0]);
        mPositions[i].y = static_cast<float>(src[index][1]);
        mPositions[i].z = static_cast<float>(src[index][2]);
    }
}

void FBX::loadNormal(FbxMesh* mesh) {
    FbxArray<FbxVector4> normals;
    mesh->GetPolygonVertexNormals(normals);
    mNormals.resize(normals.Size());

    for (size_t i = 0; i < normals.Size(); i++) {
        mNormals[i].x = static_cast<float>(normals[i][0]);
        mNormals[i].y = static_cast<float>(normals[i][1]);
        mNormals[i].z = static_cast<float>(normals[i][2]);
    }

    //FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    //if (!normalElement) {
    //    return;
    //}

    //if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //    for (size_t i = 0; i < mesh->GetControlPointsCount(); i++) {
    //        int normalIndex = 0;
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //            normalIndex = i;
    //        }
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //            normalIndex = normalElement->GetIndexArray().GetAt(i);
    //        }

    //        FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //        mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //        mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //        mNormals[normalIndex].z = static_cast<float>(normal[2]);
    //    }
    //} else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //    int indexByPolygonVertex = 0;
    //    for (size_t i = 0; i < mesh->GetPolygonCount(); i++) {
    //        int polygonSize = mesh->GetPolygonSize(i);
    //        for (size_t j = 0; j < polygonSize; j++) {
    //            int normalIndex = 0;
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //                normalIndex = indexByPolygonVertex;
    //            }
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //                normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
    //            }

    //            FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //            mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //            mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //            mNormals[normalIndex].z = static_cast<float>(normal[2]);

    //            indexByPolygonVertex++;
    //        }
    //    }
    //}
}

void FBX::loadUV(FbxMesh* mesh) {
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);

    FbxArray<FbxVector2> uvs;
    mesh->GetPolygonVertexUVs(uvSetNameList.GetStringAt(0), uvs);
    mUVs.resize(uvs.Size());

    for (size_t i = 0; i < uvs.Size(); i++) {
        mUVs[i].x = 1.f - static_cast<float>(uvs[i][0]);
        mUVs[i].y = 1.f - static_cast<float>(uvs[i][1]);
    }

    //すべてのUVセットを反復処理する
    //for (int uvSetIndex = 0; uvSetIndex < uvSetNameList.GetCount(); uvSetIndex++) {
    //    //uvSetIndex-番目のUVセットを取得
    //    const char* uvSetName = uvSetNameList.GetStringAt(uvSetIndex);
    //    const FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);
    //    if (!uvElement) {
    //        continue;
    //    }

    //    //マッピングモードeByPolygonVertexおよびeByControlPointのみをサポート
    //    if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
    //        return;
    //    }

    //    //インデックス配列。uvデータを参照するインデックスを保持します
    //    const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
    //    const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

    //    //ポリゴンごとにデータを反復処理する
    //    const int polyCount = mesh->GetPolygonCount();

    //    if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                //頂点配列の現在の頂点のインデックスを取得します
    //                int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

    //                //UVインデックスは参照モードに依存します
    //                int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;

    //                FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);
    //            }
    //        }
    //    } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //        int polyIndexCounter = 0;
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                if (polyIndexCounter < indexCount) {
    //                    //UVインデックスは参照モードに依存します
    //                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

    //                    FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                    mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                    mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);

    //                    polyIndexCounter++;
    //                }
    //            }
    //        }
    //    }
    //}
}

void FBX::loadFace(std::vector<MeshVertex>& vertices, FbxMesh* mesh) {
    auto polygonCount = mesh->GetPolygonCount();
    mIndices.resize(polygonCount * 3);

    for (int i = 0; i < polygonCount; i++) {
        mIndices[i * 3] = i * 3;
        mIndices[i * 3 + 1] = i * 3 + 1;
        mIndices[i * 3 + 2] = i * 3 + 2;
    }

    vertices.resize(mIndices.size());
    for (size_t i = 0; i < mIndices.size(); i++) {
        MeshVertex vertex;
        vertices[i].pos = mPositions[i];
        vertices[i].normal = mNormals[i];

        if (mUVs.size() > 0) {
            vertices[i].uv = mUVs[i];
        }
    }
}

void FBX::loadMaterial(FbxMesh* mesh) {
    FbxNode* node = mesh->GetNode();
    if (!node) {
        return;
    }

    //マテリアルの数
    int materialNum = node->GetMaterialCount();
    if (materialNum == 0) {
        return;
    }

    //マテリアル情報を取得
    for (size_t i = 0; i < materialNum; i++) {
        mMaterials.resize(mMaterials.size() + 1);

        FbxSurfaceMaterial* material = node->GetMaterial(i);
        if (!material) {
            continue;
        }

        //解析
        //LambertかPhongか
        if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
            FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

            auto& mat = mMaterials[i];

            //アンビエント
            auto ambient = lambert->Ambient.Get();
            mat.ambient.x = static_cast<float>(ambient[0]);
            mat.ambient.y = static_cast<float>(ambient[1]);
            mat.ambient.z = static_cast<float>(ambient[2]);

            //ディヒューズ
            auto diffuse = lambert->Diffuse.Get();
            mat.diffuse.x = static_cast<float>(diffuse[0]);
            mat.diffuse.y = static_cast<float>(diffuse[1]);
            mat.diffuse.z = static_cast<float>(diffuse[2]);

            //エミッシブ
            auto emissive = lambert->Emissive.Get();
            mat.emissive.x = static_cast<float>(emissive[0]);
            mat.emissive.y = static_cast<float>(emissive[1]);
            mat.emissive.z = static_cast<float>(emissive[2]);

            //バンプ
            auto bump = lambert->Bump.Get();
            mat.bump.x = static_cast<float>(bump[0]);
            mat.bump.y = static_cast<float>(bump[1]);
            mat.bump.z = static_cast<float>(bump[2]);

            //透過度
            mat.transparency = static_cast<float>(lambert->TransparencyFactor.Get());
        } else if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
            FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(material);

            auto& mat = mMaterials[i];

            //アンビエント
            auto ambient = phong->Ambient.Get();
            mat.ambient.x = static_cast<float>(ambient[0]);
            mat.ambient.y = static_cast<float>(ambient[1]);
            mat.ambient.z = static_cast<float>(ambient[2]);

            //ディヒューズ
            auto diffuse = phong->Diffuse.Get();
            mat.diffuse.x = static_cast<float>(diffuse[0]);
            mat.diffuse.y = static_cast<float>(diffuse[1]);
            mat.diffuse.z = static_cast<float>(diffuse[2]);

            //エミッシブ
            auto emissive = phong->Emissive.Get();
            mat.emissive.x = static_cast<float>(emissive[0]);
            mat.emissive.y = static_cast<float>(emissive[1]);
            mat.emissive.z = static_cast<float>(emissive[2]);

            //バンプ
            auto bump = phong->Bump.Get();
            mat.bump.x = static_cast<float>(bump[0]);
            mat.bump.y = static_cast<float>(bump[1]);
            mat.bump.z = static_cast<float>(bump[2]);

            //透過度
            mat.transparency = static_cast<float>(phong->TransparencyFactor.Get());

            //スペキュラ
            auto specular = phong->Specular.Get();
            mat.specular.x = static_cast<float>(specular[0]);
            mat.specular.y = static_cast<float>(specular[1]);
            mat.specular.z = static_cast<float>(specular[2]);

            //光沢
            mat.shininess = static_cast<float>(phong->Shininess.Get());
        }

        //ディフューズプロパティを検索
        FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

        //プロパティが持っているレイヤードテクスチャの枚数をチェック
        int layerNum = property.GetSrcObjectCount<FbxLayeredTexture>();

        //レイヤードテクスチャが無ければ通常テクスチャ
        if (layerNum == 0) {
            //通常テクスチャの枚数をチェック
            int numGeneralTexture = property.GetSrcObjectCount<FbxTexture>();

            //各テクスチャについてテクスチャ情報をゲット
            for (int j = 0; j < numGeneralTexture; ++j) {
                //j番目のテクスチャオブジェクト取得
                FbxTexture* texture = FbxCast<FbxTexture>(property.GetSrcObject<FbxTexture>(j));

                //テクスチャ名
                mMaterials[i].textureName = texture->GetName();

                //テクスチャを作成
                //ディレクトリが変わってない前提
                mMaterials[i].texture = World::instance().assetsManager().createTextureFromModel(mMaterials[i].textureName);

                break; //とりあえず今は1枚だけサポート
            }
        }
    }
}
