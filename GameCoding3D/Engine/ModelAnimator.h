#pragma once
#include "Component.h"

class Model;

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
	shared_ptr<Shader>	_shader;
	uint8				_pass = 0;
	shared_ptr<Model>	_model;
};

