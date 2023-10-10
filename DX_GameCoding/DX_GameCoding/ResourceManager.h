#pragma once

#include "ResourceBase.h"

class Mesh;
class Material;
class Shader;
class Animation;
class Texture;

class ResourceManager
{
public:
	ResourceManager(ComPtr<ID3D11Device> device);

	void Init();

	// 리소스를 불러오기 위한 함수를 선언합니다.
	// * 아직은 Load, Save 기능이 없으므로 구현은 나중에 하도록 하겠습니다.
	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	// 리소스를 추가하기 위한 함수를 선언합니다.
	// * 리소스의 타입이 다를 수 있으므로 템플릿 함수로 선언합니다.
	template<typename T>
	bool Add(const wstring& key, shared_ptr<T> obj);

	// 리소스를 반환하기 위한 함수를 선언합니다.
	template<typename T>
	shared_ptr<T> Get(const wstring& key);

	// 키에 해당하는 리소스의 타입을 결정하기 위한 헬퍼 함수를 선언합니다.
	template<typename T>
	ResourceType GetResourceType();

private:
	// 기본적으로 테스트를 위한 생성 함수들을 선언합니다.
	void CreateDefaultTexture();
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateDefaultAnimation();

private:
	ComPtr<ID3D11Device> _device;

	using KeyObjMap = map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
inline shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	// 리소스의 타입을 받아옵니다.
	auto objectType = GetResourceType<T>();
	// 리소스의 타입에 맞는 리소스 목록을 가져옵니다.
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	// 키 값을 토대로 리소스 목록에서 탐색합니다.
	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
	{
		// 찾은 경우이므로 해당 리소스를 반환합니다.
		return static_pointer_cast<T>(findIt->second);
	}

	// 찾지 못한 경우입니다.
	// * 새로운 리소스를 생성합니다.
	shared_ptr<T> object = make_shared<T>();
	// * 경로에 존재하는 리소스를 저장합니다.
	object->Load(path);
	// * 키 값을 토대로 오브젝트를 저장합니다.
	keyObjMap[key] = object;

	// 생성한 오브젝트를 반환합니다.
	return object;
}

template<typename T>
inline bool ResourceManager::Add(const wstring& key, shared_ptr<T> obj)
{
	// 리소스 타입을 체크합니다.
	ResourceType resourceType = GetResourceType<T>();

	// 해당 리소스 타입을 가진 맵 컨테이너를 받아옵니다.
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// 해당 맵에서 key를 찾아옵니다.
	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
	{
		return false;
	}
	else
	{
		keyObjMap[key] = obj;

		return true;
	}
}

template<typename T>
inline shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	// 리소스 타입을 체크합니다.
	ResourceType resourceType = GetResourceType<T>();

	// 해당 리소스 타입을 가진 맵 컨테이너를 받아옵니다.
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(resourceType)];

	// 해당 맵에서 key를 찾아옵니다.
	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
	{
		return static_pointer_cast<T>(findIt->second);
	}
	else
	{
		return nullptr;
	}
}

template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	// 입력으로 들어온 타입과 Mesh가 같은 타입인지 체크합니다.
	if (std::is_same_v<T, Mesh>)
	{
		return ResourceType::Mesh;
	}

	// 입력으로 들어온 타입과 Texture가 같은 타입인지 체크합니다.
	if (std::is_same_v<T, Texture>)
	{
		return ResourceType::Texture;
	}

	// 입력으로 들어온 타입과 Shader가 같은 타입인지 체크합니다.
	if (std::is_same_v<T, Shader>)
	{
		return ResourceType::Shader;
	}

	// 입력으로 들어온 타입과 Material이 같은 타입인지 체크합니다.
	if (std::is_same_v<T, Material>)
	{
		return ResourceType::Material;
	}

	// 입력으로 들어온 타입과 Animation이 같은 타입인지 체크합니다.
	if (std::is_same_v<T, Animation>)
	{
		return ResourceType::Animation;
	}

	assert(false);
	return ResourceType::None;
}
