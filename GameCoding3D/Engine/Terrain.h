#pragma once
#include "Component.h"
class Terrain : public Component
{
	using Super = Component;
public:
	Terrain();
	virtual ~Terrain();

	// Grid를 생성하기 위한 함수를 선언합니다.
	void Create(int32 sizeX, int32 sizeZ, shared_ptr<Material> material);

	// 선택되었는지 여부를 판별하기 위한 함수를 선언합니다. (RayCasting)
	bool Pick(int32 screenX, int32 screenY, Vec3& pickPos, float& distance);

// Getter
public:
	int32 GetSizeX() { return _sizeX; }
	int32 GetSizeZ() { return _sizeZ; }

private:
	shared_ptr<Mesh> _mesh;
	int32 _sizeX = 0;
	int32 _sizeZ = 0;
};

