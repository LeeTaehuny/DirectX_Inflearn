#pragma once
#include "Component.h"

enum class ProjectionType
{
	Perspective, // 원근 투영
	Orthographic, // 직교 투영
};

class Camera :  public Component
{
	using Super = Component;
public:
	Camera();
	virtual ~Camera();
	
	virtual void Update() override;

	void UpdateMatrix();

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }

private:
	Matrix _matView = Matrix::Identity;
	Matrix _matProjection = Matrix::Identity;

	float _near = 1.f;
	float _far = 1000.f;
	float _fov = XM_PI / 4.f;
	float _width = 0.f;
	float _height = 0.f;

public:
	static Matrix S_MatView;
	static Matrix S_MatProjection;

public:
	// 어떤 물체를 출력할 필요가 있는지를 체크하기 위한 함수를 정의합니다.
	// * 비트 연산 결과를 반환합니다.
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }
	// 모든 플래그를 꺼주는 함수를 정의합니다.
	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }
	// 비트 플레그를 설정하는 함수를 정의합니다.
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }

	// 특정 비트를 키고 끄기 위한 함수를 정의합니다.
	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	// 자신과 관련있는 오브젝트들을 모두 모아오는 함수를 선언합니다.
	void SortGameObject();
	// 렌더링을 위한 함수를 선언합니다.
	void Render_Forward();

public:
	float GetWidth() { return _width; }
	float GetHeight() { return _height; }

	void SetProjectionType(ProjectionType type) { _type = type; }
	ProjectionType GetProjectionType() { return _type; }

private:
	ProjectionType _type = ProjectionType::Perspective;

	// 카메라마다 찍는 물체가 달라져야 합니다.
	// * 번호로 추적하기 위한 변수를 선언합니다. (비트 플래그)
	uint32 _cullingMask = 0;

	// 자신과 관련있는 오브젝트들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<GameObject>> _vecForward;
};