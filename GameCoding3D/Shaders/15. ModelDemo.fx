#include "00. Global.fx"
#include "00. Light.fx"

// VS
MeshOutput VS(VertexTextureNormalTangent input)
{
	MeshOutput output;
	output.position = mul(input.position, W);
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
	// Normal Mapping 관련 함수
	// * input.normal을 전달해 연산된 새로운 normal 좌표를 input.normal에 저장하는 함수입니다.
	//ComputeNormalMapping(input.normal, input.tangent, input.uv);

	//float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
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