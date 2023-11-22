// pragma once와 동일한 역할
#ifndef _LIGHT_FX_
#define _LIGHT_FX_

#include "00. Global.fx"

////////////
// Struct //
////////////

// 광원의 색상 정보를 저장하기 위한 구조체를 선언합니다.
struct LightDesc
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
	float3 direction;
	float padding;
};

// 물체가 받아들이는 빛의 색상 정보를 저장하기 위한 구조체를 선언합니다.
struct MaterialDesc
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 emissive;
};

/////////////////
// ConstBuffer //
/////////////////

// 셰이더에서 각각을 따로 사용할 수 있도록 상수 버퍼를 생성해줍니다.
// * GlobalLight는 1번만 설정하면 되지만 Matrial은 물체별로 생성해야 하기 때문

// * 광원
cbuffer LightBuffer
{
	LightDesc GlobalLight;
};
// * 물체
cbuffer MaterialBuffer
{
	MaterialDesc Material;
};

/////////
// SRV //
/////////

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

//////////////
// Function //
//////////////

// 공용으로 사용할 함수들을 정의합니다.
float4 ComputeLight(float3 normal, float2 uv, float3 worldPosition)
{
	float4 ambientColor = 0;
	float4 diffuseColor = 0;
	float4 specularColor = 0;
	float4 emissiveColor = 0;

	// Ambient (주변광/환경광)
	// * 수많은 반사를 거쳐서 광원이 불분명한 빛
	// * 일정한 밝기와 색으로 표현
	{
		// 출력될 색상을 조명의 연산에 따라 결정합니다.
		float4 color = GlobalLight.ambient * Material.ambient;

		// ambientColor에 공용 셰이더에서 설정한 SamplerState를 적용시켜줍니다.
		ambientColor = DiffuseMap.Sample(LinearSampler, uv) * color;
	}

	// Diffuse (분산광)
	// * 물체의 표면에서 분산되어 눈으로 바로 들어오는 빛
	// * 각도에 따라 밝기가 다른것이 특징 (Lambert 공식)
	{
		// 공용 셰이더에서 설정한 SamplerState를 적용시켜줍니다.
		float4 color = DiffuseMap.Sample(LinearSampler, uv);

		// 출력될 색상을 조명의 연산에 따라 추가 연산해줍니다.
		float value = dot(-GlobalLight.direction, normal);
		diffuseColor = color * value * GlobalLight.diffuse * Material.diffuse;
	}

	// Specular (반사광)
	// * 한 방향으로 완전히 반사되는 빛 (Phong)
	{
		// 반사된 빛의 벡터를 구해줍니다.
		// * dot(LightDir, input.normal)와 input.normal을 곱해준 이유
		//   -> dot(LightDir, input.normal)의 결과는 스칼라 값 -> 벡터로 변경해주기 위함
		float3 R = GlobalLight.direction - (2 * normal * dot(GlobalLight.direction, normal));

		// 카메라의 위치를 구해줍니다.
		float3 cameraPosition = CameraPosition();

		// 물체에서 카메라까지 가는 방향 벡터를 구해줍니다.
		float3 E = normalize(cameraPosition - worldPosition);

		// 구한 방향벡터들을 내적해 값을 추출합니다. (R, E)
		float value = saturate(dot(R, E));
		// Specular 값으로 연산해 넣어줍니다.
		float specular = pow(value, 10);

		// 최종적으로 반환하기 위한 색상을 구해줍니다.
		specularColor = GlobalLight.specular * Material.specular * specular;
	}

	// Emissive
	// * 외각선을 구할 때 사용 (림라이트)
	{
		// 카메라의 위치를 구해줍니다.
		float3 cameraPosition = CameraPosition();
		// 물체로부터 카메라로 향하는 방향 벡터를 구해줍니다.
		float3 E = normalize(cameraPosition - worldPosition);

		// 방향벡터 E와 normal벡터를 내적합니다.
		float value = saturate(dot(E, normal));

		// 값을 반전합니다.
		float emissive = 1.0f - value;
		// 그래프를 부드럽게 보간해줍니다.
		emissive = smoothstep(0.0f, 1.0f, emissive);
		// 효과를 눈에 띄게 만들어줍니다.
		emissive = pow(emissive, 2);

		// 반환하기 위한 색상을 구해줍니다.
		emissiveColor = GlobalLight.emissive * Material.emissive * emissive;
	}

	return ambientColor + diffuseColor + specularColor + emissiveColor;
}

#endif