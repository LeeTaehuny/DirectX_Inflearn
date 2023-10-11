#pragma once
#include "Component.h"
#include "Animation.h"

class Animator : public Component
{
	using Super = Component;
public:
	Animator();
	virtual ~Animator();

	void Init();
	void Update();

	// 현재 애니메이션과 키 프레임을 반환하기 위한 함수를 선언합니다.
	shared_ptr<Animation> GetCurrentAnimation();
	const Keyframe& GetCurrnetKeyframe();

	// 실행할 애니메이션을 설정하기 위한 함수를 정의합니다.
	void SetAnimation(shared_ptr<Animation> animation) { _currentAnimation = animation; }

private:
	// 누적 시간을 저장하기 위한 변수를 선언합니다.
	float _sumTime = 0.0f;
	// 현재 재생중인 프레임 번호를 저장하기 위한 변수를 선언합니다.
	int32 _CurrentKeyframeIndex = 0;

	// 애니메이션을 저장하기 위한 변수를 선언합니다.
	shared_ptr<Animation> _currentAnimation;
};

