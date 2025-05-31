#include "Renderer.h"
#include "AutoReleasePool.h"

namespace Apple
{

void Renderer::setDevice(NS::SharedPtr<MTL::Device> deviceToUse)
{
    device = deviceToUse;
    commandQueue = NS::TransferPtr(device->newCommandQueue());

    deviceChanged();
}

void Renderer::drawIn(MTK::View* currentView)
{
    auto pool = AutoReleasePool();
    commandBuf = {commandQueue->commandBuffer()};
    view = currentView;
    renderPassDescriptor = view->currentRenderPassDescriptor();
    commandEncoder = commandBuf->renderCommandEncoder(renderPassDescriptor);

    draw();
    endFrame();
}

void Renderer::draw()
{
}

void Renderer::endFrame() const
{
    commandEncoder->endEncoding();
    commandBuf->presentDrawable(view->currentDrawable());
    commandBuf->commit();
}
} // namespace Apple