#pragma once
// 하나의 실행 단위로 생각합니다.
#include "IExecute.h"
#include "Geometry.h"

class ConstBufferDemo : public IExecute
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

	// Transform 중 위치에 관한 정보만 적용해보도록 하겠습니다.
	// * 위치 정보를 저장하기 위한 변수를 선언합니다.
	Vec3 _translation = Vec3(0.0f, 0.0f, 0.0f);

	// 월드, 뷰, 프로젝션 행렬을 저장하기 위한 변수를 선언합니다.
	Matrix _world = Matrix::Identity;
	Matrix _view = Matrix::Identity;
	Matrix _projection = Matrix::Identity;
};

