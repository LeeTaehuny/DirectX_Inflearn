#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"

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

	// RasterizerState 객체를 생성합니다.
	_rasterizerState = make_shared<RasterizerState>(device);
	// RasterizerState 생성
	_rasterizerState->Create();

	// BlendState 객체를 생성합니다.
	_blendState = make_shared<BlendState>(device);
	// BlendState 생성
	_blendState->Create();

	// TransformData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(device, deviceContext);
	// ConstantBuffer 생성
	_transformBuffer->Create();

	// CameraData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(device, deviceContext);
	// ConstantBuffer 생성
	_cameraBuffer->Create();

	// Texture 객체를 생성합니다.
	_texture1 = make_shared<Texture>(device);
	// Texture 생성
	_texture1->Create(L"test.png");

	// SamplerState 객체를 생성합니다.
	_samplerState = make_shared<SamplerState>(device);
	// SamplerState 생성
	_samplerState->Create();
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Update()
{
	_cameraDate.matView = Camera::S_MatView;
	_cameraDate.matProjection = Camera::S_MatProjection;

	_cameraBuffer->CopyData(_cameraDate);

	_transformData.matWorld = GetTransform()->GetWorldMatrix();
	_transformBuffer->CopyData(_transformData);
}

void MeshRenderer::Render(shared_ptr<Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout = _inputLayout;
	info.vertexShader = _vertexShader;
	info.pixelShader = _pixelShader;
	info.rasterizerState = _rasterizerState;
	info.blendState = _blendState;

	pipeline->UpdatePipeline(info);

	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);

	pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
	pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}
