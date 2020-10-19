#include "FBX.h"
#include "Material.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../System/World.h"
#include "../Utility/FileUtil.h"

FBX::FBX() :
    mNumMeshes(0) {
}

FBX::~FBX() = default;

void FBX::perse(const std::string& fileName, std::vector<MeshParam>& meshes) {
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

    //マテリアル単位でメッシュを分割する
    geometryConverter.SplitMeshesPerMaterial(scene, true);

    //FbxMeshの数を取得
    mNumMeshes = scene->GetSrcObjectCount<FbxMesh>();

    //メッシュの数に合わせて拡張する
    meshes.resize(mNumMeshes);
    mPositions.resize(mNumMeshes);
    mNormals.resize(mNumMeshes);
    mUVs.resize(mNumMeshes);
    mIndices.resize(mNumMeshes);

    //FbxMeshの数だけメッシュを作成する
    for (size_t i = 0; i < mNumMeshes; i++) {
        auto mesh = scene->GetSrcObject<FbxMesh>(i);
        createMesh(meshes[i], mesh, i);
    }

    //マネージャー解放
    manager->Destroy();
}

const std::vector<Vector3>& FBX::getPositions(unsigned meshIndex) const {
    return mPositions[meshIndex];
}

const std::vector<unsigned short>& FBX::getIndices(unsigned meshIndex) const {
    return mIndices[meshIndex];
}

const Material& FBX::getMaterial(unsigned index) const {
    return mMaterials[index];
}

unsigned FBX::getMeshCount() const {
    return mNumMeshes;
}

void FBX::createMesh(MeshParam& meshParam, FbxMesh* mesh, unsigned meshIndex) {
    loadPosition(mesh, meshIndex);
    loadNormal(mesh, meshIndex);
    loadUV(mesh, meshIndex);
    loadFace(meshParam, mesh, meshIndex);
    loadMaterial(mesh, meshIndex);
}

void FBX::loadPosition(FbxMesh* mesh, unsigned meshIndex) {
    auto& positions = mPositions[meshIndex];

    //頂点数
    auto polygonVertexCount = mesh->GetPolygonVertexCount();
    positions.resize(polygonVertexCount);
    auto indices = mesh->GetPolygonVertices();

    //頂点座標配列
    FbxVector4* src = mesh->GetControlPoints();
    for (size_t i = 0; i < positions.size(); i++) {
        int index = indices[i];

        //xはマイナスのはずだけど背面カリングがうまくいかないから
        positions[i].x = static_cast<float>(src[index][0]);
        positions[i].y = static_cast<float>(src[index][1]);
        positions[i].z = static_cast<float>(src[index][2]);
    }
}

void FBX::loadNormal(FbxMesh* mesh, unsigned meshIndex) {
    auto& normals = mNormals[meshIndex];

    //法線配列を取得する
    FbxArray<FbxVector4> normalArray;
    mesh->GetPolygonVertexNormals(normalArray);
    normals.resize(normalArray.Size());

    for (size_t i = 0; i < normals.size(); i++) {
        //xはマイナスのはずだけど背面カリングがうまくいかないから
        normals[i].x = static_cast<float>(normalArray[i][0]);
        normals[i].y = static_cast<float>(normalArray[i][1]);
        normals[i].z = static_cast<float>(normalArray[i][2]);
    }

#pragma region 公式サンプル
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
#pragma endregion
}

void FBX::loadUV(FbxMesh* mesh, unsigned meshIndex) {
    auto& uvs = mUVs[meshIndex];

    //UVSetの名前リストを取得
    FbxStringList uvSetNameList;
    mesh->GetUVSetNames(uvSetNameList);

    //UVSetの名前からUVSetを取得する
    FbxArray<FbxVector2> uvArray;
    mesh->GetPolygonVertexUVs(uvSetNameList.GetStringAt(0), uvArray);
    uvs.resize(uvArray.Size());

    for (size_t i = 0; i < uvs.size(); i++) {
        //xは1から引かないはずだけど背面カリングがうまくいかないから
        uvs[i].x = 1.f - static_cast<float>(uvArray[i][0]);
        uvs[i].y = 1.f - static_cast<float>(uvArray[i][1]);
    }

#pragma region 公式サンプル
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
#pragma endregion
}

void FBX::loadFace(MeshParam& meshParam, FbxMesh* mesh, unsigned meshIndex) {
    auto& indices = mIndices[meshIndex];

    //indicesはポリゴン数の3倍
    auto polygonCount = mesh->GetPolygonCount();
    indices.resize(polygonCount * 3);

    for (int i = 0; i < polygonCount; i++) {
        indices[i * 3] = i * 3;
        indices[i * 3 + 1] = i * 3 + 1;
        indices[i * 3 + 2] = i * 3 + 2;
    }

    auto& positions = mPositions[meshIndex];
    auto& normals = mNormals[meshIndex];
    auto& uvs = mUVs[meshIndex];
    meshParam.positions.resize(positions.size());
    meshParam.normals.resize(normals.size());
    meshParam.uvs.resize(uvs.size());
    std::copy(positions.begin(), positions.end(), meshParam.positions.begin());
    std::copy(normals.begin(), normals.end(), meshParam.normals.begin());
    std::copy(uvs.begin(), uvs.end(), meshParam.uvs.begin());
    //meshParam.resize(indices.size());
    //for (size_t i = 0; i < indices.size(); i++) {
    //    meshParam.positions[i] = positions[i];
    //    meshParam.normals[i] = normals[i];

    //    //uvは使われてる場合のみ
    //    if (uvs.size() > 0) {
    //        meshParam.uvs[i] = uvs[i];
    //    }
    //}
}

