#include "pch.h"
#include "03. ConstBufferDemo.h"
#include "GeometryHelper.h"

void ConstBufferDemo::Init()
{
	// 셰이더 fx를 생성합니다.
	_shader = make_shared<Shader>(L"03. ConstBuffer.fx");
	// 기하학 정보를 생성합니다.
	_geometry = make_shared<Geometry<VertexColorData>>();

	// 사각형 만들기
	{
		// GeometryHelper에 추가한 함수를 통해 사각형의 기하학적인 정보를 세팅합니다.
		GeometryHelper::CreateQuad(_geometry, Color(1.0f, 0.0f, 0.0f, 1.0f));
	}

	// 정점 버퍼를 생성합니다.
	_vertexBuffer = make_shared<VertexBuffer>();
	_vertexBuffer->Create(_geometry->GetVertices());

	// 인덱스 버퍼를 생성합니다.
	_indexBuffer = make_shared<IndexBuffer>();
	_indexBuffer->Create(_geometry->GetIndices());
}

void ConstBufferDemo::Update()
{
	float dt = TIME->GetDeltaTime();

	// WASD 입력을 통한 이동 구현
	if (INPUT->GetButton(KEY_TYPE::A))
	{
		_translation.x -= 3.0f * dt;
	}
	else if (INPUT->GetButton(KEY_TYPE::D))
	{
		_translation.x += 3.0f * dt;
	}
	else if (INPUT->GetButton(KEY_TYPE::W))
	{
		_translation.y += 3.0f * dt;
	}
	else if (INPUT->GetButton(KEY_TYPE::S))
	{
		_translation.y -= 3.0f * dt;
	}

	// 월드 행렬 구하기
	// * 원래는 SRT를 전부 넣어줘야 하지만 지금은 Translation에 대한 정보만 사용
	_world = Matrix::CreateTranslation(_translation);
}

void ConstBufferDemo::Render()
{
	// 구한 월드 행렬들을 Shader의 행렬에 적용합니다.
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&_view);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&_projection);

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// IA 단계에 VertexBuffer를 연결시켜줍니다.
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	// IA 단계에 IndexBuffer를 연결시켜줍니다.
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 화면에 그려줍니다.
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
