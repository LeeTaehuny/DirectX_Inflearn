#include "pch.h"
#include "SnowBillboard.h"
#include "Material.h"
#include "Camera.h"
#include "MathUtils.h"

SnowBillboard::SnowBillboard(Vec3 extent, int32 drawCount) : Super(ComponentType::SnowBillboard)
{
	// 범위와 거리, 그려야할 숫자를 저장합니다. 
	_desc.extent = extent;
	_desc.drawDistance = _desc.extent.z * 2.0f;
	_drawCount = drawCount;

	// 정점의 수는 그려야 하는 수 * 4개입니다.
	// * 정점의 수에 따라 벡터 컨테이너를 초기화해줍니다.
	const int32 vertexCount = _drawCount * 4;
	_vertices.resize(vertexCount);

	// 정점의 숫자만큼 순회하며 정보를 채워줍니다.
	for (int32 i = 0; i < _drawCount * 4; i += 4)
	{
		Vec2 scale = MathUtils::RandomVec2(0.1f, 0.5f);

		Vec3 position;
		position.x = MathUtils::Random(-_desc.extent.x, _desc.extent.x);
		position.y = MathUtils::Random(-_desc.extent.y, _desc.extent.y);
		position.z = MathUtils::Random(-_desc.extent.z, _desc.extent.z);

		Vec2 random = MathUtils::RandomVec2(0.0f, 1.0f);

		_vertices[i + 0].position = position;
		_vertices[i + 1].position = position;
		_vertices[i + 2].position = position;
		_vertices[i + 3].position = position;

		_vertices[i + 0].uv = Vec2(0, 1);
		_vertices[i + 1].uv = Vec2(0, 0);
		_vertices[i + 2].uv = Vec2(1, 1);
		_vertices[i + 3].uv = Vec2(1, 0);

		_vertices[i + 0].scale = scale;
		_vertices[i + 1].scale = scale;
		_vertices[i + 2].scale = scale;
		_vertices[i + 3].scale = scale;

		_vertices[i + 0].random = random;
		_vertices[i + 1].random = random;
		_vertices[i + 2].random = random;
		_vertices[i + 3].random = random;
	}

	// 위에서 만든 정점 정보들을 토대로 버퍼를 생성해줍니다.
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_vertices, 0);

	// 인덱스는 그려야 하는 숫자 * 6의 크기를 가집니다.
	// * 인덱스의 수에 따라 벡터 컨테이너를 초기화해줍니다.
	const int32 indexCount = _drawCount * 6;
	_indices.resize(indexCount);

	// 인덱스를 순회하며 값을 할당합니다.
	for (int32 i = 0; i < _drawCount; i++)
	{
		_indices[i * 6 + 0] = i * 4 + 0;
		_indices[i * 6 + 1] = i * 4 + 1;
		_indices[i * 6 + 2] = i * 4 + 2;
		_indices[i * 6 + 3] = i * 4 + 2;
		_indices[i * 6 + 4] = i * 4 + 1;
		_indices[i * 6 + 5] = i * 4 + 3;
	}

	// 위에서 만든 인덱스 정보들을 토대로 버퍼를 생성합니다.
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_indices);
}

SnowBillboard::~SnowBillboard()
{
}

void SnowBillboard::Update()
{
	// snow 정보들을 채워줍니다.
	_desc.origin = CUR_SCENE->GetMainCamera()->GetTransform()->GetPosition();
	_desc.time = _elpasedTime;
	_elpasedTime += DT;

	auto shader = _material->GetShader();

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	shader->PushTransformData(TransformDesc{ world });

	// GlobalData
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// SnowData를 셰이더에 전달해줍니다.
	shader->PushSnowData(_desc);

	// Light
	_material->Update();

	// IA
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	// 인덱스 기반으로 그려주기
	shader->DrawIndexed(0, _pass, _drawCount * 6);
}

