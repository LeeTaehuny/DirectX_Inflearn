#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "Component.h"
#include "Animator.h"

GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) : _device(device)
{
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

shared_ptr<Camera> GameObject::GetCamera()
{
	// Camera 타입의 컴포넌트를 받아옵니다.
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Camera);

	// 받아온 컴포넌트를 Camera 타입으로 형변환하여 반환합니다.
	return static_pointer_cast<Camera>(component);
}

shared_ptr<MeshRenderer> GameObject::GetMeshRenderer()
{
	// MeshRenderer 타입의 컴포넌트를 받아옵니다.
	shared_ptr<Component> component = GetFixedComponent(ComponentType::MeshRenderer);

	// 받아온 컴포넌트를 MeshRenderer 타입으로 형변환하여 반환합니다.
	return static_pointer_cast<MeshRenderer>(component);
}

shared_ptr<Animator> GameObject::GetAnimator()
{
	// Animator 타입의 컴포넌트를 받아옵니다.
	shared_ptr<Component> component = GetFixedComponent(ComponentType::Animator);

	// 받아온 컴포넌트를 Animator 타입으로 형변환하여 반환합니다.
	return static_pointer_cast<Animator>(component);
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