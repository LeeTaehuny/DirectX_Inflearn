#pragma once
#include "BaseCollider.h"

class SphereCollider : public BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider() override;

	virtual void Update() override;
	virtual bool Intersects(Ray& ray, OUT float& distance) override;

	void SetRadius(float radius) { _radius = radius; }
	BoundingSphere& GetBoundingSphere() { return _boundingSphere; }

private:
	// 외부 설정을 위한 반지름
	float _radius = 1.0f;

	// 원의 정보를 가지고 있는 구조체 타입의 변수를 선언합니다.
	BoundingSphere _boundingSphere;
};

