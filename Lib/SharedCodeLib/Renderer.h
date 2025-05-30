#pragma once

#include <MetalKit/MetalKit.hpp>

namespace Apple
{

struct RenderContext
{
    RenderContext(MTL::Device* deviceToUse,
                  MTL::CommandQueue* queueToUse,
                  MTK::View* viewToUse)
        : device(deviceToUse)
        , commandQueue(queueToUse)
        , view(viewToUse)
    {
    }

    void endFrame() const
    {
        commandEncoder->endEncoding();
        commandBuf->presentDrawable(view->currentDrawable());
        commandBuf->commit();
    }

    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
    MTL::CommandBuffer* commandBuf {commandQueue->commandBuffer()};
    MTK::View* view;
    MTL::RenderPassDescriptor* renderPassDescriptor {
        view->currentRenderPassDescriptor()};
    MTL::RenderCommandEncoder* commandEncoder {
        commandBuf->renderCommandEncoder(renderPassDescriptor)};
};

class Renderer
{
public:
    virtual ~Renderer();

    void setDevice(MTL::Device* deviceToUse);

    virtual void deviceChanged() {}
    void drawIn(MTK::View* view);

protected:
    virtual void draw(RenderContext& context);

    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
};
} // namespace Apple