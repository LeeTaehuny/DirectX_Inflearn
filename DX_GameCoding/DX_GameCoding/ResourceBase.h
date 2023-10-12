#pragma once

// 리소스의 타입을 구분하기 위한 열거형을 선언합니다.
enum class ResourceType : uint8
{
	None = -1,	// 타입 구분 아직 X

	Mesh,		// 기하학적 모양
	Shader,		// 셰이더
	Texture,	// 텍스처
	Material,	
	Animation,

	End,
};

// 리소스 타입들의 수를 표현하기 위한 열거형을 선언합니다.
enum
{
	RESOURCE_TYPE_COUNT = static_cast<uint8>(ResourceType::End),
};

class ResourceBase : public enable_shared_from_this<ResourceBase>
{
public:
	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	// 리소스 타입을 반환하기 위한 함수를 정의합니다.
	ResourceType GetType() { return _type; }
	// 리소스의 이름을 저장하기 위한 함수를 정의합니다.
	void SetName(const wstring& name) { _name = name; }
	// 리소스의 이름을 반환하기 위한 함수를 정의합니다.
	wstring GetName() { return _name; }
	// 리소스의 고유 번호를 반환하기 위한 함수를 정의합니다.
	uint32 GetID() { return _id; }

	// 리소스의 고유 이름을 설정하기 위한 함수를 정의합니다.
	void SetRName(wstring name) { _name = name; }

protected:
	// 리소스를 파일로부터 불러오고 저장하기 위한 함수를 선언합니다.
	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

private:
	friend class Animation;
	// 리소스의 타입 구분을 위한 열거형 타입의 변수를 선언합니다.
	ResourceType _type = ResourceType::None;

	// 리소스의 이름을 저장하기 위한 변수를 선언합니다.
	wstring _name;
	// 리소스의 이름을 저장하기 위한 변수를 선언합니다.
	wstring _path;
	// 리소스의 아이디를 저장하기 위한 변수를 선언합니다.
	uint32 _id = 0;
};

