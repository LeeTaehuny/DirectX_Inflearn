#include "pch.h"
#include "01. TriangleDemo.h"

void TriangleDemo::Init()
{
	// 셰이더 fx를 생성합니다.
	_shader = make_shared<Shader>(L"01. Triangle.fx");

	// 삼각형 만들기
	{
		// 정점 정보를 추가합니다.
		_vertices.resize(3);

		_vertices[0].position = Vec3{ -0.5f, 0.f, 0.f };
		_vertices[1].position = Vec3{ 0.f, 0.5f, 0.f };
		_vertices[2].position = Vec3{ 0.5f, 0.f, 0.f };
	}

	// 정점 버퍼를 생성합니다.
	_buffer = make_shared<VertexBuffer>();
	_buffer->Create(_vertices);
}

void TriangleDemo::Update()
{

}

void TriangleDemo::Render()
{
	uint32 stride = _buffer->GetStride();
	uint32 offset = _buffer->GetOffset();

	// IA 단계에 VertexBuffer를 연결시켜줍니다.
	DC->IASetVertexBuffers(0, 1, _buffer->GetComPtr().GetAddressOf(), &stride, &offset);

	// 화면에 그려줍니다.
	// * Triangle.fx의 T0의 P0 -> 빨간색
	_shader->Draw(0, 0, 3);
}
