#pragma once
#include "Component.h"

class Button : public Component
{
	using Super = Component;
public:
	Button();
	virtual ~Button();

	// 충돌 판별용 함수를 선언합니다. (선택 여부 반환)
	bool Picked(POINT screenPos);

	// 버튼 생성을 위한 함수를 선언합니다.
	void Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material);

	// 버튼이 눌리면 실행할 함수를 저장하기 위한 함수를 선언합니다.
	void AddOnClickedEvent(std::function<void(void)> func);
	// 실제로 버튼이 눌린 경우 등록된 함수를 실행하기 위한 함수를 선언합니다.
	void InvokeOnClicked();

private:
	// void 입력 / 반환 함수
	std::function<void(void)> _onClicked;

	// 영역 판별용 RECT
	RECT _rect;
};

