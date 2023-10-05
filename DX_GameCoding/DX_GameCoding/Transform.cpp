#include "pch.h"
#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update()
{
}

void Transform::UpdateTransform()
{
	// SRT 순서에 맞춰 행렬을 생성합니다.
	// * Scale
	Matrix matScale = Matrix::CreateScale(_localScale);

	// * Rotation 
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);

	// * Translation
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	// 자신의 Transform 정보를 부모 좌표계에서의 Transform 정보로 승격시켜줍니다.
	_matLocal = matScale * matRotation * matTranslation;

	// * 만약 부모가 존재하는 경우라면?
	if (HasParent())
	{
		// SRT 행렬을 적용시킨 _matLocal에 부모의 월드 변환 행렬을 곱해주면 World에서의 좌표가 나옵니다.
		_matWorld = _matLocal * _parent->GetWorldMatrix();
	}
	// * 만약 부모가 존재하지 않는 경우라면?
	else
	{
		// SRT 행렬을 적용시킨 _matLocal 자체가 World에서의 좌표를 의미합니다.
		_matWorld = _matLocal;
	}
}
