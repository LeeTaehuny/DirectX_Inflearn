#pragma once

struct ModelKeyframeData
{
	float time;
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

struct ModelKeyframe
{
	wstring boneName;
	vector<ModelKeyframeData> transforms;
};

// 하나의 애니메이션을 저장하기 위한 구조체를 생성합니다.
struct ModelAnimation
{
	// bone 이름으로 해당 모델의 키 프레임을 찾기 위한 함수를 선언합니다.
	shared_ptr<ModelKeyframe> GetKeyframe(const wstring& name);

	// 애니메이션 이름
	wstring name;
	// 재생 시간
	float duration = 0.0f;
	// 프레임 재생율 (ex. 30 -> 1 / 30초마다 다음 프레임으로 넘기기)
	float frameRate = 0.0f;
	// 프레임 숫자
	uint32 frameCount = 0;

	unordered_map<wstring, shared_ptr<ModelKeyframe>> keyframes;
};

