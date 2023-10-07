#include "pch.h"
#include "Camera.h"

// 정적 변수는 실체를 구체화해야 합니다.
Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	UpdateMatrix();
}

void Camera::UpdateMatrix()
{
	// S_MatView는 최초 Identity로 설정되어있습니다. ([0, 0, 0] 위치에서 look 벡터로 찍는 중)
	
	// 카메라의 변환 행렬을 구하는 방법 1
	// * XMMatrixLookAtLH() 함수 사용
	//   -> eyePosition		: 카메라의 위치
	//   -> focusPosition	: 카메라가 바라보는 위치
	//   -> upDirection		: 카메라의 위쪽 방향
	{
		// 현재 카메라의 위치
		Vec3 eyePosition = GetTransform()->GetPosition();
		// 현재 카메라가 바라보는 위치
		Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
		// 카메라의 위쪽 방향
		Vec3 upDirection = GetTransform()->GetUp();
		// 카메라의 View Space 행렬
		S_MatView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	}

	// 카메라의 변환 행렬을 구하는 방법 2
	// * 월드 변환 행렬을 이미 구해서 알고 있는 상황이므로 그의 역행렬은 View 변환 행렬입니다.
	//   -> 즉, 월드를 카메라 기준의 x, y, z로 변환하는 것이기 때문에 월드 변환 행렬의 역행렬을 취하면 카메라 기준의 좌표계가 나옵니다.
	{
		//S_MatView = GetTransform()->GetWorldMatrix().Invert();
	}


	// Projection Matrix

	// 만약 타입이 직교 투영이라면?
	if (_type == ProjectionType::Perspective)
	{
		// XMMatrixPerspectiveFovLH() : Perspective타입의 Projection 변환 행렬을 생성해주는 함수
		// * FovAngleY   : (field of view angle y) 카메라가 바라보는 각도(시야각) [XM_PI / 4.f = 45도]
		// * AspectRatio : 화면의 비율
		// * NearZ		 : 절두체의 가까운 면
		// * FarZ        : 절두체의 먼 면
		S_MatProjection = ::XMMatrixPerspectiveFovLH(XM_PI / 4.f, 800.0f / 600.0f, 1.0f, 100.0f);
	}
	// 만약 타입이 원근 투영이라면?
	else
	{
		// XMMatrixOrthographicLH() : Orthographic 타입의 Projection 변환 행렬을 생성해주는 함수
		// * ViewWidth / ViewHeight : 화면의 크기
		// * NearZ		 : 사각형의 가까운 면
		// * FarZ        : 사각형의 먼 면
		S_MatProjection = ::XMMatrixOrthographicLH(8.0f, 6.0f, 0.f, 1.0f);
	}
}
