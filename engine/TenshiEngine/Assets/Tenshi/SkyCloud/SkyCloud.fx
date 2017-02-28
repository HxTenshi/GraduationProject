// Cloud dome. By David Hoskins,2014.
// https://www.shadertoy.com/view/4tf3RM#
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// It ray-traces to the bottom layer then steps through to the top layer.
// It uses the same number of steps for all positions.

// \/\/ - set to .08 for useful flatten value for a dome texture, or .5 to see the clouds here nicely (not useful)
#define FLATTEN .2
#define NUM_STEPS 70

//static const float3 sunLight = normalize(float3(0.35, 0.22, 0.3));
//static const float3 sunColour = float3(1.0, .86, .7);

#define cloudLower 2400.0
#define cloudUpper 3800.0

#define cloudSpeed 0.1

static const float cloudThickness = 0.01;

static const float3 underSkyColour = float3(.1, .45, .7);
static const float3 topSkyColour = float3(.1, .1, .4);


//#define TEXTURE_NOISE


static const float cloudy = 0.01;
static const float2 add = float2(1.0, 0.0);
#define MOD3 float3(3.07965, 7.1235, 4.998784)




//Texture2D AlbedoTex : register(t0);
//SamplerState AlbedoSamLinear : register(s0);

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
	matrix BeforeWorld;
};

cbuffer cbChangesLight : register(b3)
{
	float4 LightVect;
	float4 LightColor;
};

cbuffer cbChangesLightMaterial : register(b5)
{
	float4 LDiffuse;
	float4 LSpecular;
	float4 LAmbient;
};

cbuffer cbGameParameter : register(b11)
{
	//x=AllCount,y=DeltaTimeCount,z=DeltaTime
	float4 GameTime;
};

cbuffer cbNearFar : register(b12)
{
	float Near;
	float Far;
	float2 NULLnf;
};

struct VS_INPUT
{
	float4 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 Tex		: TEXCOORD2;
};

float GetTime() {
	return GameTime.y;
}
float3 sunLight() {
	return -LightVect.xyz;
}
float3 sunColour() {
	return LightColor.rgb;
}



//--------------------------------------------------------------------------
// A new Hash from https://www.shadertoy.com/view/XlfGWN
float Hash(float3 p)
{
	p = frac(p / MOD3);
	p += dot(p.xyz, p.yzx + 19.19);
	return frac(p.x * p.y * p.z);
}


//--------------------------------------------------------------------------
#ifdef TEXTURE_NOISE
float Noise(in float3 x)
{
	float3 p = floor(x);
	float3 f = frac(x);
	f = f*f*(3.0 - 2.0*f);

	float2 uv = (p.xy + float2(37.0, 17.0)*p.z) + f.xy;
	//float2 rg = textureLod(iChannel0, (uv + 0.5) / 256.0, 0.0).yx;
	float2 rg = AlbedoTex.Sample(AlbedoSamLinear, (uv + 0.5) / 256.0).yx;
	return lerp(rg.x, rg.y, f.z);
}
#else
//--------------------------------------------------------------------------
float Noise(in float3 p)
{
	float3 i = floor(p);
	float3 f = frac(p);
	f *= f * (3.0 - 2.0*f);

	return lerp(
		lerp(lerp(Hash(i), Hash(i + add.xyy), f.x),
			lerp(Hash(i + add.yxy), Hash(i + add.xxy), f.x),
			f.y),
		lerp(lerp(Hash(i + add.yyx), Hash(i + add.xyx), f.x),
			lerp(Hash(i + add.yxx), Hash(i + add.xxx), f.x),
			f.y),
		f.z);
}
#endif

//--------------------------------------------------------------------------
float FBM(float3 p)
{
	p *= .5;
	float f;
	f = 0.5000   * Noise(p); p = p * 3.02;
	f += 0.2500   * Noise(p); p = p * 3.03;
	f += 0.1250   * Noise(p); p = p * 3.01;
	f += 0.0625   * Noise(p); p = p * 3.03;
	f += 0.03125  * Noise(p); p = p * 3.02;
	f += 0.015625 * Noise(p);
	return f;
}


//--------------------------------------------------------------------------
float Map(float3 p)
{
	float h = FBM(p);
	return h - cloudy - 0.42;
}

//--------------------------------------------------------------------------
float GetHorizonFog(in float3 rd)
{
	return 1.0 - min(pow(1.0 - rd.y, 15.)*2.0, 1.0);
}

//--------------------------------------------------------------------------
void AddSun(in float3 rd, inout float3 sky, inout float4 cloud, float fog)
{
	float sunAmount = max(dot(rd, sunLight()), 0.0);
	float3 sun = sunColour() * min(pow(sunAmount, 1500.0) * 2.0, 1.0);
	sun += sunColour() * min(pow(sunAmount, 10.0) * .75, 1.0);
	sky += sun * fog;
	cloud.xyz += min((1. - cloud.a) * pow(sunAmount, 3.0), 1.0);
}

