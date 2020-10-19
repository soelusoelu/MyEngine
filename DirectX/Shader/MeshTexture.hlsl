Texture2D tex : register(t0);
SamplerState samplerState : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //���[���h�s��
    matrix wvp : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 lightDir : packoffset(c8); //���C�g�̕����x�N�g��
    float3 cameraPos : packoffset(c9); //�J�����ʒu
};

cbuffer global_1 : register(b1)
{
    float3 ambient : packoffset(c0);
    float4 diffuse : packoffset(c1);
    float3 specular : packoffset(c2);
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float3 LightDir : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
};

VS_OUTPUT VS(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    //�@�������[���h��Ԃ�
    output.Pos = mul(wvp, pos);
    output.Normal = normal;
    //�e�N�X�`���[���W
    output.UV = uv;
    //���C�g����
    output.LightDir = normalize(lightDir);
    //�����x�N�g��
    float3 posWorld = mul(world, pos).xyz;
    output.EyeVector = normalize(cameraPos - posWorld);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 normal = input.Normal;
    float3 lightDir = input.LightDir;
    float3 viewDir = input.EyeVector;
    float NL = saturate(dot(normal, -lightDir));

    //float3 Reflect = normalize(2 * NL * normal - lightDir);
    //float spec = pow(saturate(dot(Reflect, viewDir)), 4);

    float4 color = float4(ambient + diffuse.rgb * NL, diffuse.a);
    float4 texColor = tex.Sample(samplerState, input.UV);

    color *= texColor;
    color = saturate(color);

    return texColor;
}