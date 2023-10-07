#pragma once
#include "Component.h"

// 카메라의 타입을 구분하기 위한 열거형을 선언합니다.
enum class ProjectionType
{
	Perspective,	// 원근 투영
	Orthograpic,	// 직교 투영
};

class Camera : public Component
{
	using Super = Component;

public:
	Camera();
	virtual ~Camera() override;

	// 매 프레임 호출되는 함수
	virtual void Update();

	// 카메라의 타입을 설정하기 위한 함수를 정의합니다.
	void SetProjectionType(ProjectionType type) { _type = type; }
	// 카메라의 타입을 반환하기 위함 함수를 정의합니다.
	ProjectionType GetProjectionType() { return _type; }

	// Matrix 정보를 업데이트하기 위한 함수를 선언합니다.
	// * View, Projection 행렬을 구해주는 것이 목표
	void UpdateMatrix();

private:
	// 카메라의 타입을 저장하기 위한 변수를 선언합니다.
	ProjectionType _type = ProjectionType::Orthograpic;

public:
	// 결국 카메라는 View / Projection Martix와 연관이 있습니다.
	// 지금은 카메라를 1개만 만들어 줄 것이므로 어디서든 접근 가능하게 정적 변수로 선언합니다.
	static Matrix S_MatView;
	static Matrix S_MatProjection;
};

