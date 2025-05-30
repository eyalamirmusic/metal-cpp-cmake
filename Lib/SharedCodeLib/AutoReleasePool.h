#pragma once

#include <MetalKit/MetalKit.hpp>

namespace Apple
{
struct AutoReleasePool
{
    ~AutoReleasePool() { releasePool->release(); }

    NS::AutoreleasePool* releasePool = NS::AutoreleasePool::alloc()->init();
};
}
