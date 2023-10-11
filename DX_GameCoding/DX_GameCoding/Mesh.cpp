#include "pch.h"
#include "Mesh.h"

Mesh::Mesh(ComPtr<ID3D11Device> device) : Super(ResourceType::Mesh), _device(device)
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateDefaultRectangle()
{
	// VertexTextureData 타입의 정보를 사용하는 Geometry 객체를 생성합니다.
	_geometry = make_shared<Geometry<VertexTextureData>>();
	// Vertex Data 생성
	GeometryHelper::CreateRectangle(_geometry);

	// VertexBuffer 객체를 생성합니다.
	_vertexBuffer = make_shared<VertexBuffer>(_device);
	// VertexBuffer 생성
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer 객체를 생성합니다.
	_indexBuffer = make_shared<IndexBuffer>(_device);
	// indexBuffer 생성
	_indexBuffer->Create(_geometry->GetIndices());
}
