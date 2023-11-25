#include "pch.h"
#include "18. NormalMappingDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Material.h"

void NormalMappingDemo::Init()
{
	_shader = make_shared<Shader>(L"14. NormalMapping.fx");
	// RENDER는 사실상 씬에서 동작하는 것이 좋을 것 같지만 씬이 아직 없으므로 여기서 테스트
	RENDER->Init(_shader);
	// 리소스 매니저의 초기화를 수행합니다. (기본 도형 생성)
	RESOURCES->Init();

	/** Material */
	{
		// Veigar 머터리얼
		{
			shared_ptr<Material> material = make_shared<Material>();

			{
				// 셰이더 설정
				material->SetShader(_shader);
			}
			{
				// diffuseMap 설정
				shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Wood", L"..\\Resources\\Textures\\Wood.jpg");
				material->SetDiffuseMap(texture);
			}
			{
				// NormalMap 설정
				shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Wood_Normal", L"..\\Resources\\Textures\\Wood_Normal.jpg");
				material->SetNormalMap(texture);
			}

			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(0.3f);
			desc.diffuse = Vec4(0.3f);

			// 리소스 매니저에 Veigar 이름으로 해당 머터리얼 등록
			RESOURCES->Add(L"Wood", material);
		}

		// Leather 머터리얼
		{
			shared_ptr<Material> material = make_shared<Material>();

			{
				// 셰이더 설정
				material->SetShader(_shader);
			}
			{
				// diffuseMap 설정
				shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Leather", L"..\\Resources\\Textures\\Leather.jpg");
				material->SetDiffuseMap(texture);
			}
			{
				// NormalMap 설정
				shared_ptr<Texture> texture = RESOURCES->Load<Texture>(L"Leather_Normal", L"..\\Resources\\Textures\\Leather_Normal.jpg");
				material->SetNormalMap(texture);
			}

			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(1.0f);
			desc.diffuse = Vec4(1.0f);
			desc.specular = Vec4(1.0f);

			// 리소스 매니저에 Veigar 이름으로 해당 머터리얼 등록
			RESOURCES->Add(L"Leather", material);
		}
	}

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
			// MeshRenderer는 Mesh, Material 정보를 가지고 있습니다.
			{
				// Sphere의 기하학적 모형에 대한 정보를 저장합니다.
				shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Sphere");
				_obj->GetMeshRenderer()->SetMesh(mesh);
			}
			{
				// Material을 리소스 매니저에서 가져옵니다.
				auto material = RESOURCES->Get<Material>(L"Wood");
				// MeshRenderer()에 머터리얼을 설정합니다.
				_obj->GetMeshRenderer()->SetMaterial(material);
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
			// MeshRenderer는 Mesh, Material 정보를 가지고 있습니다.
			{
				// Sphere의 기하학적 모형에 대한 정보를 저장합니다.
				shared_ptr<Mesh> mesh = RESOURCES->Get<Mesh>(L"Cube");
				_obj2->GetMeshRenderer()->SetMesh(mesh);
			}
			{
				// Material을 리소스 매니저에서 가져옵니다.
				// * 기존 머터리얼을 복사해 쓰고 싶으면 Clone() 함수 사용, 아니면 새로운 머터리얼을 미리 등록해두기.
				auto material = RESOURCES->Get<Material>(L"Leather");
				// MeshRenderer()에 머터리얼을 설정합니다.
				_obj2->GetMeshRenderer()->SetMaterial(material);
			}
		}

	}

}

void NormalMappingDemo::Update()
{
	_camera->Update();
	// 카메라의 수정된 정보를 셰이더에 Push합니다.
	RENDER->Update();

	LightDesc lightDesc;
	lightDesc.ambient = Vec4(0.5f);
	lightDesc.diffuse = Vec4(1.0f);
	lightDesc.specular = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightDesc.direction = Vec3(1.0f, 0.0f, 1.0f);

	RENDER->PushLightData(lightDesc);

	{
		_obj->Update();
	}

	{
		_obj2->Update();
	}
}

void NormalMappingDemo::Render()
{
}
