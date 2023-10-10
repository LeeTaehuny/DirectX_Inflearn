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

private:
	// Device를 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;

	// RenderManager를 friend로 등록합니다.
	friend class RenderManager;

	// Geometry (Mesh)
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;

	// (Material)
	shared_ptr<InputLayout> _inputLayout;
	// VS
	shared_ptr<VertexShader> _vertexShader;
	// PS
	shared_ptr<PixelShader> _pixelShader;
	// SRV
	shared_ptr<Texture> _texture1;

};

