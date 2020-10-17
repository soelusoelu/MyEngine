#include "OBJ.h"
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

    //解析開始
    std::string line;
    while (!ifs.eof()) {
        //1行ずつ読み込む
        std::getline(ifs, line);

        //読み込みに不要な行ならスキップ
        if (isSkip(line)) {
            continue;
        }

        //1行分の文字列をストリームに変換して解析しやすくする
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ' ');

        if (key == "v") { //先頭文字列がvなら頂点座標
            loadPosition(lineStream);
        } else if (key == "vt") { //先頭文字列がvtならuv座標
            loadUV(lineStream);
        } else if (key == "vn") { //先頭文字列がvnなら法線
            loadNormal(lineStream);
        } else if (key == "f") { //先頭文字列がfならポリゴン
            loadFace(lineStream, vertices);
        } else if (key == "mtlib") {
            loadMaterial(lineStream);
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

const Material& OBJ::getMaterial(unsigned index) const {
    return mMaterials[index];
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

void OBJ::loadPosition(std::istringstream& iss) {
    //座標読み込み
    Vector3 pos;
    iss >> pos.x;
    iss >> pos.y;
    iss >> pos.z;
    //座標データに追加
    mPositions.emplace_back(pos);
}

void OBJ::loadNormal(std::istringstream& iss) {
    //xyz成分を読み込む
    Vector3 normal;
    iss >> normal.x;
    iss >> normal.y;
    iss >> normal.z;
    //法線ベクトルデータに追加
    mNormals.emplace_back(normal);
}

void OBJ::loadUV(std::istringstream& iss) {
    //UV成分を読み込む
    Vector2 uv;
    iss >> uv.x;
    iss >> uv.y;
    //V方向反転
    uv.y = 1.f - uv.y;
    //テクスチャ座標データに追加
    mUVs.emplace_back(uv);
}

void OBJ::loadFace(std::istringstream& iss, std::vector<MeshVertex>& vertices) {
    //半角スペース区切りで行の続きを読み込む
    std::string indexString;
    while (std::getline(iss, indexString, ' ')) {
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

void OBJ::loadMaterial(std::istringstream& iss) {
    //マテリアルのファイル名読み込み
    std::string fileName;
    iss >> fileName;

    //マテリアルファイルを開く
    std::ifstream ifs(fileName);
    if (ifs.fail()) {
        Debug::windowMessage(fileName + ": ファイルが存在しません");
        return;
    }

    //解析開始
    std::string line;
    while (!ifs.eof()) {
        //1行ずつ読み込む
        std::getline(ifs, line);

        //読み込みに不要な行ならスキップ
        if (isSkip(line)) {
            continue;
        }

        //1行分の文字列をストリームに変換
        std::istringstream lineStream(line);

        //半角スペース区切りで行の先頭文字列を取得
        std::string key;
        std::getline(lineStream, key, ' ');

        //先頭のタブ文字は無視する
        if (key[0] == '\t') {
            //先頭の文字を削除
            key.erase(key.begin());
        }

        //マテリアルの各属性を読み込む
        if (key == "newmtl") {
            loadMaterialName(iss);
        } else if (key == "Ka") {
            loadAmbient(iss);
        } else if (key == "Kd") {
            loadDiffuse(iss);
        } else if (key == "Ks") {
            loadSpecular(iss);
        } else if (key == "map_Kd") {
            loadTexture(iss);
        }
    }
}

void OBJ::loadMaterialName(std::istringstream& iss) {
    mMaterials.resize(mMaterials.size() + 1);
    iss >> mMaterials.back().materialName;
}

void OBJ::loadAmbient(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.ambient.x;
    iss >> mat.ambient.y;
    iss >> mat.ambient.z;
}

void OBJ::loadDiffuse(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.diffuse.x;
    iss >> mat.diffuse.y;
    iss >> mat.diffuse.z;
}

void OBJ::loadSpecular(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    iss >> mat.specular.x;
    iss >> mat.specular.y;
    iss >> mat.specular.z;
}

void OBJ::loadTexture(std::istringstream& iss) {
    auto& mat = mMaterials.back();
    //テクスチャ名読み込み
    iss >> mat.textureName;
    //テクスチャ読み込み
    mat.texture = World::instance().assetsManager().createTextureFromModel(mat.textureName);
}

bool OBJ::isSkip(const std::string& line) {
    //空白かコメントだったらスキップ
    return (line.empty() || line[0] == '#');
}
