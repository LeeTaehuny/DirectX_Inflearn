#include "00. Global.fx"

// VS
// * 공용 셰이더에서 사용할 VertexInput 정보를 가져옵니다.
// * VertexTextureNormal
VertexOutput VS(VertexTextureNormal input)
{
	VertexOutput output;

	// 상수 버퍼의 내용을 적용합니다.
	// * 현재는 모든 정점에 대해 V와 P Matrix를 적용시켜주고 있습니다.
	output.position = mul(input.position, W);
	output.position = mul(output.position, VP);

	output.uv = input.uv;

	// Normal 좌표는 물체가 이동(회전)하면 변경됩니다.
	// * 물체의 World(회전값만 존재하는)좌표를 적용해줍니다.
	// * 물체의 이동 정보는 행렬의 마지막 줄에 저장되어 있습니다.
	//   -> 회전만 적용하기 위해서는 마지막 줄을 제거하면 됩니다.
	output.normal = mul(input.normal, (float3x3)W);

	return output;
}

Texture2D Texture0;

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	// 공용 셰이더에서 설정한 SamplerState를 적용시켜줍니다.
	return Texture0.Sample(LinearSampler, input.uv);
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
};