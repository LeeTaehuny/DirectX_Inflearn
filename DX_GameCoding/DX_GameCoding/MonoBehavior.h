#pragma once
#include "Component.h"

// GameObject에 적용할 스크립트용 컴포넌트 클래스입니다.
// * 새로운 기능을 GameObject에 적용하고 싶다면 이를 상속받은 클래스를 만들고 붙여주면 됩니다.
class MonoBehavior : public Component
{
	using Super = Component;
public:
	MonoBehavior();
	virtual ~MonoBehavior() override;

	virtual void Awake() override;
	virtual void Update() override;

};

