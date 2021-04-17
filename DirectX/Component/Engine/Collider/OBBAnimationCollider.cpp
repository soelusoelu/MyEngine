#include "OBBAnimationCollider.h"
#include "../Mesh/AnimationCPU.h"
#include "../Mesh/MeshComponent.h"
#include "../../../Engine/DebugManager/DebugLayer/Inspector/ImGuiWrapper.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../Transform/Transform3D.h"
#include <cassert>

OBBAnimationCollider::OBBAnimationCollider(GameObject& gameObject)
    : Collider(gameObject)
    , mMesh(nullptr)
    , mAnimationCPU(nullptr)
    , mIsRenderCollision(true)
{
}

OBBAnimationCollider::~OBBAnimationCollider() = default;

void OBBAnimationCollider::start() {
    Collider::start();

    mMesh = getComponent<MeshComponent>();
    mAnimationCPU = getComponent<AnimationCPU>();
    if (!mAnimationCPU) {
        mAnimationCPU = addComponent<AnimationCPU>("AnimationCPU");
    }

    //基礎となるOBBを作成する
    createOBB();
    //最新のOBBの点を計算する
    updatePoints();

    if (mPhysics) {
        //mPhysics->add(shared_from_this());
    }
}

void OBBAnimationCollider::lateUpdate() {
    Collider::lateUpdate();

    //OBBを更新する
    //computeOBB();

    ////OBBの点を更新する
    //updatePoints();

    //当たり判定を可視化する
    if (mIsRenderCollision) {
        renderCollision();
    }
}

void OBBAnimationCollider::finalize() {
    Collider::finalize();

    if (mPhysics) {
        //mPhysics->remove(shared_from_this());
    }
}

void OBBAnimationCollider::onEnable(bool value) {
    Collider::onEnable(value);

    setRenderCollision(value);
}

void OBBAnimationCollider::drawInspector() {
    Collider::drawInspector();

    ImGui::Checkbox("IsRenderCollision", &mIsRenderCollision);
}

void OBBAnimationCollider::concatenate(unsigned a, unsigned b) {
    auto size = mOBBs.size();
    assert(a < size);
    assert(b < size);

    auto& obbA = mOBBs[a];
    obbA.concatenateTargets.emplace_back(b);
    obbA.isActive = true;
    mOBBs[b].isActive = false;
}

const OBB& OBBAnimationCollider::getOBB(unsigned index) const {
    assert(index < mOBBs.size());
    return mOBBs[index].obb;
}

void OBBAnimationCollider::setRenderCollision(bool value) {
    mIsRenderCollision = value;
}

void OBBAnimationCollider::computeOBB() {
    //すべてのメッシュからOBBを作成する
    auto size = mOBBs.size();
    for (size_t i = 0; i < size; ++i) {
        const auto& target = mOBBs[i];
        if (!target.isActive) {
            continue;
        }
        resizeOBB(i);

        const auto& targets = target.concatenateTargets;
        for (const auto& t : targets) {
            updateOBB(i, t);
        }
    }
}

void OBBAnimationCollider::updateOBB(unsigned target, unsigned index) {
    //スキニング結果から更新する
    //Vector3 min, max;
    //computeMinMax(min, max, mAnimationCPU->getCurrentMotionVertexPositions(index));

    //auto& obb = mOBBs[target].obb;
    //obb.updateMinMax(min);
    //obb.updateMinMax(max);
}

