#pragma once
#include "Component.h"

class Model;

// 애니메이션의 프레임별 각각의 bone마다 transform정보를 저장하기 위한 구조체를 선언합니다. 
struct AnimTransfrom
{
	// [ ][ ][ ]... 250개의 bone Transform 정보를 담아줄 수 있음
	using TransformArrayType = array<Matrix, MAX_MODEL_TRANSFORMS>;
	// [ ][ ][ ]... 500개의 칸이 있고 각 칸다마 250개의 bone Transform 정보를 담을 수 있는 2차원 배열을 생성
	array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};

class ModelAnimator : public Component
{
	using Super = Component;

public:
	ModelAnimator(shared_ptr<Shader> shader);
	~ModelAnimator();

	virtual void Update() override;

	// 모델을 설정하기 위한 함수를 선언합니다.
	void SetModel(shared_ptr<Model> model);
	// Pass 정보를 설정하기 위한 함수를 정의합니다.
	void SetPass(uint8 pass) { _pass = pass; }

private:
	// 텍스처를 생성하기 위한 함수를 선언합니다.
	void CreateTexture();
	// 애니메이션의 Transform 정보를 생성하기 위한 함수를 선언합니다.
	void CreateAnimationTransform(uint32 index);

private:
	// 애니메이션의 개수만큼 위에서 선언한 구조체를 사용해 관리합니다.
	// -> _animTransforms를 텍스처로 만들어 셰이더에 전달할 예정입니다.
	vector<AnimTransfrom> _animTransforms;
	
	// 위 애니메이션 Transform 정보를 토대로 생성된 텍스처를 저장하기 위한 변수를 선언합니다.
	// * 리소스
	ComPtr<ID3D11Texture2D> _texture;
	// 실제 GPU에 넘겨주기 위한 SRV를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11ShaderResourceView> _srv;

	// 키 프레임 정보를 저장하기 위한 변수를 선언합니다.
	KeyframeDesc _keyframeDesc;
	// 두 개의 키 프레임 정보를 저장하기 위한 변수를 선언합니다.
	TweenDesc _tweenDesc;

private:
	shared_ptr<Shader>	_shader;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};

