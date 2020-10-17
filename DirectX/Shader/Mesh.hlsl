Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer global_0 : register(b0)
{
    matrix world : packoffset(c0); //���[���h�s��
    matrix wvp : packoffset(c4); //���[���h����ˉe�܂ł̕ϊ��s��
    float3 lightDir : packoffset(c8); //���C�g�̕����x�N�g��
    float3 cameraPos : packoffset(c9); //�J�����ʒu
};

cbuffer global_1 : register(b1)
{
    float4 diffuse : packoffset(c0) = float4(1, 1, 1, 1); //�g�U����(�F�j
    float3 specular : packoffset(c1) = float3(1, 1, 1); //���ʔ���
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
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
    output.Pos = mul(wvp, pos);
    output.WorldPos = mul(world, pos);
    output.Normal = normal;
	//���C�g����
    output.Light = lightDir;
	//�����x�N�g��
    float3 posWorld = mul(world, pos);
    output.EyeVector = cameraPos - posWorld;
	
    float3 Normal = normalize(output.Normal);
    float3 LightDir = normalize(output.Light);
    float3 ViewDir = normalize(output.EyeVector);
    float NL = saturate(dot(Normal, LightDir));
	
    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

    output.Color = diffuse * NL + specular * specular;
	
	//�e�N�X�`���[���W
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