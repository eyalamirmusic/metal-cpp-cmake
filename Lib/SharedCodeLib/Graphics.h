#pragma once

#include "GLM.h"
#include <Metal/Metal.hpp>

namespace Graphics
{
using Vertices = std::vector<glm::vec3>;

inline size_t getSize(const Vertices& vertices)
{
    return sizeof(glm::vec3) * vertices.size();
}

inline NS::SharedPtr<MTL::Buffer> createNewBuffer(MTL::Device* device, size_t size)
{
    return NS::TransferPtr(device->newBuffer(size, MTL::ResourceStorageModeManaged));
}

inline NS::SharedPtr<MTL::Buffer> createBufferFrom(MTL::Device* device,
                                            const Vertices& vertices)
{
    auto size = getSize(vertices);
    auto buf = createNewBuffer(device, size);
    memcpy(buf->contents(), vertices.data(), size);
    buf->didModifyRange(NS::Range::Make(0, buf->length()));

    return buf;
}
} // namespace Graphics
