#include "00. Global.fx"

// 빛의 방향을 저장하기 위한 변수
float3 LightDir;
// 빛의 색상을 저장하기 위한 변수
float4 LightDiffuse;
// 물체가 흡수하는 색상을 저장하기 위한 변수
float4 MaterialDiffuse;

Texture2D DiffuseMap;

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

// Diffuse (분산광)
// * 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
// * 각도에 따라 밝기가 다른것이 특징 (Lambert 공식)

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	// 공용 셰이더에서 설정한 SamplerState를 적용시켜줍니다.
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	// 출력될 색상을 조명의 연산에 따라 추가 연산해줍니다.
	float value = dot(-LightDir, input.normal);
	color = color * value * LightDiffuse * MaterialDiffuse;

	return color;
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
};