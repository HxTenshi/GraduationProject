//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register(s0);

Texture2D txSphereMap : register(t1);
SamplerState samSphereMapLinear : register(s1);

Texture2D txToonMap : register(t2);
SamplerState samToonMapLinear : register(s2);

cbuffer cbNeverChanges : register( b0 )
{
	matrix View;
};

cbuffer cbChangeOnResize : register( b1 )
{
	matrix Projection;
};

cbuffer cbChangesEveryFrame : register( b2 )
{
	matrix World;
};

cbuffer cbChangesLight : register( b3 )
{
	float4 LightVect;
	float4 LightColor;
};
cbuffer cbChangesMaterial : register( b4 )
{
	float4 MDiffuse;
	float4 MSpecular;
	float4 MAmbient;
};
cbuffer cbChangesLightMaterial : register( b5 )
{
	float4 LDiffuse;
	float4 LSpecular;
	float4 LAmbient;
};

cbuffer cbChangesUseTexture : register( b6 )
{
	float4 UseTexture;
};
cbuffer cbBoneMatrix : register(b7)
{
	//float4x3 BoneMatrix[153];
	float4x3 BoneMatrix[2] :  packoffset(c0);
	float4x3 BoneMatrixDmmy[253] :  packoffset(c6);
	float4x3 BoneMatrixEnd :  packoffset(c765);
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
	uint4 BoneIdx		: BONEINDEX;
	uint4 BoneWeight	: BONEWEIGHT;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
	float4 VPos		: TEXCOORD1;
};

struct PS_OUTPUT_1
{
	float4 Color0 : SV_Target0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul( input.Pos, World );
	output.VPos = mul( output.Pos, View );
	output.Pos = mul( output.VPos, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	//output.Normal = mul(output.Normal, (float3x3)View);

	output.Tex = input.Tex;
	
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;

	// �@���̏���
	float3 N = normalize(input.Normal);
	
	// ���C�g�̃x�N�g��
	float3 L = normalize(LightVect.xyz);
	
	// ���_���W���王�_�ւ̃x�N�g���𐳋K��
	float3 V = normalize(-input.VPos.xyz);
	
	// �f�B�t���[�Y�p�x�������v�Z
	float DifGen = saturate(dot(N, -L));
	
	// �n�[�t�x�N�g���̌v�Z norm( ( norm( ���_�ʒu���王�_�ւ̃x�N�g�� ) + ���C�g�̕��� ) )
	float H = normalize(V - L);

	float SpeGen = pow(max(0.0f, dot(N, H)), (MSpecular.a));

	float3 SpeColor = SpeGen * LSpecular.rgb;


	// �o�̓J���[�v�Z +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �J�n )

	float4 DifColor = float4(1.0, 1.0, 1.0, 1.0);
	if (UseTexture.x != 0.0)
		DifColor = txDiffuse.Sample(samLinear, input.Tex);


	// �f�B�t���[�Y�J���[ = ���C�g�̃f�B�t���[�Y�J���[�~�ϒl * �}�e���A���̃f�B�t���[�Y�J���[
	float4 LMDif = LDiffuse * MDiffuse * DifGen;
	float4 LMAmb = LAmbient * MAmbient;
	// �X�y�L�����J���[ = ���C�g�̃X�y�L�����J���[�~�ϒl * �}�e���A���̃X�y�L�����J���[
	float3 LMSpe = SpeColor * MSpecular.rgb;
	//LMSpe = float3(0,0,0);

	float4 Color;
	// �o�� = saturate( saturate( �f�B�t���[�Y�J���[ * �A���r�G���g�J���[�̒~�ϒl ) * �g�D�[���e�N�X�`���J���[ + �X�y�L�����J���[ ) * �e�N�X�`���J���[
	Color.rgb = saturate(saturate(LMDif.rgb + LMAmb.rgb) + LMSpe.rgb) * DifColor.rgb;

	// �A���t�@�l = �f�B�t���[�Y�A���t�@ * �}�e���A���̃f�B�t���[�Y�A���t�@ * �s�����x
	Color.a = DifColor.a * MDiffuse.a;

	Out.Color0 = saturate(Color);
	return Out;
}
