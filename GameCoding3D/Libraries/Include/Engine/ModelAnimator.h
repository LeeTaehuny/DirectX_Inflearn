#pragma once
#include "Component.h"
#include "ConstantBuffer.h"
#include "BindShaderDesc.h"

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
	// Tween Data는 각 모델마다 따로 들고 있어야 하는 정보입니다. (인스턴싱 불가능)
	// -> 업데이트 함수를 추가로 생성합니다.
	void UpdateTweenData();

	// 모델을 설정하기 위한 함수를 선언합니다.
	void SetModel(shared_ptr<Model> model);
	// Pass 정보를 설정하기 위한 함수를 정의합니다.
	void SetPass(uint8 pass) { _pass = pass; }

	shared_ptr<Shader> GetShader() { return _shader; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);

	// 인스턴스의 ID를 반환하기 위한 함수를 선언합니다.
	InstanceID GetInstanceID();
	// TweenDesc를 반환하기 위한 함수를 선언합니다.
	TweenDesc& GetTweenDesc() { return _tweenDesc; }


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

	// 두 개의 키 프레임 정보를 저장하기 위한 변수를 선언합니다.
	TweenDesc _tweenDesc;

private:
	shared_ptr<Shader>	_shader;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};

