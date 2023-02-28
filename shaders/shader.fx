Texture2D txDiffuse : register(t0);     // Буфер текстури
SamplerState samLinear : register(s0);  // Буфер зразка, потрібен для згладжування 

// Буфер інформації про матрицю
cbuffer ConstantBufferMatrixes : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
}

// Буфер з інформаціємо про світло
cbuffer ConstantBufferLight : register(b1)
{
    float4 vLightDir[2]; 
    float4 vLightColor[2];
    float4 vOutputColor;
}

struct VertShaderInput
{
    float4 Pos : POSITION;
    float2 Text : TEXCOORD0;
    float3 Norm : NORMAL;
};

struct PixelShaderInput
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Norm : TEXCOORD1;
};

PixelShaderInput VertexShaderFunc( VertShaderInput input )
{
    PixelShaderInput to_ret = (PixelShaderInput)0;
    to_ret.Pos = mul(input.Pos, World);
    to_ret.Pos = mul(to_ret.Pos, View);
    to_ret.Pos = mul(to_ret.Pos, Projection);
    to_ret.Norm = mul(input.Norm, World);
    to_ret.Tex = input.Text;

    return to_ret;
}

float4 PixelShaderFunc( PixelShaderInput input ) : SV_Target
{
    float4 final_color = 0;

    for (int i = 0; i < 2; ++i)
    {
        final_color += saturate(
			dot((float3) vLightDir[i], input.Norm) *
			vLightColor[i]
        );
    }
    // Метод Smaple вертає колір точки з малюнка текстури
    final_color *= txDiffuse.Sample(samLinear, input.Tex);
    final_color.a = 1.0f;

    return final_color;
}

float4 PixelShaderSolidFunc( PixelShaderInput input ) : SV_Target
{
    return vOutputColor;
}

