#pragma once

#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform() override;
	
	virtual void Init() override;
	virtual void Update() override;

	// Transform 정보를 부모의 Transform 정보로 갱신하기 위한 함수를 선언합니다.
	void UpdateTransform();

	// Local
	// * Local Scale Getter & Setter
	Vec3 GetLocalScale() { return _localScale; }
	void SetLocalScale(const Vec3& localScale) { _localScale = localScale; UpdateTransform(); }
	// * Local Rotation Getter & Setter
	Vec3 GetLocalRotation() { return _localRotation; }
	void SetLocalRotation(const Vec3& localRotation) { _localRotation = localRotation; UpdateTransform(); }
	// * Local Position Getter & Setter
	Vec3 GetLocalPosition() { return _localPosition; }
	void SetLocalPosition(const Vec3& localPosition) { _localPosition = localPosition; UpdateTransform(); }

	// World
	// * World Scale Getter & Setter
	Vec3 GetScale() { return _scale; }
	void SetScale(const Vec3& worldScale);
	// * World Rotation Getter & Setter
	Vec3 GetRotation() { return _rotation; }
	void SetRotation(const Vec3& worldRotation);
	// * World Position Getter & Setter
	Vec3 GetPosition() { return _position; }
	void SetPosition(const Vec3& worldPosition);

	// 월드 변환 행렬을 반환하기 위한 함수를 정의합니다.
	Matrix GetWorldMatrix() { return _matWorld; }

	// 계층 관계
	// * 부모를 설정하기 위한 함수를 정의합니다.
	void SetParent(shared_ptr<Transform> parent) { _parent = parent; }
	// * 부모의 존재 여부를 판별하기 위한 함수를 정의합니다.
	bool HasParent() { return _parent != nullptr; }
	// * 부모의 Transform 정보를 반환하기 위한 함수를 정의합니다.
	shared_ptr<Transform> GetParent() { return _parent; }

	// * 자식들의 정보를 반환하기 위한 함수를 정의합니다.
	const vector<shared_ptr<Transform>>& GetChildren() { return _children; }
	// * 자식을 추가하기 위한 함수를 정의합니다.
	void AddChild(shared_ptr<Transform> child) { _children.push_back(child); }

private:
	// SRT와 관련된 정보를 저장하기 위한 변수를 선언합니다.
	// * Local
	Vec3 _localScale = { 1.0f, 1.0f, 1.0f };
	Vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
	Vec3 _localPosition = { 0.0f, 0.0f, 0.0f };

	// 부모를 기준으로 하는 좌표계를 저장하기 위한 변수를 선언합니다.
	Matrix _matLocal = Matrix::Identity;
	// 월드를 기준으로 하는 좌표계를 저장하기 위한 변수를 선언합니다.
	Matrix _matWorld = Matrix::Identity;

	// SRT와 관련된 정보를 저장하기 위한 변수를 선언합니다.
	// * World
	Vec3 _scale;
	Vec3 _rotation;
	Vec3 _position;

	// World 기준의 right, up, forward 벡터를 저장하기 위한 변수를 선언합니다.
	Vec3 _right;
	Vec3 _up;
	Vec3 _look;

private:
	// 부모의 Transform 정보를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Transform> _parent;
	// 자식의 Transform 정보들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<Transform>> _children;
};

