#include "pch.h"
#include "Transform.h"

Transform::Transform() : Super(ComponentType::Transform)
{

}

Transform::~Transform()
{

}

void Transform::Awake()
{

}

void Transform::Update()
{

}

Vec3 ToEulerAngles(Quaternion q) {
	Vec3 angles;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.y = 2 * std::atan2(sinp, cosp) - 3.14159f / 2;

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
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

	// 월드 변환 행렬이 만들어졌습니다.
	// * 월드 변환 행렬을 사용해 월드 기준의 right, up, look 벡터를 구할 수 있습니다.
	// * TransformNormal() - (x, y, z, 0)으로 연산한 방향을 반환해주는 함수
	// * Transform()       - (x, y, z, 1)으로 연산한 방향에 위치까지 반환해주는 함수
	//_right = Vec3::TransformNormal(Vec3::Right, _matWorld); // (1, 0, 0) * matWorld
	//_up = Vec3::TransformNormal(Vec3::Up, _matWorld);		// (0, 1, 0) * matWorld
	//_look = Vec3::TransformNormal(Vec3::Backward, _matWorld); // 오른손좌표계 라이브러리이므로 앞 뒤 방향 반대

	// * 월드 변환 행렬을 반대로 분리해 월드의 SRT를 얻을 수 있습니다.
	//   -> vector3로 관리하게 된다면 짐벌럭 현상이 발생할 수 있기 때문에 게임에서는 사원수 방식으로 회전을 관리합니다.
	//   -> Quaternion에서 Vector3 형태로 변환시킬 수 있습니다.
	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = ToEulerAngles(quat);

	// 자식 클래스들을 순회하며 해당 클래스의 Transform을 업데이트를 해줍니다.
	for (const shared_ptr<Transform>& child : _children)
	{
		child->UpdateTransform();
	}
}

void Transform::SetScale(const Vec3& worldScale)
{
	// 만약 부모가 존재하는 경우라면?
	if (HasParent())
	{
		// 부모의 크기에 따른 조정을 해줘야 합니다.
		// * 부모의 스케일 정보를 받아옵니다.
		Vec3 parentScale = _parent->GetScale();
		// * 자신에게 적용할 스케일 정보를 받아옵니다.
		Vec3 scale = worldScale;
		// * 부모와 비율을 맞춰줍니다.
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;

		// 적용한 값을 실제 자신의 스케일에 적용시켜줍니다.
		SetLocalScale(scale);
	}
	else
	{
		// 크기 자체를 자신에게 적용시킵니다.
		SetLocalScale(worldScale);
	}
}

void Transform::SetRotation(const Vec3& worldRotation)
{
	// 만약 부모가 존재하는 경우라면?
	if (HasParent())
	{
		// 부모의 로컬에서 월드로 가는 행렬의 역행렬을 구해줍니다.
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 rotation;
		rotation.TransformNormal(worldRotation, worldToParentLocalMatrix);
		SetLocalRotation(rotation);
	}
	else
	{
		// 회전값 자체를 자신에게 적용시킵니다.
		SetLocalRotation(worldRotation);
	}
}

void Transform::SetPosition(const Vec3& worldPosition)
{
	// 만약 부모가 존재하는 경우라면?
	if (HasParent())
	{
		// 부모의 로컬에서 월드로 가는 행렬의 역행렬을 구해줍니다.
		Matrix worldToParentLocalMatrix = _parent->GetWorldMatrix().Invert();

		Vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);
		SetLocalPosition(position);
	}
	else
	{
		// 위치값 자체를 자신에게 적용시킵니다.
		SetLocalPosition(worldPosition);
	}
}
