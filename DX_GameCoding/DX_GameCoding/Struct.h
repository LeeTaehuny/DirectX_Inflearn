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