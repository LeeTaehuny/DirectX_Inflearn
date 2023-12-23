#include "pch.h"
#include "SphereCollider.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Update()
{
	// 구체의 중심을 갱신합니다. (오브젝트의 중심)
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetPosition();
	// 반지름을 갱신합니다. (Scale값 적용)
	Vec3 scale = GetGameObject()->GetTransform()->GetScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Ray& ray, OUT float& distance)
{
	// 광선이 _boundingSphere와 충돌한다면 true 리턴 및 distance에 거리를 넣어주는 함수입니다.
	return _boundingSphere.Intersects(ray.position, ray.direction, OUT distance);
}

bool SphereCollider::Intersects(shared_ptr<BaseCollider>& other)
{
	// 충돌한 물체의 콜라이더 타입을 체크합니다.
	ColliderType type = other->GetColliderType();

	// 타입에 따라 분기합니다.
	switch (type)
	{
	case ColliderType::Sphere:
		return _boundingSphere.Intersects(dynamic_pointer_cast<SphereCollider>(other)->GetBoundingSphere());

	case ColliderType::AABB:
		return _boundingSphere.Intersects(dynamic_pointer_cast<AABBBoxCollider>(other)->GetBoundingBox());

	case ColliderType::OBB:
		return _boundingSphere.Intersects(dynamic_pointer_cast<OBBBoxCollider>(other)->GetBoundingBox());
	}

	return false;
}
