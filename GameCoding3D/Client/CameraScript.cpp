#include "pch.h"
#include "CameraScript.h"

void CameraScript::Start()
{
}

void CameraScript::Update()
{
	/** 카메라의 이동 구현 */
	float dt = TIME->GetDeltaTime();

	// 카메라의 위치를 얻어옵니다.
	Vec3 pos = GetTransform()->GetPosition();

	// 카메라의 이동 입력받기
	// * 상하좌우(WSAD)이동
	{
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			pos += GetTransform()->GetLook() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			pos -= GetTransform()->GetLook() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			pos -= GetTransform()->GetRight() * _speed * dt;
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			pos += GetTransform()->GetRight() * _speed * dt;
		}

		// 이동이 종료되면 저장한 Transform 정보를 반영해줍니다.
		GetTransform()->SetPosition(pos);
	}

	// * 카메라 회전(QEZC)
	{
		if (INPUT->GetButton(KEY_TYPE::Q))
		{
			// 기본 Rotation 정보를 받아옵니다.
			Vec3 rotation = GetTransform()->GetLocalRotation();

			// rotation을 회전시킨 후 다시 Transform에 적용시켜줍니다.
			rotation.x += dt * 0.5f;
			GetTransform()->SetLocalRotation(rotation);
		}
		if (INPUT->GetButton(KEY_TYPE::E))
		{
			// 기본 Rotation 정보를 받아옵니다.
			Vec3 rotation = GetTransform()->GetLocalRotation();

			// rotation을 회전시킨 후 다시 Transform에 적용시켜줍니다.
			rotation.x -= dt * 0.5f;
			GetTransform()->SetLocalRotation(rotation);
		}

		if (INPUT->GetButton(KEY_TYPE::Z))
		{
			// 기본 Rotation 정보를 받아옵니다.
			Vec3 rotation = GetTransform()->GetLocalRotation();

			// rotation을 회전시킨 후 다시 Transform에 적용시켜줍니다.
			rotation.y += dt * 0.5f;
			GetTransform()->SetLocalRotation(rotation);
		}
		if (INPUT->GetButton(KEY_TYPE::C))
		{
			// 기본 Rotation 정보를 받아옵니다.
			Vec3 rotation = GetTransform()->GetLocalRotation();

			// rotation을 회전시킨 후 다시 Transform에 적용시켜줍니다.
			rotation.y -= dt * 0.5f;
			GetTransform()->SetLocalRotation(rotation);
		}
	}
}
