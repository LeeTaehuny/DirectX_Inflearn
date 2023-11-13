#pragma once
// 하나의 실행 단위로 생각합니다.
#include "IExecute.h"

class TriangleDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	// 셰이더를 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<Shader> _shader;
	// 정점 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<VertexData> _vertices;
	// 정점 버퍼 정보를 저장하기 위한 스마트 포인터를 선언합니다.
	shared_ptr<VertexBuffer> _buffer;
};

