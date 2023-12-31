#pragma once
#include "Component.h"

// 정점의 정보를 저장하기 위한 구조체를 선언합니다.
struct VertexSnow
{
	Vec3 position;
	Vec2 uv;
	Vec2 scale;
	Vec2 random;
};

#define MAX_BILLBOARD_COUNT 500

class SnowBillboard : public Component
{
	using Super = Component;

public:
	SnowBillboard(Vec3 extent, int32 drawCount = 100);
	virtual ~SnowBillboard();

	void Update();


	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetPass(uint8 pass) { _pass = pass; }

private:
	// snow 관련 정보를 저장하기 위한 변수를 선언합니다.
	SnowBillboardDesc _desc;
	// 얼마의 시간이 지났는지 시간 정보를 저장하기 위한 변수를 선언합니다.
	float _elpasedTime = 0.0f;

private:
	// 정점과 인덱스들을 저장할 배열을 선언합니다.
	vector<VertexSnow> _vertices;
	vector<uint32> _indices;

	// 정점, 인덱스들의 정보를 GPU에 넘겨주기 위한 버퍼를 생성합니다.
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	// 그려야 하는 물체의 수를 저장하기 위한 변수를 선언합니다.
	int32 _drawCount = 0;

	// Material을 저장하기 위한 변수를 선언합니다.
	shared_ptr<Material> _material;

	uint8 _pass = 0;
};

