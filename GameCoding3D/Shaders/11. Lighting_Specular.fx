#include "00. Global.fx"

// 빛의 방향을 저장하기 위한 변수
float3 LightDir;
// 빛의 색상을 저장하기 위한 변수
float4 LightSpecular;
// 물체가 흡수하는 색상을 저장하기 위한 변수
float4 MaterialSpecular;

Texture2D DiffuseMap;

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

// Specular (반사광)
// * 한 방향으로 완전히 반사되는 빛 (Phong)

// PS
// * VS의 결과물로 나오는 output이 변경되었습니다. 동일하게 설정합니다.
float4 PS(MeshOutput input) : SV_TARGET
{
	// 반사된 빛의 벡터를 구해줍니다.
	// 1. 내장 함수 이용하기 reflect()
	// 2. 직접 구하기
	//float3 R = reflect(LightDir, input.normal);

	// * dot(LightDir, input.normal)와 input.normal을 곱해준 이유
	//   -> dot(LightDir, input.normal)의 결과는 스칼라 값 -> 벡터로 변경해주기 위함
	float3 R = LightDir - (2 * input.normal * dot(LightDir, input.normal));

	// 카메라의 위치를 구해줍니다.
	// * View 행렬의 4-1,2,3 인자를 꺼내 -부호를 붙여준 것이 카메라의 위치입니다.
	float3 cameraPosition = -V._41_42_43;

	// 물체에서 카메라까지 가는 방향 벡터를 구해줍니다.
	// * 월드상의 (카메라 위치 - 물체 위치)의 정규화 
	float3 E = normalize(cameraPosition - input.worldPosition);

	// 구한 방향벡터들을 내적해 값을 추출합니다. (R, E)
	// * 항상 0 ~ 1 사이의 값을 남기기 위해 saturate() 함수 사용
	// * saturate() : 0 ~ 1 사이를 유지시켜주는 함수입니다. 0이하로 내려가면 0, 1이상으로 넘어가면 1로 유지시켜줍니다.
	float value = saturate(dot(R, E));
	// Specular 값으로 연산해 넣어줍니다.
	// * 연산이 필요한 이유 : 0 ~ 1 사이의 값을 사용하면 너무 넓은 범위가 설정됩니다.
	// * 0 ~ 1 사이의 값이므로 제곱을 통해 값을 줄여 범위를 줄여줄 수 있습니다.
	float specular = pow(value, 10);

	// 최종적으로 반환하기 위한 색상을 구해줍니다.
	// * 다른 빛들은 텍스처와 상관있게 표현했지만, Specular는 눈부심을 적용하기 위해 주로 사용하므로, 텍스처의 샘플링 없이 진행합니다.
	float4 color = LightSpecular * MaterialSpecular * specular;

	return color;
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
};