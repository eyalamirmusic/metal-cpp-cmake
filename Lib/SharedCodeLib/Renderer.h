#pragma once

#include <MetalKit/MetalKit.hpp>

namespace Apple
{

struct Renderer
{
    virtual ~Renderer() = default;

    void setDevice(NS::SharedPtr<MTL::Device> deviceToUse);

    virtual void deviceChanged() {}
    void drawIn(MTK::View* currentView);
    virtual void draw();

    void endFrame() const;

    NS::SharedPtr<MTL::Device> device;
    NS::SharedPtr<MTL::CommandQueue> commandQueue;
    MTK::View* view;
    MTL::RenderPassDescriptor* passDescriptor;
    MTL::CommandBuffer* commandBuf;
    MTL::RenderCommandEncoder* commandEncoder;
};
} // namespace Apple