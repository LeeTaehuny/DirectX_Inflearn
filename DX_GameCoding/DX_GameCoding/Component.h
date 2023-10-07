#pragma once

class GameObject;
class Transform;

// 컴포넌트의 종류에 따른 타입을 구분하기 위한 열거형을 선언합니다.
enum class ComponentType : uint8
{
	// 고정적인 컴포넌트 타입 (오브젝트마다 최대 1개)
	Transform,	// 위치 관리
	MeshRender, // 렌더링
	Camera,		// 카메라
	Animator,	// 애니메이션

	// 유동적인 컴포넌트 타입 (오브젝트마다 여러개가 들어갈 수 있는 타입)
	Script,		// 대본

	End,		// 컴포넌트 타입의 수를 계산하기 위함 (현재 5)
};

enum
{
	// 고정적인 컴포넌트 타입의 수를 저장합니다.
	FIXED_COMPONENT_COUNT = static_cast<uint8>(ComponentType::End) - 1,
};

class Component
{
public:
	Component(ComponentType type);
	virtual ~Component();

	// 인게임 시작 전에 호출되는 함수
	virtual void Awake() {}
	// 인게임 시작 시 호출되는 함수
	virtual void Start() {}

	// 매 프레임 호출되는 함수
	virtual void Update() {}
	// Update보다 나중에 실행되는 Update 함수
	virtual void LateUpdate() {}
	// 고정된 시간마다 호출되는 함수
	virtual void FixedUpdate() {}

public:
	// 컴포넌트와 연결된 주체를 반환하는 함수를 선언합니다.
	shared_ptr<GameObject> GetGameObject(); 
	// 컴포넌트와 연결된 주체의 현재 Transform 정보를 반환하기 위한 함수를 선언합니다.
	shared_ptr<Transform> GetTransform();
	// 컴포넌트의 타입을 반환하기 위한 함수를 정의합니다.
	ComponentType GetType() { return _type; }

private:
	friend class GameObject;
	// 컴포넌트에 주체를 설정하기 위한 함수를 정의합니다.
	void SetGameObject(shared_ptr<GameObject> gameObject) { _gameObject = gameObject; }

protected:
	// 컴포넌트의 타입을 저장하기 위한 변수를 선언합니다.
	ComponentType _type;
	// 컴포넌트와 연결된 주체를 저장하기 위한 변수를 선언합니다. (순환 참조를 방지하기 위한 weak_ptr)
	weak_ptr<GameObject> _gameObject;
};

