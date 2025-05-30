#include "Renderer.h"

Renderer::Renderer(MTL::Device* pDevice)
    : device(pDevice->retain())
{
    commandQueue = device->newCommandQueue();
}

Renderer::~Renderer()
{
    commandQueue->release();
    device->release();
}

void Renderer::draw(const MTK::View* view) const
{
    auto pool = NS::AutoreleasePool::alloc()->init();

    auto cmd = commandQueue->commandBuffer();
    auto rpd = view->currentRenderPassDescriptor();
    auto enc = cmd->renderCommandEncoder(rpd);

    enc->endEncoding();
    cmd->presentDrawable(view->currentDrawable());
    cmd->commit();

    pool->release();
}
