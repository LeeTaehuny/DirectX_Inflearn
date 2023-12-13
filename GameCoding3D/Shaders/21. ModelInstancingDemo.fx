#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250

// ���� �� �������� �����ϱ� ���� ������۸� �����մϴ�.
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

// ���� �������ϴ� ��ü�� Bone Index�� �����ϱ� ���� ���� ���۸� �����մϴ�.
uint BoneIndex;

struct VS_IN
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

	// Animation
	float4 blendIndices : BLEND_INDICES;
	float4 blendWeights : BLEND_WEIGHTS;

	// INSTANCING
	uint instanceID : SV_INSTANCEID;
	matrix world :INST;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION1;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// VS
VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	// Model�� ���� ���������� �������� root �������� �ϴ� �������� ��ȯ(root�� ����)
	output.position = mul(input.position, BoneTransforms[BoneIndex]);

	output.position = mul(output.position, input.world); // W
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);

	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

// PS
float4 PS(VS_OUT input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

// Effect11
technique11 T0
{
	PASS_VP(P0, VS, PS)
};