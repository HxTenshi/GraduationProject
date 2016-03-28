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

cbuffer cbNeverChanges : register( b0 )
{
	matrix View;
	matrix ViewInv;
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

cbuffer cbChangesLightCamera : register(b10)
{
	matrix LViewProjection[4];
	float4 SplitPosition;
};

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

	//float4 LVPos	: TEXCOORD2;
	float4 LPos[4]		: TEXCOORD2;
};

struct PS_OUTPUT_1
{
	float4 ColorAlbedo : SV_Target0;
	float4 ColorNormal : SV_Target1;
	float4 ColorDepth : SV_Target2;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul( input.Pos, World );

	//output.LVPos = mul(output.Pos, LView);
	//output.LPos = mul(output.LVPos, LProjection);
	output.LPos[0] = mul(output.Pos, LViewProjection[0]);
	output.LPos[1] = mul(output.Pos, LViewProjection[1]);
	output.LPos[2] = mul(output.Pos, LViewProjection[2]);
	output.LPos[3] = mul(output.Pos, LViewProjection[3]);


	output.VPos = mul( output.Pos, View );
	output.Pos = mul( output.VPos, Projection);

	output.Normal = mul(input.Normal, (float3x3)World);
	output.Normal = mul(output.Normal, (float3x3)View);

	output.Tex = input.Tex;
	
	return output;
}



float4x3 getBoneMatrix(int idx)
{
	return BoneMatrix[idx];
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VSSkin(VS_INPUT input)
{

	float4 pos = input.Pos;
	float3 nor = input.Normal;


	float3 bpos = float3(0, 0, 0);
	float3 bnor = float3(0, 0, 0);
	[unroll]
	for (uint i = 0; i < 4; i++){
		float4x3 bm = getBoneMatrix(input.BoneIdx[i]);
		bpos += input.BoneWeight[i] * mul(pos, bm).xyz;
		bnor += input.BoneWeight[i] * mul(nor, (float3x3)bm);
	}

	pos.xyz = bpos / 100.0;
	nor = bnor / 100.0;



	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(pos, World);

	//output.LVPos = mul(output.Pos, LView);
	//output.LPos = mul(output.LVPos, LProjection);
	output.LPos[0] = mul(output.Pos, LViewProjection[0]);
	output.LPos[1] = mul(output.Pos, LViewProjection[1]);
	output.LPos[2] = mul(output.Pos, LViewProjection[2]);
	output.LPos[3] = mul(output.Pos, LViewProjection[3]);

	output.VPos = mul(output.Pos, View);
	output.Pos = mul(output.VPos, Projection);

	output.Normal = mul(nor, (float3x3)World);
	output.Normal = mul(output.Normal, (float3x3)View);

	output.Tex = input.Tex;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;

	// 法線の準備
	float3 N = normalize(input.Normal);
	N = N * 0.5 + 0.5;

	float farClip = 100;
	float D = input.VPos.z / farClip;
	//float D2 = input.Pos.z / input.Pos.w;


	float3 LD;

	float dist = input.VPos.z;
	if (dist < SplitPosition.x)
	{
		LD.xy = (input.LPos[0].xy + 1.0) / 2.0;
		LD.z = input.LPos[0].z / input.LPos[0].w;
	}
	else if (dist < SplitPosition.y)
	{
		LD.xy = (input.LPos[1].xy + 1.0) / 2.0;
		LD.z = input.LPos[1].z / input.LPos[1].w;
	}
	else if (dist < SplitPosition.z)
	{
		LD.xy = (input.LPos[2].xy + 1.0) / 2.0;
		LD.z = input.LPos[2].z / input.LPos[2].w;
	}
	else
	{
		LD.xy = (input.LPos[3].xy + 1.0) / 2.0;
		LD.z = input.LPos[3].z / input.LPos[3].w;
	}

	
	//MSpecular

	// 出力カラー計算 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( 開始 )

	float4 DifColor = float4(1.0, 1.0, 1.0, 1.0);
	if (UseTexture.x != 0.0)
		DifColor = txDiffuse.Sample(samLinear, input.Tex);


	Out.ColorAlbedo = DifColor * MDiffuse;
	Out.ColorNormal = float4(N,1);
	Out.ColorDepth = float4(D, 1-LD.z, LD.x, 1.0-LD.y);
	return Out;
}
