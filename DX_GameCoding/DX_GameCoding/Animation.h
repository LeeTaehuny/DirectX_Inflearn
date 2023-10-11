#pragma once
#include "ResourceBase.h"

// 키 프레임을 저장하기 위한 구조체를 선언합니다.
struct Keyframe
{
	// 그림을 그릴 위치를 저장하기 위한 변수를 선언합니다.
	Vec2 offset = Vec2{ 0.0f, 0.0f };
	// 그릴 그림의 크기를 저장하기 위한 변수를 선언합니다.
	Vec2 size = Vec2{ 0.0f, 0.0f };
	// 그림을 그릴 시간을 저장하기 위한 변수를 선언합니다.
	float time = 0.0f;
};

class Texture;

class Animation : public ResourceBase
{
	using Super = ResourceBase;
public:
	Animation();
	virtual ~Animation();

	// 저장 및 불러오는 기능의 함수를 선언합니다.
	virtual void Load(const wstring& path) override;
	virtual void Save(const wstring& path) override;

	// 무한 재생 여부를 설정, 반환하기 위한 함수들을 정의합니다.
	void SetLoop(bool loop) { _loop = loop; }
	bool IsLoop() { return _loop; }

	// 텍스처를 설정, 반환하기 위한 함수들을 정의합니다.
	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }
	shared_ptr<Texture> GetTexture() { return _texture; }

	// 텍스처의 크기를 반환하기 위한 함수를 선언합니다.
	Vec2 GetTextureSize();

	// 키프레임 관련 정보들을 반환하기 위한 함수를 선언합니다.
	const Keyframe& GetKeyframe(int32 index);
	int32 GetKeyframeCount();

	// 키프레임에 정보를 추가하기 위한 함수를 선언합니다.
	void AddKeyframe(const Keyframe& keyframe);

private:
	// 무한 재생 여부를 판별하기 위한 변수를 선언합니다.
	bool _loop = false;

	// 텍스처를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Texture> _texture;
	// 키 프레임들을 저장하기 위한 배열을 선언합니다.
	vector<Keyframe> _keyframes;
};

