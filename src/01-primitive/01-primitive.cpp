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

glm::mat4x4 getMVP()
{
    auto aspect = 1.0f; // Assuming square viewport; replace with actual width/height
    auto scale = 1.0f / 100.0f; // Scale down large models

    glm::mat4 proj = glm::ortho(-aspect, aspect, -1.f, 1.f, -1.f, 1.f);
    glm::mat4 model =
        glm::rotate(glm::mat4(1.f), getCurrentTimeInRadians(), glm::vec3(0, 0, 1));
    glm::mat4 view = glm::scale(glm::mat4(1.0f), glm::vec3(scale));

    return proj * view * model;
}

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

        model = loadModelFromObjData(getObj(), getMaterial());

        vertexPositions = createBufferFrom(device.get(), model.positions);
        vertexColors = createBufferFrom(device.get(), model.colors);
        uniformBuffer.create(device.get());
    }

    void draw() override
    {
        uniformBuffer->modelMatrix = getMVP();
        uniformBuffer.update();

        commandEncoder->setRenderPipelineState(pso.get());
        commandEncoder->setVertexBuffer(vertexPositions.get(), 0, 0);
        commandEncoder->setVertexBuffer(vertexColors.get(), 0, 1);
        commandEncoder->setVertexBuffer(uniformBuffer.getBuffer(), 0, 2);

        commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                       NS::UInteger(0),
                                       NS::UInteger(model.positions.size()));
    }

    NS::SharedPtr<MTL::RenderPipelineState> pso;
    NS::SharedPtr<MTL::Buffer> vertexPositions;
    NS::SharedPtr<MTL::Buffer> vertexColors;
    Graphics::BufferOwner<Uniforms> uniformBuffer;
    Graphics::Model model;
};

int main()
{
    Apple::startApp<Renderer>();

    return 0;
}
