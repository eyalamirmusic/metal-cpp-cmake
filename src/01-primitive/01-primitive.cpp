#include <SharedCodeLib/Lib.h>
#include "Shader.h"

static float getCurrentTimeInRadians()
{
    using Clock = std::chrono::high_resolution_clock;

    static auto startTime = Clock::now();
    auto now = Clock::now();
    auto seconds = std::chrono::duration<float>(now - startTime).count();

    return seconds;
}

struct Uniforms
{
    glm::mat4x4 modelMatrix;
};

glm::mat4x4 getRotation()
{
    auto angle = getCurrentTimeInRadians();
    return glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 0, 1));
}

struct Renderer : Apple::Renderer
{
    void deviceChanged() override
    {
        buildShaders();
        buildBuffers();
    }

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
        using namespace Graphics;

        auto positions =
            Vertices {{-0.6f, 0.6f, 0.0f}, {0.0f, -0.6f, 0.0f}, {+0.6f, 0.6f, 0.0f}};

        auto colors =
            Vertices {{1.0, 0.3f, 0.2f}, {0.8f, 1.0, 0.0f}, {0.8f, 0.0f, 1.0}};

        vertexPositions = createBufferFrom(device.get(), positions);
        vertexColors = createBufferFrom(device.get(), colors);
        uniformBuffer.create(device.get());
    }

    void draw() override
    {
        uniformBuffer->modelMatrix = getRotation();
        uniformBuffer.update();

        commandEncoder->setRenderPipelineState(pso.get());
        commandEncoder->setVertexBuffer(vertexPositions.get(), 0, 0);
        commandEncoder->setVertexBuffer(vertexColors.get(), 0, 1);
        commandEncoder->setVertexBuffer(uniformBuffer.getBuffer(), 0, 2);

        commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                       NS::UInteger(0),
                                       NS::UInteger(3));
    }

    NS::SharedPtr<MTL::RenderPipelineState> pso;
    NS::SharedPtr<MTL::Buffer> vertexPositions;
    NS::SharedPtr<MTL::Buffer> vertexColors;
    Graphics::BufferOwner<Uniforms> uniformBuffer;
};

int main()
{
    Apple::startApp<Renderer>();

    return 0;
}
