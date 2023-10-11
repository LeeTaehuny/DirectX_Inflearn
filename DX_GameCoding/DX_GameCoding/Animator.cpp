#include "pch.h"
#include "Animator.h"
#include "Game.h"
#include "TimeManager.h"

Animator::Animator() : Super(ComponentType::Animator)
{
}

Animator::~Animator()
{
}

void Animator::Init()
{
}

void Animator::Update()
{
	// 현재 실행할 애니메이션을 받아옵니다.
	shared_ptr<Animation> animation = GetCurrentAnimation();

	// 만약 애니메이션이 존재하지 않는다면?
	if (animation == nullptr)
	{
		// 종료합니다.
		return;
	}

	// 현재 애니메이션의 키 프레임을 가져옵니다.
	const Keyframe& keyframe = animation->GetKeyframe(_CurrentKeyframeIndex);

	// 시간을 누적시켜줍니다.
	float deltaTime = TIME->GetDeltaTime();
	_sumTime += deltaTime;

	// 만약 누적 시간이 키프레임의 시간보다 커진다면?
	if (_sumTime >= keyframe.time)
	{
		// 누적 시간을 초기화합니다.
		_sumTime = 0.0f;

		// 프레임 인덱스를 1 증가시킵니다.
		_CurrentKeyframeIndex++;
		// 최종 프레임의 수를 받아옵니다.
		int32 totalCount = animation->GetKeyframeCount();

		// 만약 현재 프레임 인덱스가 최종 프레임 수 보다 커진다면?
		if (_CurrentKeyframeIndex >= totalCount)
		{
			// 만약 무한 반복 애니메이션이라면?
			if (animation->IsLoop())
			{
				// 인덱스를 초기화시켜줍니다.
				_CurrentKeyframeIndex = 0;
			}
			else
			{
				_CurrentKeyframeIndex = totalCount - 1;
			}
		}
	}
}

shared_ptr<Animation> Animator::GetCurrentAnimation()
{
	return  _currentAnimation;
}

const Keyframe& Animator::GetCurrnetKeyframe()
{
	return _currentAnimation->GetKeyframe(_CurrentKeyframeIndex);
}
