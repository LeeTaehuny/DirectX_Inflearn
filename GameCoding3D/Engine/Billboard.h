#pragma once
#include "Component.h"

// 정점의 정보를 저장하기 위한 구조체를 선언합니다.
struct VertexBillboard
{
	Vec3 position;
	Vec2 uv;
	Vec2 scale;
};

#define MAX_BILLBOARD_COUNT 500

class Billboard : public Component
{
	using Super = Component;

public:
	Billboard();
	virtual ~Billboard();

	void Update();
	// 특정 위치에 특정 크기에 해당하는 점을 추가하기 위한 함수를 선언합니다.
	void Add(Vec3 position, Vec2 scale);

	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetPass(uint8 pass) { _pass = pass; }

private:
	// 정점과 인덱스들을 저장할 배열을 선언합니다.
	vector<VertexBillboard> _vertices;
	vector<uint32> _indices;

	// 정점, 인덱스들의 정보를 GPU에 넘겨주기 위한 버퍼를 생성합니다.
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	// 그려야 하는 물체의 수를 저장하기 위한 변수를 선언합니다.
	// (물체를 추가할 때마다 리셋하지 않고 바뀌는 순간에만 추적하기 위함)
	// * (갱신) 
	int32 _drawCount = 0;
	// * (원본) 
	int32 _prevCount = 0;

	// Material을 저장하기 위한 변수를 선언합니다.
	shared_ptr<Material> _material;

	uint8 _pass = 0;
};

