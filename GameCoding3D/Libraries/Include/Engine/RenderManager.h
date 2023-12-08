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

class RenderManager
{
	DECLARE_SINGLE(RenderManager);

public:
	// 초기화 함수를 선언합니다.
	void Init(shared_ptr<Shader> shader);

	// 업데이트 함수를 선언합니다.
	void Update();

	// 전역 데이터(V, P)를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushGlobalData(const Matrix& view, const Matrix& projection);
	// 위치 데이터(W)를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushTransformData(const TransformDesc& desc);

	// 빛 정보를 셰이더에 Push하기 위한 함수들을 선언합니다.
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);

	// Bone 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushBoneData(const BoneDesc& desc);

	// Keyframe 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushKeyframeData(const KeyframeDesc& desc);

	// Tween 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushTweenData(const TweenDesc& desc);

private:
	// 셰이더마다 연결해줘야 하는 것들이 달라져야 합니다.
	// * 셰이더를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Shader> _shader;

	// 전역으로 프레임마다 한 번만 세팅하는 정보들을 저장하기 위한 변수를 선언합니다.
	GlobalDesc _globalDesc;
	// 전역 정보를 GPU에 넘겨주기 위한 상수 버퍼를 선언합니다.
	shared_ptr<ConstantBuffer<GlobalDesc>> _globalBuffer;
	// 셰이더에 어떤 버퍼를 사용하라고 Get시리즈를 통해 버퍼의 정보를 가져왔습니다.
	// * 이제부터는 사용할 상수 버퍼를 동일하게 가져와서 처리하게 될 것입니다.
	// * 매 프레임마다 가져와서 사용하는 것보다 가져온 정보를 캐싱하여 처리할 수 있도록 변수를 선언해줍니다.
	ComPtr<ID3DX11EffectConstantBuffer> _globalEffectBuffer;

	// W와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	TransformDesc _transformDesc;
	shared_ptr<ConstantBuffer<TransformDesc>> _transformBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _transformEffectBuffer;

	// Light와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	LightDesc _lightDesc;
	shared_ptr<ConstantBuffer<LightDesc>> _lightBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _lightEffectBuffer;

	MaterialDesc _materialDesc;
	shared_ptr<ConstantBuffer<MaterialDesc>> _materialBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _materialEffectBuffer;

	// Bone와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	BoneDesc _boneDesc;
	shared_ptr<ConstantBuffer<BoneDesc>> _boneBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _boneEffectBuffer;

	// Keyframe과 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	KeyframeDesc _keyframeDesc;
	shared_ptr<ConstantBuffer<KeyframeDesc>> _keyframeBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _keyframeEffectBuffer;

	// Tween 애니메이션 재생과 관련된 정보 또한 설정 가능하도록 변수럴 선언해줍니다.
	TweenDesc _tweenDesc;
	shared_ptr<ConstantBuffer<TweenDesc>> _tweenBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _tweenEffectBuffer;

};

