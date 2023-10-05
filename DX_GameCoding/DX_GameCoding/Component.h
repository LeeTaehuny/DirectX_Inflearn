#pragma once

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void Init() = 0;
	virtual void Update() = 0;

	// 컴포넌트와 연결된 주체를 반환하는 함수를 정의합니다.
	shared_ptr<GameObject> GetGameObject() { return _owner.lock(); }

protected:
	// 컴포넌트와 연결된 주체를 저장하기 위한 변수를 선언합니다.
	weak_ptr<GameObject> _owner;
};

