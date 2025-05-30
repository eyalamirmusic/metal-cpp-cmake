#include "Renderer.h"
#include "AutoReleasePool.h"

namespace Apple
{
Renderer::~Renderer()
{
    commandQueue->release();
    device->release();
}

void Renderer::setDevice(MTL::Device* deviceToUse)
{
    device = deviceToUse;
    commandQueue = device->newCommandQueue();
}

void Renderer::draw(const MTK::View* view) const
{
    auto pool = AutoReleasePool();

    auto cmd = commandQueue->commandBuffer();
    auto rpd = view->currentRenderPassDescriptor();
    auto enc = cmd->renderCommandEncoder(rpd);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();
}
}