void FBX::loadMaterial(FbxMesh* mesh, unsigned meshIndex) {
    //すべてのメッシュにマテリアルを持たせるため
    mMaterials.emplace_back();

    //マテリアルがなければ終了
    if (mesh->GetElementMaterialCount() == 0) {
        return;
    }

    //Mesh側のマテリアル情報を取得
    FbxLayerElementMaterial* material = mesh->GetElementMaterial(0);

    //インデックスバッファからインデックスを取得
    int index = material->GetIndexArray().GetAt(0);
    //マテリアル取得
    FbxSurfaceMaterial* surfaceMaterial = mesh->GetNode()->GetSrcObject<FbxSurfaceMaterial>(index);

    //マテリアル名の保存
    if (surfaceMaterial) {
        //マテリアル名を取得
        mMaterials[meshIndex].materialName = surfaceMaterial->GetName();

        //テクスチャ以外の情報を取得
        loadMaterialAttribute(surfaceMaterial, meshIndex);

        //テクスチャを取得
        loadMaterialTexture(surfaceMaterial, meshIndex);
    }
}

void FBX::loadMaterialAttribute(FbxSurfaceMaterial* material, unsigned meshIndex) {
    auto& mat = mMaterials[meshIndex];
    //まずはランバート分読み込む
    FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

    //アンビエント
    auto prop = lambert->FindProperty(FbxSurfaceMaterial::sAmbient);
    if (prop.IsValid()) {
        auto ambient = lambert->Ambient.Get();
        mat.ambient.x = static_cast<float>(ambient[0]);
        mat.ambient.y = static_cast<float>(ambient[1]);
        mat.ambient.z = static_cast<float>(ambient[2]);
    }

    //ディヒューズ
    prop = lambert->FindProperty(FbxSurfaceMaterial::sDiffuse);
    if (prop.IsValid()) {
        auto diffuse = lambert->Diffuse.Get();
        mat.diffuse.x = static_cast<float>(diffuse[0]);
        mat.diffuse.y = static_cast<float>(diffuse[1]);
        mat.diffuse.z = static_cast<float>(diffuse[2]);
    }

    //エミッシブ
    prop = lambert->FindProperty(FbxSurfaceMaterial::sEmissive);
    if (prop.IsValid()) {
        auto emissive = lambert->Emissive.Get();
        mat.emissive.x = static_cast<float>(emissive[0]);
        mat.emissive.y = static_cast<float>(emissive[1]);
        mat.emissive.z = static_cast<float>(emissive[2]);
    }

    //バンプ
    prop = lambert->FindProperty(FbxSurfaceMaterial::sBump);
    if (prop.IsValid()) {
        auto bump = lambert->Bump.Get();
        mat.bump.x = static_cast<float>(bump[0]);
        mat.bump.y = static_cast<float>(bump[1]);
        mat.bump.z = static_cast<float>(bump[2]);
    }

    //透過度
    prop = lambert->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
    if (prop.IsValid()) {
        mat.transparency = static_cast<float>(lambert->TransparencyFactor.Get());
    }

    //Phongだったら追加読み込み
    if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
        FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(material);

        //スペキュラ
        prop = lambert->FindProperty(FbxSurfaceMaterial::sSpecular);
        if (prop.IsValid()) {
            auto specular = phong->Specular.Get();
            mat.specular.x = static_cast<float>(specular[0]);
            mat.specular.y = static_cast<float>(specular[1]);
            mat.specular.z = static_cast<float>(specular[2]);
        }

        //光沢
        prop = lambert->FindProperty(FbxSurfaceMaterial::sShininess);
        if (prop.IsValid()) {
            mat.shininess = static_cast<float>(phong->Shininess.Get());
        }
    }
}

void FBX::loadMaterialTexture(FbxSurfaceMaterial* material, unsigned meshIndex) {
    //Diffuseプロパティを取得
    auto prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

    //テクスチャを取得する
    FbxFileTexture* texture = nullptr;
    int numTexture = prop.GetSrcObjectCount<FbxFileTexture>();
    if (numTexture > 0) {
        texture = prop.GetSrcObject<FbxFileTexture>(0);
    } else {
        //失敗したらマルチテクスチャの可能性を考えて、FbxLayerdTextureを指定する
        //FbxLayerdTextureからテクスチャを取得する
        int numLayer = prop.GetSrcObjectCount<FbxLayeredTexture>();
        if (numLayer > 0) {
            texture = prop.GetSrcObject<FbxFileTexture>(0);
        }
    }

    //テクスチャを使用していなかったら終了
    if (!texture) {
        return;
    }

    //適用したいマテリアル
    auto& mat = mMaterials[meshIndex];

    //ファイルパスを相対パスで取得
    auto filePath = texture->GetRelativeFileName();
    //ファイルパスからファイル名を取得する
    mat.textureName = FileUtil::getFileNameFromDirectry(filePath);

    //テクスチャ作成
    mat.texture = World::instance().assetsManager().createTextureFromModel(mat.textureName);
}
