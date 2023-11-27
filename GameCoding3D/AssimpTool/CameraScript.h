#pragma once
#include "MonoBehaviour.h"

class CameraScript : public MonoBehaviour
{
	using Supre = MonoBehaviour;

public:
	virtual void Start() override;
	virtual void Update() override;

	// 카메라의 속도를 저장하기 위한 변수를 선언합니다.
	float _speed = 10.0f;
};

