Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //ワールド行列
    matrix wvp : packoffset(c4); //ワールドから射影までの変換行列
    float3 lightDir : packoffset(c8); //ライトの方向ベクトル
    float3 cameraPos : packoffset(c9); //カメラ位置
};

cbuffer global_1 : register(b1)
{
    float4 diffuse : packoffset(c0) = float4(1, 1, 1, 1); //拡散反射(色）
    float3 specular : packoffset(c1) = float3(1, 1, 1); //鏡面反射
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
    float3 Light : TEXCOORD0;
    float3 EyeVector : TEXCOORD1;
    float2 UV : TEXCOORD2;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
    output.Pos = mul(wvp, pos);
    output.WorldPos = mul(world, pos);
    output.Normal = normal;
	//ライト方向
    output.Light = lightDir;
	//視線ベクトル
    float3 posWorld = mul(world, pos);
    output.EyeVector = cameraPos - posWorld;
	
    float3 Normal = normalize(output.Normal);
    float3 LightDir = normalize(output.Light);
    float3 ViewDir = normalize(output.EyeVector);
    float NL = saturate(dot(Normal, LightDir));
	
    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

    output.Color = diffuse * NL + specular * specular;
	
	//テクスチャー座標
    output.UV = uv;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = input.Color;
    //if (isTexture == 1)
    //{
    //    color = g_texDecal.Sample(g_samLinear, input.UV);
    //}
    color.a = diffuse.a;

    return color;
}