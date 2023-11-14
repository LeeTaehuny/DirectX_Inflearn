#pragma once
// 하나의 실행 단위로 생각합니다.
#include "IExecute.h"
#include "Geometry.h"

class QuadDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	// 셰이더를 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<Shader> _shader;
	// 기하학적인 모형을 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<Geometry<VertexColorData>> _geometry;
	// 정점 버퍼 정보를 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<VertexBuffer> _vertexBuffer;
	// 인덱스 버퍼 정보를 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<IndexBuffer> _indexBuffer;
};

