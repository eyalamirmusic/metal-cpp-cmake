
#include <SharedCodeLib/App.h>
#include <simd/simd.h>
#include <iostream>

const char* getShader()
{
    return R"(
        #include <metal_stdlib>
        using namespace metal;

        struct v2f
        {
            float4 position [[position]];
            half3 color;
        };

        v2f vertex vertexMain( uint vertexId [[vertex_id]],
                               device const float3* positions [[buffer(0)]],
                               device const float3* colors [[buffer(1)]] )
        {
            v2f o;
            o.position = float4( positions[ vertexId ], 1.0 );
            o.color = half3 ( colors[ vertexId ] );
            return o;
        }

        half4 fragment fragmentMain( v2f in [[stage_in]] )
        {
            return half4( in.color, 1.0 );
        }
    )";
}

struct Renderer : Apple::Renderer
{
    ~Renderer() override
    {
        vertexPositionsBuffer->release();
        vertexColorsBuffer->release();
        pso->release();
    }

    void buildShaders()
    {
        using NS::StringEncoding::UTF8StringEncoding;

        auto shaderSrc = getShader();

        NS::Error* pError = nullptr;
        auto* pLibrary = device->newLibrary(
            NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &pError);

        if (!pLibrary)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        auto* pVertexFn = pLibrary->newFunction(
            NS::String::string("vertexMain", UTF8StringEncoding));
        auto* pFragFn = pLibrary->newFunction(
            NS::String::string("fragmentMain", UTF8StringEncoding));

        auto* pDesc = MTL::RenderPipelineDescriptor::alloc()->init();
        pDesc->setVertexFunction(pVertexFn);
        pDesc->setFragmentFunction(pFragFn);
        pDesc->colorAttachments()->object(0)->setPixelFormat(
            MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

        pso = device->newRenderPipelineState(pDesc, &pError);

        if (!pso)
        {
            std::cout << pError->localizedDescription()->utf8String() << std::endl;
            assert(false);
        }

        pVertexFn->release();
        pFragFn->release();
        pDesc->release();
        pLibrary->release();
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

        auto* pVertexPositionsBuffer =
            device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
        auto* pVertexColorsBuffer =
            device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

        vertexPositionsBuffer = pVertexPositionsBuffer;
        vertexColorsBuffer = pVertexColorsBuffer;

        memcpy(vertexPositionsBuffer->contents(), positions, positionsDataSize);
        memcpy(vertexColorsBuffer->contents(), colors, colorDataSize);

        vertexPositionsBuffer->didModifyRange(
            NS::Range::Make(0, vertexPositionsBuffer->length()));
        vertexColorsBuffer->didModifyRange(
            NS::Range::Make(0, vertexColorsBuffer->length()));
    }

    void draw(Apple::RenderContext& context) override
    {
        auto enc = context.commandEncoder;

        enc->setRenderPipelineState(pso);
        enc->setVertexBuffer(vertexPositionsBuffer, 0, 0);
        enc->setVertexBuffer(vertexColorsBuffer, 0, 1);
        enc->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                            NS::UInteger(0),
                            NS::UInteger(3));

        context.endFrame();
    }

    void deviceChanged() override
    {
        buildShaders();
        buildBuffers();
    }

    MTL::RenderPipelineState* pso {};
    MTL::Buffer* vertexPositionsBuffer {};
    MTL::Buffer* vertexColorsBuffer {};
};

int main()
{
    Apple::startApp<Renderer>();

    return 0;
}
