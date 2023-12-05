#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 50

// 모델의 뼈 정보들을 저장하기 위한 상수버퍼를 정의합니다.
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

// 현재 렌더링하는 물체의 Bone Index를 관리하기 위한 전역 버퍼를 생성합니다.
uint BoneIndex;

// VS
MeshOutput VS(VertexTextureNormalTangent/*Blend*/ input)
{
	MeshOutput output;

	// TODO

	output.position = mul(input.position, BoneTransforms[BoneIndex]);

	output.position = mul(output.position, W);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, VP);
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	output.tangent = mul(input.tangent, (float3x3)W);

	return output;
}

// PS
float4 PS(MeshOutput input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};