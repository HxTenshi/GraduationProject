//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D NormalTex : register( t0 );
SamplerState NormalSamLinear : register(s0);
Texture2D DepthTex : register(t1);
SamplerState DepthSamLinear : register(s1);

cbuffer cbNeverChanges : register(b0)
{
	matrix View;
	matrix ViewInv;
};

cbuffer cbChangeOnResize : register(b1)
{
	matrix Projection;
	matrix ProjectionInv;
};

cbuffer cbChangesEveryFrame : register(b2)
{
	matrix World;
};

cbuffer cbChangesPointLight : register(b8)
{
	float4 PtLVPos;
	float4 PtLCol;
	float4 PtLPrm;//������ = Prm.x*r^Prm.w + Prm.y, Prm.z = �e�����a
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 Pos		: POSITION;
	//float3 Normal	: NORMAL;
	//float2 Tex		: TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float4 VPos		: TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.VPos = mul(output.Pos, View);
	output.Pos = mul(output.VPos, Projection);

	return output;
}



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

//projection depth 0~1
float3 DepthToPosition2(float2 textureCoord, float depth)
{
	float3 vpos;
	// �X�N���[�����W�@����(-1,+1) �E��(+1,-1)
	float2 spos = textureCoord * float2(2, -2) + float2(-1, 1);
	vpos.z = Projection._m23 / (depth/* - Projection._m22*/);
	vpos.xy = (spos.xy*vpos.z) / float2(Projection._m00, Projection._m11);

	return vpos;
}
//view depth
float3 DepthToPosition(float2 textureCoord, float depth)
{
	const float FarClip = 100.0;
	depth *= FarClip;
	float4 projectedPosition = float4(textureCoord.xy * float2(2, -2) + float2(-1, 1), 0.0, 1.0);

	float3 viewPosition = mul(projectedPosition, ProjectionInv).xyz;
	float3 viewRay = float3(viewPosition.xy / viewPosition.z, 1.0);
	return viewRay * depth;
}

//view depth 0~1
float ToPerspectiveDepth(float viewDepth)
{
	float4 projectedPosition = mul(float4(0, 0, viewDepth, 1.0), Projection);
	return 1.0 - (projectedPosition.z / projectedPosition.w);
}
//view depth 0~1
float ToPerspectiveDepth2(float viewDepth)
{
	const float FarClip = 100.0;
	const float NearClip = 0.01;

	///@note Left-hand coordinate system
	float a = FarClip / (FarClip - NearClip);
	float b = (NearClip * FarClip) / (NearClip - FarClip);
	float z = viewDepth * a + b;
	float w = viewDepth;
	return 1.0 - (z / w);
}

float4 PS(PS_INPUT input) : SV_Target
{

	//�W�I���g���o�b�t�@�̃e�N�X�`�����W�i�@���A�f�v�X�l�擾�p�j
	float2 tex = input.Pos.xy/float2(1200,800);


	// �ʒu(view���W�n)
	float dep = DepthTex.Sample(DepthSamLinear, tex).r;
	dep = ToPerspectiveDepth(dep);
	float3 vpos = DepthToPosition2(tex, dep);


	// �ʒu(view���W�n) = -��������

	// �@��(view���W�n) 0~1��-1�`+1�ɕϊ�
	float3 nor = NormalTex.Sample(NormalSamLinear, tex).xyz * 2 - 1;

	//float3 lpos = mul(PtLVPos, View).xyz;
	float3 lpos = PtLVPos.xyz;

	//�_�����܂ł̋���
	float3 dir = lpos - vpos;
	float r = length(dir);

	// �����̕���(view���W�n)
	dir = dir / r;

	// ����  A*r^n + B
	float atte = saturate(PtLPrm.x*pow(r, PtLPrm.w) + PtLPrm.y);

	//float n = 1;//�C�ӂ̒l(��0)�@�����Ȑ��𐧌�
	//float rmin = 0;// �����J�n�����@������ = 1
	//float rmax = 2.5;// �e���͈́@������ = 0
	//float _rmin = pow(rmin, n);
	//float _rmax = pow(rmax, n);
	//float A = -1 / (_rmax - _rmin);
	//float B = _rmax / (_rmax - _rmin);
	//
	//atte = saturate(A*pow(r, n) + B);

	// �g�U
	float df = saturate(dot(nor, dir));

	// ����
	//float3 h = normalize(dir - normalize(vpos));//�n�[�t�x�N�g��
	//float spec_pow = 255 * txSpecPow.Sample(samSpecPow, tex).x;
	//float3 spec = pow(saturate(dot(nor, h)), spec_pow);

	float4 Out;
	Out = float4(atte*df*PtLCol.xyz, 1);
	//Out.spec = float4(atte*spec*PtLCol.xyz, 1);
	return Out;
}