Texture2D g_tex : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world; //ワールド行列
    matrix wvp; //ワールドから射影までの変換行列
};

cbuffer global_1 : register(b1)
{
    float4 diffuse;
    float4 specular;
    float textureFlag;
}

//バーテックスバッファー出力
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 WorldNormal : NORMAL;
    float2 UV : TEXCOORD;
};
//ピクセルシェーダー出力
struct PS_OUTPUT
{
    float4 Color : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Specular : SV_Target3;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    output.Pos = mul(wvp, Pos);
    output.WorldPos = mul(world, Pos);
    Norm.w = 0;
    output.WorldNormal = mul(world, Norm);

    output.UV = UV;

    return output;
}

PS_OUTPUT PS(VS_OUTPUT input)
{
    PS_OUTPUT Out = (PS_OUTPUT) 0;

    //カラーテクスチャーへ出力
    Out.Color = diffuse;
    if (textureFlag == 1)
    {
        Out.Color = g_tex.Sample(g_samLinear, input.UV);
    }

    //ワールド法線テクスチャーへ出力
    float3 normal = input.WorldNormal.xyz;
    Out.Normal = float4(normal * 0.5 + 0.5, 1.0);

    //ワールド座標テクスチャーへ出力
    Out.Position = float4(input.WorldPos.xyz, 1.0);

    //スペキュラテクスチャへ出力
    Out.Specular = float4(specular.xyz, 1.0);

    return Out;
}
