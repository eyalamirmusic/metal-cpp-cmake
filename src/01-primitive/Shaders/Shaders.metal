#include <metal_stdlib>
using namespace metal;

struct v2f
{
    float4 position [[position]];
    half3  color;
};

struct Uniforms
{
    float4x4 modelMatrix;
};

vertex v2f vertexMain(uint                         vertexID [[vertex_id]],
                      const device packed_float3*  positions [[buffer(0)]],
                      const device packed_float3*  colors    [[buffer(1)]],
                      constant Uniforms&           uniforms  [[buffer(2)]])
{
    v2f out;
    float3 p = float3(positions[vertexID]);
    out.position = uniforms.modelMatrix * float4(p, 1.0);
    out.color    = half3(colors[vertexID]);
    return out;
}

fragment half4 fragmentMain(v2f in [[stage_in]])
{
    return half4(in.color, 1.0);
}