void OBBAnimationCollider::resizeOBB(unsigned meshIndex) {
    auto collectMat = collectMatrix(meshIndex);
    auto eigenvector = getEigenvector(collectMat);

    const auto& eigenMat = eigenvector.m;
    auto vec1 = Vector3(
        eigenMat[0][0],
        eigenMat[1][0],
        eigenMat[2][0]
    );

    auto vec2 = Vector3(
        eigenMat[0][1],
        eigenMat[1][1],
        eigenMat[2][1]
    );

    auto vec3 = Vector3(
        eigenMat[0][2],
        eigenMat[1][2],
        eigenMat[2][2]
    );

    //全頂点に対して内積を取り、最小値・最大値を計算する
    float min1 = FLT_MAX;
    float min2 = FLT_MAX;
    float min3 = FLT_MAX;
    float max1 = FLT_MIN;
    float max2 = FLT_MIN;
    float max3 = FLT_MIN;

    const auto& mesh = mMesh->getMesh();
    const auto& meshVertices = mesh->getMeshVertices(meshIndex);
    for (const auto& vert : meshVertices) {
        const auto& pos = vert.pos;
        float dot1 = Vector3::dot(vec1, pos);
        if (dot1 > max1) {
            max1 = dot1;
        }
        if (dot1 < min1) {
            min1 = dot1;
        }

        float dot2 = Vector3::dot(vec2, pos);
        if (dot2 > max2) {
            max2 = dot2;
        }
        if (dot2 < min2) {
            min2 = dot2;
        }

        float dot3 = Vector3::dot(vec3, pos);
        if (dot3 > max3) {
            max3 = dot3;
        }
        if (dot3 < min3) {
            min3 = dot3;
        }
    }

    mOBBs[meshIndex].obb.edges[0] = vec1 * (max1 - min1);
    mOBBs[meshIndex].obb.edges[1] = vec2 * (max2 - min2);
    mOBBs[meshIndex].obb.edges[2] = vec3 * (max3 - min3);

    Vector3 center1 = (vec1 * (max1 + min1)) / 2.f;
    Vector3 center2 = (vec2 * (max2 + min2)) / 2.f;
    Vector3 center3 = (vec3 * (max3 + min3)) / 2.f;

    mOBBs[meshIndex].obb.center = center1 + center2 + center3;
}

void OBBAnimationCollider::createOBB() {
    //メッシュの数分拡張する
    unsigned meshCount = mMesh->getMesh()->getMeshCount();
    mOBBs.resize(meshCount);
    //すべてのメッシュからOBBを作成する
    for (unsigned i = 0; i < meshCount; ++i) {
        resizeOBB(i);
    }
}

Matrix3 OBBAnimationCollider::collectMatrix(unsigned meshIndex) {
    //各成分の平均を求める
    auto avg = Vector3::zero;

    const auto& mesh = mMesh->getMesh();
    const auto& meshVertices = mesh->getMeshVertices(meshIndex);
    for (const auto& vert : meshVertices) {
        avg += vert.pos;
    }

    auto meshVerticesSize = meshVertices.size();
    avg.x /= meshVerticesSize;
    avg.y /= meshVerticesSize;
    avg.z /= meshVerticesSize;

    float m00 = 0.f;
    float m11 = 0.f;
    float m22 = 0.f;
    float m01and10 = 0.f;
    float m02and20 = 0.f;
    float m12and21 = 0.f;
    for (const auto& vert : meshVertices) {
        const auto& pos = vert.pos;
        float x = pos.x - avg.x;
        float y = pos.y - avg.y;
        float z = pos.z - avg.z;

        m00 += x * x;
        m11 += y * y;
        m22 += z * z;
        m01and10 += x * y;
        m02and20 += x * z;
        m12and21 += y * z;
    }

    m00 /= meshVerticesSize;
    m11 /= meshVerticesSize;
    m22 /= meshVerticesSize;
    m01and10 /= meshVerticesSize;
    m02and20 /= meshVerticesSize;
    m12and21 /= meshVerticesSize;

    float m[3][3] = {
        { m00, m01and10, m02and20 },
        { m01and10, m11, m12and21 },
        { m02and20, m12and21, m22 }
    };

    return Matrix3(m);
}

