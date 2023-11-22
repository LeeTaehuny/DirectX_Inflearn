﻿#include "pch.h"
#include "15. EmissiveDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"

void EmissiveDemo::Init()
{
	_shader = make_shared<Shader>(L"12. Lighting_Emissive.fx");
	// RENDER는 사실상 씬에서 동작하는 것이 좋을 것 같지만 씬이 아직 없으므로 여기서 테스트
	RENDER->Init(_shader);
	// 리소스 매니저의 초기화를 수행합니다. (기본 도형 생성)
	RESOURCES->Init();

	/** 카메라 */
	{
		// 카메라를 저장하기 위한 게임 오브젝트를 생성합니다.
		_camera = make_shared<GameObject>();
		// 해당 게임 오브젝트에 Transform 컴포넌트를 추가합니다.
		_camera->GetOrAddTransform()->SetPosition(Vec3(0.0f, 0.0f, -10.0f));
		// 해당 게임 오브젝트에 카메라 컴포넌트를 추가합니다.
		_camera->AddComponent(make_shared<Camera>());
		// 생성한 카메라의 이동 스크립트(CameraScript)를 추가합니다.
		_camera->AddComponent(make_shared<CameraScript>());
	}

	/** Object */
	{
		// Object 1
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
				_obj->GetMeshRenderer()->SetShader(_shader);
			}
			{
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

		// Object 2
		{
			// 게임 오브젝트를 생성합니다.
			_obj2 = make_shared<GameObject>();
			// 해당 게임 오브젝트에 Transform 컴포넌트를 추가합니다. (깊이 조정)
			_obj2->GetOrAddTransform()->SetPosition(Vec3(0.5f, 0.0f, 2.0f));
			// 해당 게임 오브젝트에 필요한 컴포넌트들을 추가합니다.
			_obj2->AddComponent(make_shared<MeshRenderer>());
			// MeshRenderer는 Mesh, Shader, Texture 정보를 가지고 있습니다.
			// * 부품의 추가 정보를 넣어줍니다.
			{
				// 셰이더 정보를 추가합니다.
				_obj2->GetMeshRenderer()->SetShader(_shader);
			}
			{
				// Sphere의 기하학적 모형에 대한 정보를 저장합니다.
				shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
				_obj2->GetMeshRenderer()->SetMesh(mesh);
			}
			{
				// 텍스처 정보를 추가합니다.
				shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
				_obj2->GetMeshRenderer()->SetTexture(texture);
			}
		}

	}

}

void EmissiveDemo::Update()
{
	_camera->Update();
	// 카메라의 수정된 정보를 셰이더에 Push합니다.
	RENDER->Update();

	// * 각각의 오브젝트만의 머터리얼 색상을 설정합니다.
	{
		// * 임시로 빨간 빛을 받아주도록 설정합니다.
		Vec4 material(1.0f, 0.0f, 0.0f, 1.0f);
		// * 설정한 값을 셰이더에 세팅시켜줍니다.
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&material);
		_obj->Update();
	}

	{
		// * 임시로 초록 빛을 받아주도록 설정합니다.
		Vec4 material(0.0f, 1.0f, 0.0f, 1.0f);
		// * 설정한 값을 셰이더에 세팅시켜줍니다.
		_shader->GetVector("MaterialEmissive")->SetFloatVector((float*)&material);
		_obj2->Update();
	}
}

void EmissiveDemo::Render()
{
}
