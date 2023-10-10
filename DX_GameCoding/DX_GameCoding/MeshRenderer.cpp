#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"

MeshRenderer::MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: Super(ComponentType::MeshRenderer), _device(device)
{
	// VertexTextureData 타입의 정보를 사용하는 Geometry 객체를 생성합니다.
	_geometry = make_shared<Geometry<VertexTextureData>>();
	// Vertex Data 생성
	GeometryHelper::CreateRectangle(_geometry);

	// VertexBuffer 객체를 생성합니다.
	_vertexBuffer = make_shared<VertexBuffer>(device);
	// VertexBuffer 생성
	_vertexBuffer->Create(_geometry->GetVertices());

	// IndexBuffer 객체를 생성합니다.
	_indexBuffer = make_shared<IndexBuffer>(device);
	// indexBuffer 생성
	_indexBuffer->Create(_geometry->GetIndices());

	// VertexShader 객체를 생성합니다.
	_vertexShader = make_shared<VertexShader>(device);
	// VertexShader 생성
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	// InputLayout 객체를 생성합니다.
	_inputLayout = make_shared<InputLayout>(device);
	// InputLayout 생성
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

	// PixelShader 객체를 생성합니다.
	_pixelShader = make_shared<PixelShader>(device);
	// PixelShader 생성
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	// Texture 객체를 생성합니다.
	_texture1 = make_shared<Texture>(device);
	// Texture 생성
	_texture1->Create(L"test.png");
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{

}
