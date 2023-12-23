#pragma once
#include "BaseCollider.h"

class AABBBoxCollider : public BaseCollider
{
public:
	AABBBoxCollider();
	virtual ~AABBBoxCollider();

	virtual void Update() override;

	// RayCasting
	virtual bool Intersects(Ray& ray, OUT float& distance) override;
	// Other Collider
	virtual bool Intersects(shared_ptr<BaseCollider>& other) override;

	BoundingBox& GetBoundingBox() { return _boundingBox; }

private:
	// BoundingBox은 축과 정렬되어있는 박스를 의미합니다.
	// (Axis-aligned bounding box)
	BoundingBox _boundingBox;
};

