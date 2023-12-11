#pragma once
#include "IExecute.h"

class InstancingDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader> _shader;
	shared_ptr<GameObject> _camera;
	vector<shared_ptr<GameObject>> _objs;

// Instancing
private:
	// 원본 Mesh를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Mesh> _mesh;
	// 원본 Material을 저장하기 위한 변수를 선언합니다.
	shared_ptr<Material> _material;

	// 물체들의 Transform 정보(W)들을 저장해두기 위한 벡터 컨테이너를 선언합니다.
	vector<Matrix> _worlds;
	// Instancing에 사용될 버퍼를 저장하기 위한 변수를 선언합니다.
	shared_ptr<VertexBuffer> _instanceBuffer;
};

