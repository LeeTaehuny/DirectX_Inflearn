#pragma once
#include "Component.h"

class VertexBuffer;
class IndexBuffer;
class InputLayout;

class MeshRenderer : public Component
{
	using Super = Component;

public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer() override;
	
	virtual void Update() override;
	
	void Render(shared_ptr<Pipeline> pipeline);

private:
	// Device를 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;

	// Geometry (Mesh)
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	// (Material)
	shared_ptr<InputLayout> _inputLayout;
	// VS
	shared_ptr<VertexShader> _vertexShader;
	// RS
	shared_ptr<RasterizerState> _rasterizerState;
	// PS
	shared_ptr<PixelShader> _pixelShader;
	// SRV
	shared_ptr<Texture> _texture1;


	// Sampler
	shared_ptr<SamplerState> _samplerState;
	// Blend
	shared_ptr<BlendState> _blendState;

private:
	// Camera
	CameraData _cameraDate;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	// SRT
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;
};

