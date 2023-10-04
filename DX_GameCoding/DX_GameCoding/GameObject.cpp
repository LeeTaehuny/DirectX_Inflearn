#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) : _device(device)
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
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(device, deviceContext);
	// ConstantBuffer 생성
	_constantBuffer->Create();

	// Texture 객체를 생성합니다.
	_texture1 = make_shared<Texture>(device);
	// Texture 생성
	_texture1->Create(L"test.png");

	// SamplerState 객체를 생성합니다.
	_samplerState = make_shared<SamplerState>(device);
	// SamplerState 생성
	_samplerState->Create();
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	// 테스트를 위해 로컬 위치를 이동시켜봅니다.
	_localPosition.x += 0.001f;

	// SRT
	{
		// SRT 순서에 맞춰 행렬을 생성합니다.
		// * Scale
		Matrix matScale = Matrix::CreateScale(_localScale);

		// * Rotation 
		Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
		matRotation *= Matrix::CreateRotationY(_localRotation.y);
		matRotation *= Matrix::CreateRotationZ(_localRotation.z);

		// * Translation
		Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

		// 만들어진 SRT 정보를 조합해 월드 행렬을 만들어줍니다. 
		Matrix matWorld = matScale * matRotation * matTranslation;
		// World 행렬에 넣어줍니다.
		_transformData.matWorld = matWorld;
	}

	_constantBuffer->CopyData(_transformData);
}

void GameObject::Render(shared_ptr<Pipeline> pipeline)
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

	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);

	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}