//--------------------------------------------------------------------------
// Grab all sky information for a given ray from camera
float3 GetSky(in float3 pos, in float3 rd)
{
	// Do the blue and sun...	
	float3  sky = lerp(topSkyColour, underSkyColour, 1.0 - pow(abs(rd.y), 0.5));

	// Find the start and end of the cloud layer...
	float beg = ((cloudLower - pos.y) / rd.y);
	float end = ((cloudUpper - pos.y) / rd.y);
	// Start position...
	float3 p = float3(pos.x + rd.x * beg, cloudLower, pos.z + rd.z * beg);

	// Trace clouds through that layer...
	float d = 0.0;
	float add = (end - beg) / float(NUM_STEPS);
	float4 sum = float4(0.1, .1, .1, 0.0);
	// Horizon fog is just thicker clouds...
	//float4 col = float4(0.0, 0.0, 0.0, pow(1.0-rd.y,30.) * .2);
	float4 col = float4(0.0, 0.0, 0.0, 0.0);
	for (int i = 0; i < NUM_STEPS; i++)
	{
		if (sum.a >= 1.0) continue;
		float3 pos = p + rd * d;
		float h = Map(pos * .001);
		col.a += max(-h, 0.0) * cloudThickness;
		float cloud = (pos.y - cloudLower) / (cloudUpper - cloudLower);
		col.rgb = lerp(float3(cloud, cloud, cloud) * col.a, sunColour(), max(.3 - col.a, 0.0) * .04);
		sum = sum + col*(1.0 - sum.a);
		d += add;
	}

	float fog = GetHorizonFog(rd);
	sum.a *= fog;
	AddSun(rd, sky, sum, fog);

	sky = lerp(sky, sum.xyz, sum.a);

	return clamp(sky, 0.0, 1.0);
}

//--------------------------------------------------------------------------
float3 CameraPath(float t)
{
	//return float3(4000.0 * sin(.16*t), 0.0, 4000.0 * cos(.155*t) );
	return float3(4000.0 * (.16*t), 0.0, 4000.0 * (.155*t)) * cloudSpeed;
}


//--------------------------------------------------------------------------
float3 IntoSphere(float2 uv)
{
	float3 dir;
	uv = (-1.0 + 2.0 * uv);
	dir.x = uv.x;
	dir.z = uv.y;
	dir.y = sqrt(1.0 - dir.x * dir.x - dir.z*dir.z) * FLATTEN;
	if (length(dir) >= 1.0) return float3(0.0, .001, .999);
	dir = normalize(dir);

	return dir;
}

//--------------------------------------------------------------------------
float2 IntoCartesian(float3 dir)
{
	float2 uv;
	dir.y /= FLATTEN;
	dir = normalize(dir);
	uv.x = dir.x;
	uv.y = dir.z;
	uv = .5 + (.5 * uv);
	return uv;
}

//--------------------------------------------------------------------------
void mainImage(out float4 fragColor, in float3 normal)
{

	//float m = (iMouse.x/iResolution.x)*30.0;
	//gTime = GetTime();// +m - 0.8;
						  //cloudy = cos(gTime * .27+.4) * .3;

	//float2 uv = fragCoord.xy;// / iResolution.xy;

	float3 dir = normal;// IntoSphere(uv);

	//uv  = IntoCartesian(dir); // ...Test conversion!
	//dir = IntoSphere(uv);

	float3 col = GetSky(CameraPath(GetTime()), dir);

	// Don't gamma too much to keep the moody look...
	col = pow(col, float3(.6,.6,.6));
	fragColor = float4(col, 1.0);
}


//--------------------------------------------------------------------------

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);

	//float3x3 WV = mul((float3x3)World, (float3x3)View);
	output.Normal = mul(input.Normal, (float3x3)World);
	output.Normal = normalize(output.Normal);

	output.Tex = input.Tex;

	return output;
}

struct PS_OUTPUT_1
{
	float4 ColorAlbedo : SV_Target0;
	float4 ColorSpecular : SV_Target1;
	float4 ColorNormal : SV_Target2;
	float4 ColorDepth : SV_Target3;
	float4 ColorVelocity : SV_Target4;
};
PS_OUTPUT_1 PS(PS_INPUT input)
{
	PS_OUTPUT_1 Out;
	//input.Tex = input.Tex * 0.5 + float2(-0.5,-0.5);
	mainImage(Out.ColorSpecular, -input.Normal);
	Out.ColorSpecular.a = -1;
	Out.ColorAlbedo = float4(0,0,0,0);
	Out.ColorNormal = float4(0,0,0,0);
	Out.ColorDepth = float4(1,1,1,1);
	Out.ColorVelocity = float4(0.5, 0.5, 0, 1);

	return Out;
}
