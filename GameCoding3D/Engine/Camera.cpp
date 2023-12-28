#include "pch.h"
#include "Camera.h"
#include "Scene.h"

Matrix Camera::S_MatView = Matrix::Identity;
Matrix Camera::S_MatProjection = Matrix::Identity;

Camera::Camera() : Super(ComponentType::Camera)
{
	_width = static_cast<float>(GAME->GetGameDesc().width);
	_height = static_cast<float>(GAME->GetGameDesc().height);
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
	Vec3 eyePosition = GetTransform()->GetPosition();
	Vec3 focusPosition = eyePosition + GetTransform()->GetLook();
	Vec3 upDirection = GetTransform()->GetUp();
	
	//S_MatView;
	_matView = ::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	

	if (_type == ProjectionType::Perspective)
	{
		//S_MatProjection;
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
	}
	else
	{
		//S_MatProjection;
		_matProjection = ::XMMatrixOrthographicLH(_width, _height, _near, _far);
	}
	
}

void Camera::SortGameObject()
{
	// 현재 씬을 가져옵니다.
	shared_ptr<Scene> scene = CUR_SCENE;
	// 현재 씬으로부터 오브젝트들을 가져옵니다.
	unordered_set<shared_ptr<GameObject>>& gameObjects = scene->GetObjects();

	// 해당 카메라와 연관있는 물체들을 초기화합니다.
	_vecForward.clear();

	// 가져온 게임 오브젝트들을 순회합니다.
	for (auto& gameObject : gameObjects)
	{
		// 해당 오브젝트의 layer 번호가 카메라의 비트 플래그에 존재하지 않는다면 무시합니다.
		if (IsCulled(gameObject->GetLayerIndex()))
			continue;

		// 해당 게임 오브젝트가 그리기 위한 컴포넌트(Mesh, Model, Animation)를 하나도 가지지 않는다면 무시합니다.
		if (gameObject->GetMeshRenderer() == nullptr
			&& gameObject->GetModelRenderer() == nullptr
			&& gameObject->GetModelAnimator() == nullptr)
			continue;

		// 현재 카메라와 연관있는 물체 모음에 추가합니다.
		_vecForward.push_back(gameObject);
	}
}

void Camera::Render_Forward()
{
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	INSTANCING->Render(_vecForward);
}
