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
    deviceChanged();
}

void Renderer::drawIn(MTK::View* view)
{
    auto pool = AutoReleasePool();

    auto context = RenderContext(device, commandQueue, view);
    draw(context);
}

void Renderer::draw(RenderContext& context)
{
    context.endFrame();
}
} // namespace Apple