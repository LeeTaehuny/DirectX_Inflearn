#pragma once

class MonoBehavior;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~GameObject();

	// 컴포넌트가 가지고 있는 모든 함수들을 여기서 호출해줄 것
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

	// 특정 타입의 고정 컴포넌트를 반환하기 위한 함수를 선언합니다.
	shared_ptr<Component> GetFixedComponent(ComponentType type);
	// Transform 컴포넌트를 반환하기 위한 함수를 선언합니다.
	shared_ptr<Transform> GetTransform();

	// Transform 컴포넌트를 추가하거나 얻어오기 위한 함수를 선언합니다.
	shared_ptr<Transform> GetOrAddTransform();

	// 컴포넌트를 추가하기 위한 함수를 선언합니다.
	void AddComponent(shared_ptr<Component> component);

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

protected:
	// 컴포넌트들을 저장하기 위한 배열을 선언합니다.
	// * array<타입, 크기> : 영역을 초과해 접근하는 것을 막아주는 배열
	// * FIXED_COMPONENT_COUNT (고정 크기의 컴포넌트들은 게임 오브젝트당 1개씩만 가지고 있어야 하기 때문)
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;

	// 물체에 적용될 스크립트들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<MonoBehavior>> _scripts;
};

