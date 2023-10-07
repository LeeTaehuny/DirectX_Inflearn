#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"

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

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Awake();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Start();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->Update();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Update();
	}

	_transformData.matWorld = GetOrAddTransform()->GetWorldMatrix();
	_constantBuffer->CopyData(_transformData);
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->LateUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		if (component)
			component->FixedUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->FixedUpdate();
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	// 타입을 받아옵니다.
	uint8 index = static_cast<uint8>(type);

	// 예외처리 (이중 체크)
	assert(index < FIXED_COMPONENT_COUNT);

	// 타입에 따른 컴포넌트를 반환합니다.
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	// Transform 타입의 컴포넌트를 받아옵니다.
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Transform);

	// 받아온 컴포넌트를 Transform 타입으로 형변환하여 반환합니다.
	return static_pointer_cast<Transform>(component);
}

shared_ptr<Transform> GameObject::GetOrAddTransform()
{
	// 만약 Transform 컴포넌트가 존재하지 않는다면?
	if (GetTransform() == nullptr)
	{
		// Transform 컴포넌트를 생성하여 추가합니다.
		shared_ptr<Transform> transform = make_shared<Transform>();
		AddComponent(transform);
	}

	// 여기까지 오면 무조건 Transform 컴포넌트가 추가된 상황이므로 트랜스폼 컴포넌트를 반환해줍니다.
	return GetTransform();
}

void GameObject::AddComponent(shared_ptr<Component> component)
{
	// 컴포넌트를 게임오브젝트와 연결해줍니다. (컴포넌트의 주인 설정)
	// * shared_from_this() : 자신이 들고 있는 스마트 포인터로 변환하는 작업을 해주는 함수 (생성자에서 사용하면 Crash 발생)
	// * shared_from_this를 사용하지 않고 this로 넘겨주게 된다면?
	// * Component에서도 GameObject를 참조하고, GameObject에서도 Component를 참조하기 때문에 이중으로 레퍼런스 카운트를 관리하게 됨
	//   -> 메모리 누수 발생 (메모리 오염)
	component->SetGameObject(shared_from_this());

	// 컴포넌트 타입에 따른 인덱스를 계산합니다.
	uint8 index = static_cast<uint8>(component->GetType());
	// 만약 고정적인 컴포넌트 타입이라면?
	if (index < FIXED_COMPONENT_COUNT)
	{
		_components[index] = component;
	}
	else
	// 유동적인 컴포넌트 타입이라면? (현재는 script밖에 없음)
	{
		_scripts.push_back(static_pointer_cast<MonoBehavior>(component));
	}
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
