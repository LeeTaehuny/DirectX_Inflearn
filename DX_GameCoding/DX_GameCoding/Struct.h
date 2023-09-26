#pragma once
#include "Types.h"

// 공용으로 사용할 구조체들을 저장하기 위한 헤더입니다. 

// 정점에 대한 정보를 저장하기 위한 구조체를 선언합니다.
struct Vertex
{
	// 위치 좌표 (x, y, z)
	Vec3 position;
	// 색상 정보 (R, G, B, A)
	//Color color;

	// 텍스처 좌표 (u, v)
	Vec2 uv;
};

// 위치, 회전, 크기를 가지는 구조체를 선언합니다.
struct TransformData
{
	// 보정값
	Vec3 offset;

	// 상수 버퍼는 16바이트 정렬을 해야 하기 때문에 더미 값을 추가해줍니다.
	float dumy;
};