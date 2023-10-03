#pragma once
#include "Types.h"

// 공용으로 사용할 구조체들을 저장하기 위한 헤더입니다. 

// 위치, 회전, 크기를 가지는 구조체를 선언합니다.
struct TransformData
{
	// 월드 행렬
	Matrix matWorld = Matrix::Identity;
	// 뷰 행렬
	Matrix matView = Matrix::Identity;
	// 프로젝션 행렬
	Matrix matProjection = Matrix::Identity;
};