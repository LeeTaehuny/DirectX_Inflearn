#include "pch.h"
#include "09. MeshDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"

void MeshDemo::Init()
{
	/** 카메라 */
	{
		// 카메라를 저장하기 위한 게임 오브젝트를 생성합니다.
		_camera = make_shared<GameObject>();
		// 해당 게임 오브젝트에 Transform 컴포넌트를 추가합니다.
		_camera->GetOrAddTransform();
		// 해당 게임 오브젝트에 카메라 컴포넌트를 추가합니다.
		_camera->AddComponent(make_shared<Camera>());
		// 생성한 카메라의 이동 스크립트(CameraScript)를 추가합니다.
		_camera->AddComponent(make_shared<CameraScript>());
	}

	/** Object */
	{
		// 게임 오브젝트를 생성합니다.
		_obj = make_shared<GameObject>();
		// 해당 게임 오브젝트에 Transform 컴포넌트를 추가합니다.
		_obj->GetOrAddTransform();
		// 해당 게임 오브젝트에 필요한 컴포넌트들을 추가합니다.
		_obj->AddComponent(make_shared<MeshRenderer>());
		// MeshRenderer는 Mesh, Shader, Texture 정보를 가지고 있습니다.
		// * 부품의 추가 정보를 넣어줍니다.
		{
			// 셰이더 정보를 추가합니다.
			shared_ptr<Shader> shader = make_shared<Shader>(L"07. Normal.fx");
			_obj->GetMeshRenderer()->SetShader(shader);
		}
		{
			// 리소스 매니저의 초기화를 수행합니다. (기본 도형 생성)
			RESOURCES->Init();
			
			// Sphere의 기하학적 모형에 대한 정보를 저장합니다.
			shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Sphere");
			_obj->GetMeshRenderer()->SetMesh(mesh);
		}
		{
			// 텍스처 정보를 추가합니다.
			shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
			_obj->GetMeshRenderer()->SetTexture(texture);
		}
	}

}

void MeshDemo::Update()
{
	_camera->Update();
	_obj->Update();
}

void MeshDemo::Render()
{
}
