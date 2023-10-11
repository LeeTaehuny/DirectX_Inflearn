#pragma once
#include "Component.h"
#include "Material.h"
#include "Shader.h"

class VertexBuffer;
class IndexBuffer;
class InputLayout;
class Mesh;
class Material;
class Shader;

class MeshRenderer : public Component
{
	using Super = Component;

public:
	MeshRenderer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	virtual ~MeshRenderer() override;
	
	// 메시를 설정하고 반환하기 위한 함수들을 정의합니다.
	void SetMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetShader(shared_ptr<Shader> shader) { _material->SetShader(shader); }
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetTexture(shared_ptr<Texture> texture) { _material->SetTexture(texture); }

	shared_ptr<Mesh> GetMesh() { return _mesh; }
	shared_ptr<Material> GetMaterial() { return _material; }
	shared_ptr<Texture> GetTexture() { return GetMaterial()->GetTexture(); }
	shared_ptr<VertexShader> GetVertexShader() { return GetMaterial()->GetShader()->GetVertexShader(); }
	shared_ptr<InputLayout> GetInputLayout() { return GetMaterial()->GetShader()->GetInputLayout(); }
	shared_ptr<PixelShader> GetPixelShader() { return GetMaterial()->GetShader()->GetPixelShader(); }

private:
	// Device를 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;

	// Mesh
	shared_ptr<Mesh> _mesh;

	// Material
	shared_ptr< Material> _material;
};

