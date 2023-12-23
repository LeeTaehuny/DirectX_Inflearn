#include "pch.h"
#include "OBBBoxCollider.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"

OBBBoxCollider::OBBBoxCollider() : BaseCollider(ColliderType::OBB)
{
}

OBBBoxCollider::~OBBBoxCollider()
{
}

void OBBBoxCollider::Update()
{
}

bool OBBBoxCollider::Intersects(Ray& ray, OUT float& distance)
{
	return _boundingBox.Intersects(ray.position, ray.direction, OUT distance);
}

bool OBBBoxCollider::Intersects(shared_ptr<BaseCollider>& other)
{
	// 충돌한 물체의 콜라이더 타입을 체크합니다.
	ColliderType type = other->GetColliderType();

	// 타입에 따라 분기합니다.
	switch (type)
	{
	case ColliderType::Sphere:
		return _boundingBox.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());

	case ColliderType::AABB:
		return _boundingBox.Intersects(dynamic_pointer_cast<AABBBoxCollider>(other)->GetBoundingBox());

	case ColliderType::OBB:
		return _boundingBox.Intersects(dynamic_pointer_cast<OBBBoxCollider>(other)->GetBoundingBox());
	}

	return false;
}
