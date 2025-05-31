
#include <SharedCodeLib/App.h>
#include <simd/simd.h>
#include <iostream>
#include "Shader.h"

struct Renderer : Apple::Renderer
{
    void buildShaders()
    {
        auto lib = Apple::createLibrary(device.get(), getShader());

        auto vertex = Apple::createFunction(lib.get(), "vertexMain");
        auto fragment = Apple::createFunction(lib.get(), "fragmentMain");

        auto desc = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
        desc->setVertexFunction(vertex.get());
        desc->setFragmentFunction(fragment.get());
        desc->colorAttachments()->object(0)->setPixelFormat(
            MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

        pso = Apple::createRenderState(device.get(), desc.get());
    }

    void buildBuffers()
    {
        const size_t NumVertices = 3;

        simd::float3 positions[NumVertices] = {
            {-0.8f, 0.8f, 0.0f}, {0.0f, -0.8f, 0.0f}, {+0.8f, 0.8f, 0.0f}};

        simd::float3 colors[NumVertices] = {
            {1.0, 0.3f, 0.2f}, {0.8f, 1.0, 0.0f}, {0.8f, 0.0f, 1.0}};

        const size_t positionsDataSize = NumVertices * sizeof(simd::float3);
        const size_t colorDataSize = NumVertices * sizeof(simd::float3);

        vertexPositionsBuffer = NS::TransferPtr(
            device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged));
        vertexColorsBuffer = NS::TransferPtr(
            device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged));

        memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
        memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);

        vertexPositionsBuffer->didModifyRange(
            NS::Range::Make(0, vertexPositionsBuffer->length()));
        vertexColorsBuffer->didModifyRange(
            NS::Range::Make(0, vertexColorsBuffer->length()));
    }

    void draw() override
    {
        commandEncoder->setRenderPipelineState(pso.get());
        commandEncoder->setVertexBuffer(vertexPositionsBuffer.get(), 0, 0);
        commandEncoder->setVertexBuffer(vertexColorsBuffer.get(), 0, 1);
        commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                       NS::UInteger(0),
                                       NS::UInteger(3));
    }

    void deviceChanged() override
    {
        buildShaders();
        buildBuffers();
    }

    NS::SharedPtr<MTL::RenderPipelineState> pso;
    NS::SharedPtr<MTL::Buffer> vertexPositionsBuffer;
    NS::SharedPtr<MTL::Buffer> vertexColorsBuffer;
};

int main()
{
    Apple::startApp<Renderer>();

    return 0;
}
