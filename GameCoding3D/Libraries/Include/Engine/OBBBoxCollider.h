#pragma once
#include "BaseCollider.h"
class OBBBoxCollider : public BaseCollider
{
public:
	OBBBoxCollider();
	virtual ~OBBBoxCollider();

	virtual void Update() override;

	// RayCasting
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	// Other Collider
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	BoundingOrientedBox& GetBoundingBox() { return _boundingBox; }

private:
	// BoundingOrientedBox은 객체의 실제 방향을 따라 회전된 상자를 의미합니다.
	// (Oriented bounding box)
	BoundingOrientedBox _boundingBox;
};

