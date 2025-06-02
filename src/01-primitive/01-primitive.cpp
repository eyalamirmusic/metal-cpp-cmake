#include <SharedCodeLib/App.h>
#include "Shader.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // Metal uses 0..1 depth
#define GLM_FORCE_LEFT_HANDED // Optional: Metal is right-handed by default
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES // Good for matching Metal struct layout
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Generate a rotation matrix (angle in radians)
glm::mat4 makeZRotation(float angle)
{
    return glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

static float getCurrentTimeInRadians()
{
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();
    float seconds = std::chrono::duration<float>(now - startTime).count();
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
        const size_t NumVertices = 3;

        glm::vec3 positions[NumVertices] = {
            {-0.8f, 0.8f, 0.0f}, {0.0f, -0.8f, 0.0f}, {+0.8f, 0.8f, 0.0f}};

        glm::vec3 colors[NumVertices] = {
            {1.0, 0.3f, 0.2f}, {0.8f, 1.0, 0.0f}, {0.8f, 0.0f, 1.0}};

        const size_t positionsDataSize = NumVertices * sizeof(glm::vec3);
        const size_t colorDataSize = NumVertices * sizeof(glm::vec3);

        vertexPositions = NS::TransferPtr(
            device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged));
        vertexColors = NS::TransferPtr(
            device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged));

        memcpy(vertexPositions->contents(), positions, positionsDataSize);
        memcpy(vertexColors->contents(), colors, colorDataSize);

        vertexPositions->didModifyRange(
            NS::Range::Make(0, vertexPositions->length()));
        vertexColors->didModifyRange(NS::Range::Make(0, vertexColors->length()));

        uniformBuffer = NS::TransferPtr(
            device->newBuffer(sizeof(Uniforms), MTL::ResourceStorageModeManaged));
    }

    void draw() override
    {
        float angle = getCurrentTimeInRadians(); // You define how angle is updated
        float c = std::cos(angle);
        float s = std::sin(angle);

        // Row-major 4x4 rotation matrix (around Z)
        glm::mat4x4 rotation = {
            {c, s, 0.f, 0.f}, {-s, c, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};

        auto* uniforms = static_cast<Uniforms*>(uniformBuffer->contents());
        uniforms->modelMatrix = rotation;
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
