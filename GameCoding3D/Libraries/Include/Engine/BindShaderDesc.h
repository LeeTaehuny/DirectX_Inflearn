#pragma once
#include "ConstantBuffer.h"

class Shader;

// VP와 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다.
struct GlobalDesc
{
	Matrix V = Matrix::Identity;
	Matrix P = Matrix::Identity;
	Matrix VP = Matrix::Identity;
	Matrix VInv = Matrix::Identity;
};

// W와 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다.
struct TransformDesc
{
	Matrix W = Matrix::Identity;
};

// Light와 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다.
struct LightDesc
{
	Color ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color emissive = Color(1.0f, 1.0f, 1.0f, 1.0f);

	Vec3 direction;

	float padding0;
};

struct MaterialDesc
{
	Color ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Color emissive = Color(0.f, 0.f, 0.f, 1.f);
};

// Bone과 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다.
#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

struct BoneDesc
{
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

// Animation과 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다.
struct KeyframeDesc
{
	// 현재 실행중인 애니메이션 번호
	int32 animIndex = -1;
	// 현재 프레임 카운트
	uint32 currFrame = 0;

	// 다음 프레임 카운트
	uint32 nextFrame = 0;
	
	float ratio = 0.0f;
	float sumTime = 0.0f;
	float speed = 1.0f;

	// padding
	Vec2 padding;
};

// Animation과 관련된 정보를 버퍼에 넘겨주기 위한 정보 구조체를 선언합니다. (Tween)
struct TweenDesc
{
	TweenDesc()
	{
		curr.animIndex = 0;
		next.animIndex = -1;
	}

	void ClearNextAnim()
	{
		next.animIndex = -1;
		next.currFrame = 0;
		next.nextFrame = 0;
		next.sumTime = 0;
		tweenSumTime = 0;
		tweenRatio = 0;
	}

	float tweenDuration = 1.0f;
	float tweenRatio = 0.f;
	float tweenSumTime = 0.f;
	float padding = 0.f;
	KeyframeDesc curr;
	KeyframeDesc next;
};

struct InstancedTweedDesc
{
	TweenDesc tweens[MAX_MODEL_INSTANCE];
};

struct SnowBillboardDesc
{
	// 눈의 색상
	Color color = Color(1, 1, 1, 1);

	// 속도
	Vec3 velocity = Vec3(0, -5, 0);
	// 거리
	float drawDistance = 0;

	// 위치
	Vec3 origin = Vec3(0, 0, 0);
	// 흔들리는 강도
	float turbulence = 5;

	// 범위
	Vec3 extent = Vec3(0, 0, 0);
	// 시간
	float time;
};