#include "ShadowHeader.hlsli"

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 WorldPos : POSITION;
    float4 ZCalcTex : TEXCOORD1; //Z値算出用テクスチャ
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(wvp, pos);
    float3 norm = mul(world, float4(normal, 0)).xyz;
    output.Normal = normalize(norm);
    output.UV = uv;
    output.WorldPos = mul(world, pos).xyz;

    matrix mat = mul(lightView, world);
    mat = mul(lightProj, mat);
    output.ZCalcTex = mul(mat, pos);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 viewDir = normalize(cameraPos - input.WorldPos);
    float4 zCalcTex = input.ZCalcTex;

    float NL = dot(normal, -lightDir);

    //拡散反射光
    float3 diff = NL * diffuse.rgb;

    //反射光ベクトル
    float3 reflect = normalize(lightDir + 2 * NL * normal);
    //鏡面反射光
    float3 spec = pow(saturate(dot(reflect, viewDir)), shininess) * specular;

    float3 color = saturate(ambient + diff + spec) * lightColor;
    float4 texColor = tex.Sample(samplerState, input.UV);

    float4 outColor = float4(color * texColor.rgb, diffuse.a * texColor.a);

    //ライト目線によるZ値の再算出
    float ZValue = zCalcTex.z / zCalcTex.w;

    //テクスチャ座標に変換
    float2 transTexCoord;
    transTexCoord.x = (1.0 + zCalcTex.x / zCalcTex.w) * 0.5;
    transTexCoord.y = (1.0 - zCalcTex.y / zCalcTex.w) * 0.5;

    // 同じ座標のZ値を抽出
    float SM_Z = depthTex.Sample(samplerState, transTexCoord).x;

    //算出点がシャドウマップのZ値よりも大きければ影と判断
    if (ZValue > SM_Z + 0.004)
    {
        outColor.rgb *= 0.2;
    }

    return outColor;
}