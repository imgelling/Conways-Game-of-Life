////Texture2D<float4> DiffuseMap : register(t0);

//struct VS_OUTPUT
//{
//    float4 Pos : SV_POSITION;
//    float4 Color : COLOR0;
//    float2 TexCoord : TEXCOORD;
//};

//VS_OUTPUT main(float4 inPos : POSITION, float4 inColor : COLOR, float2 inTexCoord : TEXCOORD)
//{
//    VS_OUTPUT output = (VS_OUTPUT) 0;
//    output.Pos = inPos;
//    output.Color = inColor;
//    output.TexCoord = inTexCoord;
//    return output;
//}
// simple vertex shader
//float4 main(float3 pos : POSITION) : SV_POSITION
//{
//    return float4(pos, 1.0f);
//}
//struct PSInput
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};

//PSInput main(float4 position : POSITION, float4 color : COLOR)
//{
//    PSInput result;

//    result.position = position;
//    result.color = color;

//    return result;
//}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput main(float4 position : POSITION, float4 color: COLOR, float2 uv : TEXCOORD)
{
    PSInput result;

    result.position = position;
    result.color = color;
    result.uv = uv;

    return result;
}