#pragma once

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

struct AnimationData
{
	Vec2 spriteOffset;
	Vec2 spriteSize;
	Vec2 textureSize;

	// GPU로 넘겨줄 때는 16바이트로 정렬을 하는 과정이 필요
	float useAnimation;
	float padding;
};
