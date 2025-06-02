#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3 color;
};

struct Uniforms {
    float4x4 modelMatrix;
};

vertex v2f vertexMain(uint vertexID [[vertex_id]],
                      const device float3* positions [[buffer(0)]],
                      const device float3* colors [[buffer(1)]],
                      constant Uniforms& uniforms [[buffer(2)]])
{
    v2f out;
    float4 pos = float4(positions[vertexID], 1.0);
    out.position = uniforms.modelMatrix * pos;
    out.color = half3(colors[vertexID]); // Explicit conversion to half3
    return out;
}

half4 fragment fragmentMain( v2f in [[stage_in]] )
{
    return half4( in.color, 1.0 );
}