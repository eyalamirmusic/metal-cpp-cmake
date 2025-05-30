#pragma once

#include <MetalKit/MetalKit.hpp>

namespace Apple
{

class Renderer
{
public:
    virtual ~Renderer();

    void setDevice(MTL::Device* deviceToUse);

    virtual void draw(const MTK::View* view) const;

protected:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
};
} // namespace Apple