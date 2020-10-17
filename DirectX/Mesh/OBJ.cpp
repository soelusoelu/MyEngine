#include "OBJ.h"
#include "VertexArray.h"
#include "../DebugLayer/Debug.h"
#include "../Device/AssetsManager.h"
#include "../System/World.h"
#include <sstream>

OBJ::OBJ() = default;

OBJ::~OBJ() = default;

void OBJ::perse(const std::string& fileName, std::vector<MeshVertex>& vertices) {
    //OBJファイルを開いて内容を読み込む
    std::ifstream ifs(fileName);
    if (ifs.fail()) {
        Debug::windowMessage(fileName + ": ファイルが存在しません");
        return;
    }

    //1行ずつ読み込む
    std::string line;
    while (!ifs.eof()) {
        //キーワード読み込み
        std::getline(ifs, line);

        //空白かコメントだったらスキップ
        if (line.empty() || line[0] == '#') {
            continue;
        }

        //1行分の文字列をストリームに変換して解析しやすくする
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ' ');

        //先頭文字列がvなら頂点座標
        if (key == "v") {
            //座標読み込み
            Vector3 pos;
            lineStream >> pos.x;
            lineStream >> pos.y;
            lineStream >> pos.z;
            //座標データに追加
            mPositions.emplace_back(pos);
        } else if (key == "vt") {
            //UV成分を読み込む
            Vector2 uv;
            lineStream >> uv.x;
            lineStream >> uv.y;
            //V方向反転
            uv.y = 1.f - uv.y;
            //テクスチャ座標データに追加
            mUVs.emplace_back(uv);
        } else if (key == "vn") {
            //xyz成分を読み込む
            Vector3 normal;
            lineStream >> normal.x;
            lineStream >> normal.y;
            lineStream >> normal.z;
            //法線ベクトルデータに追加
            mNormals.emplace_back(normal);
        } else if (key == "f") {
            //半角スペース区切りで行の続きを読み込む
            std::string indexString;
            while (std::getline(lineStream, indexString, ' ')) {
                //頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
                std::istringstream indexStream(indexString);
                unsigned short indexPos, indexNormal, indexUV;
                indexStream >> indexPos;
                //1文字分読み飛ばす
                indexStream.seekg(1, std::ios_base::cur);
                //indexStream >> indexUV;
                //1文字分読み飛ばす
                indexStream.seekg(1, std::ios_base::cur);
                indexStream >> indexNormal;

                //頂点データ追加
                MeshVertex vertex{};
                vertex.pos = mPositions[indexPos - 1];
                vertex.normal = mNormals[indexNormal - 1];
                //vertex.uv = mUVs[indexUV - 1];
                vertices.emplace_back(vertex);

                //頂点インデックスに追加
                mIndices.emplace_back(mIndices.size());
            }
        }
    }
}

const std::vector<Vector3>& OBJ::getPositions() const {
    return mPositions;
}

const std::vector<Vector3>& OBJ::getNormals() const {
    return mNormals;
}

const std::vector<Vector2>& OBJ::getUVs() const {
    return mUVs;
}

const std::vector<unsigned short>& OBJ::getIndices() const {
    return mIndices;
}

Vector3 OBJ::getCenter() const {
    auto min = Vector3::one * Math::infinity;
    auto max = Vector3::one * Math::negInfinity;
    for (size_t i = 0; i < mPositions.size(); i++) {
        if (mPositions[i].x < min.x) {
            min.x = mPositions[i].x;
        }
        if (mPositions[i].x > max.x) {
            max.x = mPositions[i].x;
        }
        if (mPositions[i].y < min.y) {
            min.y = mPositions[i].y;
        }
        if (mPositions[i].y > max.y) {
            max.y = mPositions[i].y;
        }
        if (mPositions[i].z < min.z) {
            min.z = mPositions[i].z;
        }
        if (mPositions[i].z > max.z) {
            max.z = mPositions[i].z;
        }
    }
    return (max + min) / 2.f;
}

float OBJ::getRadius() const {
    float min = Math::infinity;
    float max = Math::negInfinity;
    for (size_t i = 0; i < mPositions.size(); i++) {
        if (mPositions[i].x < min) {
            min = mPositions[i].x;
        }
        if (mPositions[i].x > max) {
            max = mPositions[i].x;
        }
        if (mPositions[i].y < min) {
            min = mPositions[i].y;
        }
        if (mPositions[i].y > max) {
            max = mPositions[i].y;
        }
        if (mPositions[i].z < min) {
            min = mPositions[i].z;
        }
        if (mPositions[i].z > max) {
            max = mPositions[i].z;
        }
    }
    return (max - min) / 2.f;
}

bool OBJ::materialLoad(const std::string& materialName) {
    //std::ifstream ifs(materialName, std::ios::in);
    //if (ifs.fail()) {
    //    Debug::windowMessage(materialName + ": ファイルが存在しません");
    //    return false;
    //}

    ////ファイルの先頭に戻る
    //ifs.clear();
    //ifs.seekg(0, std::ios_base::beg);

    //std::string line;
    //char s[256]; //ダミー
    //Vector3 v;
    //int matCount = -1;

    ////本読み込み
    //while (!ifs.eof()) {
    //    //キーワード読み込み
    //    std::getline(ifs, line);

    //    if (line.empty() || line[0] == '#') {
    //        continue;
    //    }

    //    sscanf_s(line.c_str(), "%s", s, sizeof(s));

    //    //マテリアル名
    //    if (strcmp(s, "newmtl") == 0) {
    //        matCount++;
    //        mInitMaterials[matCount]->matName = line.substr(7); //「newmtl 」の文字数分
    //    }
    //    //アンビエント
    //    if (strcmp(s, "Ka") == 0) {
    //        sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
    //        mInitMaterials[matCount]->ambient = v;
    //    }
    //    //ディフューズ
    //    if (strcmp(s, "Kd") == 0) {
    //        sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
    //        mInitMaterials[matCount]->diffuse = v;
    //    }
    //    //スペキュラー
    //    if (strcmp(s, "Ks") == 0) {
    //        sscanf_s(line.c_str(), "%s %f %f %f", s, sizeof(s), &v.x, &v.y, &v.z);
    //        mInitMaterials[matCount]->specular = v;
    //    }
    //    //テクスチャー
    //    if (strcmp(s, "map_Kd") == 0) {
    //        mInitMaterials[matCount]->textureName = line.substr(7); //「map_Kd 」の文字数分

    //        //テクスチャーを作成
    //        //ディレクトリが変わってない前提
    //        mInitMaterials[matCount]->texture = World::instance().assetsManager().createTextureFromModel(mInitMaterials[matCount]->textureName);
    //    }
    //}

    return true;
}
