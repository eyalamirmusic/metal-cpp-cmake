#include <SharedCodeLib/Lib.h>
#include "Shader.h"

using Vertices = std::vector<glm::vec3>;

inline size_t getSize(const Vertices& vertices)
{
    return sizeof(glm::vec3) * vertices.size();
}

NS::SharedPtr<MTL::Buffer> createNewBuffer(MTL::Device* device, size_t size)
{
    return NS::TransferPtr(device->newBuffer(size, MTL::ResourceStorageModeManaged));
}

NS::SharedPtr<MTL::Buffer> createBufferFrom(MTL::Device* device,
                                            const Vertices& vertices)
{
    auto size = getSize(vertices);
    auto buf = createNewBuffer(device, size);
    memcpy(buf->contents(), vertices.data(), size);
    buf->didModifyRange(NS::Range::Make(0, buf->length()));

    return buf;
}

glm::mat4 makeCenteredZRotation(float angle, const glm::vec3& center)
{
    glm::mat4 translateToOrigin = glm::translate(glm::mat4(1.0f), -center);
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1));
    glm::mat4 translateBack = glm::translate(glm::mat4(1.0f), center);
    return translateBack * rotate * translateToOrigin;
}

glm::vec3 computeTriangleCentroid(const Vertices& positions)
{
    auto sum = glm::vec3(0.f);

    for (auto& pos: positions)
        sum += pos;

    return sum / static_cast<float>(positions.size());
}

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
        auto positions =
            Vertices {{-0.8f, 0.8f, 0.0f}, {0.0f, -0.8f, 0.0f}, {+0.8f, 0.8f, 0.0f}};

        auto colors =
            Vertices {{1.0, 0.3f, 0.2f}, {0.8f, 1.0, 0.0f}, {0.8f, 0.0f, 1.0}};

        center = computeTriangleCentroid(positions);

        vertexPositions = createBufferFrom(device.get(), positions);
        vertexColors = createBufferFrom(device.get(), positions);
        uniformBuffer = createNewBuffer(device.get(), sizeof(Uniforms));
    }

    void draw() override
    {
        auto angle = getCurrentTimeInRadians(); // You define how angle is updated

        auto* uniforms = static_cast<Uniforms*>(uniformBuffer->contents());
        uniforms->modelMatrix = makeCenteredZRotation(angle, center);
        uniformBuffer->didModifyRange(NS::Range::Make(0, sizeof(Uniforms)));

        commandEncoder->setRenderPipelineState(pso.get());
        commandEncoder->setVertexBuffer(vertexPositions.get(), 0, 0);
        commandEncoder->setVertexBuffer(vertexColors.get(), 0, 1);
        commandEncoder->setVertexBuffer(uniformBuffer.get(), 0, 2); // New

        commandEncoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                                       NS::UInteger(0),
                                       NS::UInteger(3));
    }

    glm::vec3 center {};

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
