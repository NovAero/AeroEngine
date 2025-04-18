struct VertexPosColour
{
    float3 Position : POSITION;
    float3 Colour : COLOUR;
};

struct VertexShaderOutput
{
    float4 Colour : COLOUR;
    float4 Position : Position;
};

struct ModelViewProjection
{
    matrix MVP;
};

ConstantBuffer<ModelViewProjection> MVP_CB : register(b0);

VertexShaderOutput main(VertexPosColour IN)
{
    VertexShaderOutput OUT;
    
    OUT.Position = mul(MVP_CB.MVP, float4(IN.Position, 1.0f));
    OUT.Colour = float4(IN.Colour, 1.0f);
    
    return OUT;
}

struct PixelShaderInput
{
    float4 Colour : COLOUR;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    return IN.Colour;
}