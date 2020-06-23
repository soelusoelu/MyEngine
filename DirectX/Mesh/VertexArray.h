#pragma once

#include <memory>
#include <vector>

class IndexBuffer;
class VertexBuffer;

class VertexArray {
    using IndexBufferPtr = std::unique_ptr<IndexBuffer>;
    using IndexBufferPtrArray = std::vector<IndexBufferPtr>;

public:
    VertexArray();
    ~VertexArray();

    void setNumVerts(unsigned num);
    unsigned getNumVerts() const;
    void setNumNormal(unsigned num);
    unsigned getNumNormal() const;
    void setNumTex(unsigned num);
    unsigned getNumTex() const;
    void setNumFace(unsigned num);
    unsigned getNumFace() const;

    void createVertexBuffer(unsigned vertexSize, const void* data);
    void createIndexBuffer(unsigned index, unsigned numFace, const void* data);

    void setVertexBuffer(unsigned numStream = 1, unsigned start = 0, unsigned offset = 0);
    void setIndexBuffer(unsigned index, unsigned offset = 0);

private:
    unsigned mNumVerts; //���_��
    unsigned mNumNormal; //�@����
    unsigned mNumTex; //�e�N�X�`�����W��
    unsigned mNumFace; //�|���S����

    std::unique_ptr<VertexBuffer> mVertexBuffer;
    IndexBufferPtrArray mIndexBuffer;
};
