#include <SharedCodeLib/Lib.h>
#include "Shader.h"

static float getCurrentTimeInRadians()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    auto seconds = std::chrono::duration<float>(now - startTime).count();

    return seconds; // 1 rad/sec rotation speed
}

struct Uniforms
{
    glm::mat4x4 modelMatrix;
};

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
        uniformBuffer = createNewBuffer(device.get(), sizeof(Uniforms));
    }

    void draw() override
    {
        auto angle = getCurrentTimeInRadians(); // You define how angle is updated

        auto* uniforms = static_cast<Uniforms*>(uniformBuffer->contents());
        uniforms->modelMatrix =
            glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 0, 1));
        uniformBuffer->didModifyRange(NS::Range::Make(0, sizeof(Uniforms)));

        commandEncoder->setRenderPipelineState(pso.get());
        commandEncoder->setVertexBuffer(vertexPositions.get(), 0, 0);
        commandEncoder->setVertexBuffer(vertexColors.get(), 0, 1);
        commandEncoder->setVertexBuffer(uniformBuffer.get(), 0, 2); // New

        commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                       NS::UInteger(0),
                                       NS::UInteger(3));
    }

    NS::SharedPtr<MTL::RenderPipelineState> pso;
    NS::SharedPtr<MTL::Buffer> vertexPositions;
    NS::SharedPtr<MTL::Buffer> vertexColors;
    NS::SharedPtr<MTL::Buffer> uniformBuffer;
};

int main()
{
    Apple::startApp<Renderer>();

    return 0;
}
