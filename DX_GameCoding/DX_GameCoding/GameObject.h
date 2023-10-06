#pragma once
class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	void Update();
	void Render(shared_ptr<Pipeline> pipeline);

private:
	// Device를 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;

	// Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
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
	// 위치, 회전, 크기를 가지는 구조체 타입의 변수를 선언합니다.
	TransformData _transformData;
	// constantBuffer
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	// Transform 컴포넌트를 추가합니다.
	shared_ptr<Transform> _transform = make_shared<Transform>();

	//test
	shared_ptr<Transform> _parnet = make_shared<Transform>();
};

