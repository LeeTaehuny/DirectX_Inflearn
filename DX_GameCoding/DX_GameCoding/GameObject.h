#pragma once

class MonoBehavior;
class Transform;
class Camera;
class MeshRenderer;
class Component;

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
	// Camera 컴포넌트를 반환하기 위한 함수를 선언합니다.
	shared_ptr<Camera> GetCamera();
	// MeshRenderer 컴포넌트를 반환하기 위한 함수를 선언합니다.
	shared_ptr<MeshRenderer> GetMeshRenderer();

	// Transform 컴포넌트를 추가하거나 얻어오기 위한 함수를 선언합니다.
	shared_ptr<Transform> GetOrAddTransform();

	// 컴포넌트를 추가하기 위한 함수를 선언합니다.
	void AddComponent(shared_ptr<Component> component);

private:
	// Device를 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;

protected:
	// 컴포넌트들을 저장하기 위한 배열을 선언합니다.
	// * array<타입, 크기> : 영역을 초과해 접근하는 것을 막아주는 배열
	// * FIXED_COMPONENT_COUNT (고정 크기의 컴포넌트들은 게임 오브젝트당 1개씩만 가지고 있어야 하기 때문)
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;

	// 물체에 적용될 스크립트들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<MonoBehavior>> _scripts;
};

