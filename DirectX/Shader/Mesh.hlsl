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
    float4 diffuse : packoffset(c0) = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 specular : packoffset(c1) = float4(1, 1, 1, 1); //���ʔ���
    float isTexture : packoffset(c2) = 0; //�e�N�X�`���[���\���Ă��郁�b�V�����ǂ����̃t���O
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 Color : COLOR0;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
    float2 Tex : TEXCOORD3;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 UV : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
    Norm.w = 0; //�ړ������𔽉f�����Ȃ�
    output.Normal = mul(world, Norm);
    output.Pos = mul(wvp, Pos);
    output.WorldPos = mul(world, Pos);
	//���C�g����
    output.Light = lightDir;
	//�����x�N�g��
    float3 posWorld = mul(world, Pos);
    output.EyeVector = cameraPos - posWorld;
	
    float3 Normal = normalize(output.Normal);
    float3 LightDir = normalize(output.Light);
    float3 ViewDir = normalize(output.EyeVector);
    float NL = saturate(dot(Normal, LightDir));
	
    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

    output.Color = diffuse * NL + specular * specular;
	
	//�e�N�X�`���[���W
    output.Tex = UV;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = diffuse;
    //float4 color = input.Color;
    if (isTexture == 1)
    {
        color = g_texDecal.Sample(g_samLinear, input.Tex);
    }
    color.a = diffuse.a;

    return color;
}