Matrix3 OBBAnimationCollider::getEigenvector(Matrix3& mat) {
    Matrix3 eigenvector{};
    constexpr int limit = 100;
    int count = 0;
    unsigned row = 0;
    unsigned column = 0;
    while (true) {
        ++count;
        if (count >= limit) {
            assert(false);
        }


        float max = 0.f;
        getMaxValueForMatrix(mat, max, row, column);
        if (max <= 0.0001f) {
            break;
        }

        auto& m = mat.m;
        float rowRow = m[row][row];
        float rowColumn = m[row][column];
        float columnColumn = m[column][column];

        float alpha = (rowRow - columnColumn) / 2.f;
        float beta = -rowColumn;
        float gamma = Math::abs(alpha) / Math::sqrt(alpha * alpha + beta * beta);

        float sin = Math::sqrt((1.f - gamma) / 2.f);
        float cos = Math::sqrt((1.f + gamma) / 2.f);

        if (alpha * beta < 0) {
            sin = -sin;
        }

        for (int i = 0; i < 3; ++i) {
            float temp = cos * m[row][i] - sin * m[column][i];
            m[column][i] = sin * m[row][i] + cos * m[column][i];
            m[row][i] = temp;
        }

        for (int i = 0; i < 3; ++i) {
            m[i][row] = m[row][i];
            m[i][column] = m[column][i];
        }

        m[row][row] = cos * cos * rowRow + sin * sin * columnColumn - 2.f * sin * cos * rowColumn;
        m[row][column] = sin * cos * (rowRow - columnColumn) + (cos * cos - sin * sin) * rowColumn;
        m[column][row] = sin * cos * (rowRow - columnColumn) + (cos * cos - sin * sin) * rowColumn;
        m[column][column] = sin * sin * rowRow + cos * cos * columnColumn + 2.f * sin * cos * rowColumn;

        auto& eigenMat = eigenvector.m;
        for (int i = 0; i < 3; ++i) {
            float temp = cos * eigenMat[i][row] - sin * eigenMat[i][column];
            eigenMat[i][column] = sin * eigenMat[i][row] + cos * eigenMat[i][column];
            eigenMat[i][row] = temp;
        }
    }

    return eigenvector;
}

void OBBAnimationCollider::getMaxValueForMatrix(const Matrix3& mat, float& outMax, unsigned& outRow, unsigned& outColumn) {
    float max = FLT_MIN;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            //対角線分は評価しない
            if (i == j) {
                continue;
            }

            float absMax = Math::abs(mat.m[i][j]);
            if (absMax > max) {
                max = absMax;
                outRow = i;
                outColumn = j;
            }
        }
    }

    if (outRow > outColumn) {
        std::swap(outRow, outColumn);
    }

    outMax = max;
}

void OBBAnimationCollider::updatePoints() {

}

void OBBAnimationCollider::renderCollision() {
#ifdef _DEBUG
    //デバッグ時のみ当たり判定を表示
    for (const auto& target : mOBBs) {
        if (!target.isActive) {
            continue;
        }

        const auto& obb = target.obb;
        const auto& edges = obb.edges;
        Vector3 ftr = obb.center + (edges[0] / 2.f) + (edges[1] / 2.f) + (edges[2] / 2.f);
        Vector3 fbr = ftr - edges[2];
        Vector3 ftl = ftr - edges[0];
        Vector3 fbl = ftl - edges[2];

        Vector3 btr = ftr - edges[1];
        Vector3 bbr = btr - edges[2];
        Vector3 btl = btr - edges[0];
        Vector3 bbl = btl - edges[2];

        Debug::renderLine(ftr, fbr, ColorPalette::lightGreen);
        Debug::renderLine(ftr, ftl, ColorPalette::lightGreen);
        Debug::renderLine(ftl, fbl, ColorPalette::lightGreen);
        Debug::renderLine(fbl, fbr, ColorPalette::lightGreen);

        Debug::renderLine(btr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(btr, btl, ColorPalette::lightGreen);
        Debug::renderLine(btl, bbl, ColorPalette::lightGreen);
        Debug::renderLine(bbl, bbr, ColorPalette::lightGreen);

        Debug::renderLine(ftr, btr, ColorPalette::lightGreen);
        Debug::renderLine(ftl, btl, ColorPalette::lightGreen);
        Debug::renderLine(fbr, bbr, ColorPalette::lightGreen);
        Debug::renderLine(fbl, bbl, ColorPalette::lightGreen);
    }
#endif // _DEBUG
}
