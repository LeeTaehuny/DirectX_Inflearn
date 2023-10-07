#pragma once
#include "Types.h"

// 공용으로 사용할 구조체들을 저장하기 위한 헤더입니다. 

struct TransformData
{
	// 월드 행렬
	Matrix matWorld = Matrix::Identity;

};

struct CameraData
{
	// 뷰 행렬
	Matrix matView = Matrix::Identity;
	// 프로젝션 행렬
	Matrix matProjection = Matrix::Identity;
};