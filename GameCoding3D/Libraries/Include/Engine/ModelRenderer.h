#pragma once
#include "Component.h"

class Model;
class Shader;
class Material;

// Model을 출력하기 위한 컴포넌트입니다.
class ModelRenderer : public Component
{
	using Super = Component;
public:
	ModelRenderer(shared_ptr<Shader> shader);
	virtual ~ModelRenderer();

	virtual void Update() override;

	// 모델을 설정하기 위한 함수를 선언합니다.
	void SetModel(shared_ptr<Model> model);
	// Pass 정보를 설정하기 위한 함수를 정의합니다.
	void SetPass(uint8 pass) { _pass = pass; }

	void RenderInstancing(shared_ptr<class InstancingBuffer>& buffer);

	// 인스턴스의 ID를 반환하기 위한 함수를 선언합니다.
	InstanceID GetInstanceID();
	
private:
	shared_ptr<Shader>	_shader;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;

};

