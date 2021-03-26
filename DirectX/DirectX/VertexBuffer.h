#pragma once

#include "Buffer.h"
#include "BufferDesc.h"
#include "SubResourceDesc.h"

class VertexBuffer : public Buffer {
public:
    VertexBuffer(const BufferDesc& desc, const SubResourceDesc& data);
    ~VertexBuffer();
    //頂点バッファをGPUに設定する
    void setVertexBuffer(unsigned start = 0, unsigned numStream = 1, unsigned offset = 0);
    //頂点バッファを更新する
    void updateVertexBuffer(const void* newVertices);
};
