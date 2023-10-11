#pragma once
#include "ResourceBase.h"
class Mesh : public ResourceBase
{
	using Super = ResourceBase;
public:
	Mesh(ComPtr<ID3D11Device> device);
	virtual ~Mesh();

	// 기본 도형을 만들기 위한 함수를 선언합니다.
	void CreateDefaultRectangle();

	// VertexBuffer, IndexBuffer를 반환하기 위한 함수를 정의합니다.
	shared_ptr<VertexBuffer> GetVertexBuffer() { return _vertexBuffer; }
	shared_ptr<IndexBuffer> GetIndexBuffer() { return _indexBuffer; }

private:
	ComPtr<ID3D11Device> _device;

	// Geometry (Mesh)
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
};

