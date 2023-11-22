#include "00. Global.fx"


// 물체가 흡수하는 색상을 저장하기 위한 변수
float4 MaterialEmissive;

// VS
// * 공용 셰이더에서 사용할 VertexInput 정보를 가져옵니다.
// * VertexTextureNormal
MeshOutput VS(VertexTextureNormal input)
{
	// output 구조체를 변경합니다. 
	MeshOutput output;

	// 상수 버퍼의 내용을 적용합니다.
	// * 현재는 모든 정점에 대해 V와 P Matrix를 적용시켜주고 있습니다.
	output.position = mul(input.position, W);
	
	// worldPosition 정보(물체의 월드 위치)를 추가로 저장합니다. (조명연산에 필요)
	output.worldPosition = input.position;

	output.position = mul(output.position, VP);
	output.uv = input.uv;

	// Normal 좌표는 물체가 이동(회전)하면 변경됩니다.
	// * 물체의 World(회전값만 존재하는)좌표를 적용해줍니다.
	// * 물체의 이동 정보는 행렬의 마지막 줄에 저장되어 있습니다.
	//   -> 회전만 적용하기 위해서는 마지막 줄을 제거하면 됩니다.
	output.normal = mul(input.normal, (float3x3)W);

	return output;
}

// Emissive
// * 외각선을 구할 때 사용 (림라이트)

// PS
// * VS의 결과물로 나오는 output이 변경되었습니다. 동일하게 설정합니다.
float4 PS(MeshOutput input) : SV_TARGET
{
	// 카메라의 위치를 구해줍니다.
	float3 cameraPosition = -V._41_42_43;
	// 물체로부터 카메라로 향하는 방향 벡터를 구해줍니다.
	float3 E = normalize(cameraPosition - input.worldPosition);

	// 방향벡터 E와 normal벡터를 내적합니다.
	// * 0~1 사이의 값만 가지게 하도록 saturate() 함수를 사용합니다.
	float value = saturate(dot(E, input.normal));
	// 값을 반전합니다.
	float emissive = 1.0f - value;

	// 그래프를 부드럽게 보간해줍니다.
	emissive = smoothstep(0.0f, 1.0f, emissive);

	// 효과를 눈에 띄게 만들어줍니다.
	emissive = pow(emissive, 3);

	// 반환하기 위한 색상을 구해줍니다.
	// * 물체가 흡수하는 색상 * emissive
	float4 color = MaterialEmissive * emissive;

	return color;
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
};