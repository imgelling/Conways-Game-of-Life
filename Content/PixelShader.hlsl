////Texture2D<float4> DiffuseMap : register(t0);
////SamplerState Sampler : register(s0);

//struct VS_OUTPUT
//{
//    float4 Pos : SV_POSITION;
//    float4 Color : COLOR0;
//    float2 TexCoord : TEXCOORD;
//};


//float4 main(VS_OUTPUT input) : SV_Target
//{

//    float4 diffuse = input.Color; // DiffuseMap.Sample(Sampler, input.TexCoord);
//    //diffuse = diffuse * input.Color;
//    return diffuse; // input.Color;
//}
// simple pixel shader
//float4 main() : SV_TARGET
//{
//    return float4(0.0f, 1.0f, 0.0f, 1.0f); // Red, Green, Blue, Alpha
//}

//struct PSInput
//{
//    float4 position : SV_POSITION;
//    float4 color : COLOR;
//};

//float4 main(PSInput input) : SV_TARGET
//{
//    return input.color;
//}
struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{
    return g_texture.Sample(g_sampler, input.uv) * input.color;
}