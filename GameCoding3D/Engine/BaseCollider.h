#pragma once
#include "Component.h"

// 충돌체 종류를 구분하기 위한 열거형을 선언합니다.
enum class ColliderType
{
	Sphere,
	AABB,
	OBB,
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider() override;

	// RayCasting
	virtual bool Intersects(Ray& ray, OUT float& distance) = 0;
	// Other Collider
	virtual bool Intersects(shared_ptr<BaseCollider>& other) = 0;

	ColliderType GetColliderType() { return _colliderType; }

protected:
	ColliderType _colliderType;
};

