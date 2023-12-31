#include "pch.h"
#include "Billboard.h"
#include "Material.h"
#include "Camera.h"

Billboard::Billboard() : Super(ComponentType::Billboard)
{
	// 최대 정점의 개수와 인덱스의 개수를 지정합니다.
	// * 정점의 수	= 최대 사각형의 수 * 4
	// * 인덱스의 수 = 최대 사각형의 수 * 6
	int32 vertexCount = MAX_BILLBOARD_COUNT * 4;
	int32 indexCount = MAX_BILLBOARD_COUNT * 6;

	// vertex 설정
	{
		// 최대 수에 맞춰서 초기화해줍니다.
		_vertices.resize(vertexCount);
		// 버퍼를 생성합니다.
		_vertexBuffer = make_shared<VertexBuffer>();
		// 버퍼에서 정점 정보를 생성합니다. (갱신 가능하도록)
		_vertexBuffer->Create(_vertices, 0, true);
	}

	// index 설정
	{
		_indices.resize(indexCount);

		// 인덱스 배열 채워주기
		for (int32 i = 0; i < MAX_BILLBOARD_COUNT; i++)
		{
			// 물체(i)별로 6개 존재. 정점은 4개 존재(순서 : 012 213)
			_indices[i * 6 + 0] = i * 4 + 0;
			_indices[i * 6 + 1] = i * 4 + 1;
			_indices[i * 6 + 2] = i * 4 + 2;
			_indices[i * 6 + 3] = i * 4 + 2;
			_indices[i * 6 + 4] = i * 4 + 1;
			_indices[i * 6 + 5] = i * 4 + 3;
		}

		_indexBuffer = make_shared<IndexBuffer>();
		_indexBuffer->Create(_indices);
	}
}

Billboard::~Billboard()
{
}

void Billboard::Update()
{
	// 이전 업데이트 정보와 현재 업데이트 정보가 달라졌다면? (정점 정보)
	if (_drawCount != _prevCount)
	{
		_prevCount = _drawCount;

		// 정점 정보를 다시 버퍼에 할당해줍니다.
		D3D11_MAPPED_SUBRESOURCE subResource;
		DC->Map(_vertexBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
		{
			memcpy(subResource.pData, _vertices.data(), sizeof(VertexBillboard) * _vertices.size());
		}
		DC->Unmap(_vertexBuffer->GetComPtr().Get(), 0);
	}

	auto shader = _material->GetShader();

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	shader->PushTransformData(TransformDesc{ world });

	// GlobalData
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	_material->Update();

	// IA
	_vertexBuffer->PushData();
	_indexBuffer->PushData();

	// 인덱스 기반으로 그려주기
	shader->DrawIndexed(0, _pass, _drawCount * 6);
}

void Billboard::Add(Vec3 position, Vec2 scale)
{
	// 특정 위치에 해당하는 정점들에 값을 설정해줍니다.
	// * 정점의 위치를 동일하게 설정하여 렌더링 되지 않도록 설정했습니다.
	//   -> 즉, 셰이더에서 position을 이동시켜 원하는 자리에 찍어주도록 해야 합니다.
	_vertices[_drawCount * 4 + 0].position = position;
	_vertices[_drawCount * 4 + 1].position = position;
	_vertices[_drawCount * 4 + 2].position = position;
	_vertices[_drawCount * 4 + 3].position = position;

	_vertices[_drawCount * 4 + 0].uv = Vec2(0, 1);
	_vertices[_drawCount * 4 + 1].uv = Vec2(0, 0);
	_vertices[_drawCount * 4 + 2].uv = Vec2(1, 1);
	_vertices[_drawCount * 4 + 3].uv = Vec2(1, 0);

	_vertices[_drawCount * 4 + 0].scale = scale;
	_vertices[_drawCount * 4 + 1].scale = scale;
	_vertices[_drawCount * 4 + 2].scale = scale;
	_vertices[_drawCount * 4 + 3].scale = scale;

	_drawCount++;
}
