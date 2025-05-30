#pragma once

#include <MetalKit/MetalKit.hpp>

class Renderer
{
public:
    Renderer(MTL::Device* pDevice);
    ~Renderer();
    void draw(const MTK::View* view) const;

private:
    MTL::Device* device;
    MTL::CommandQueue* commandQueue;
};
