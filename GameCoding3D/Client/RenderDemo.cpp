#include "pch.h"
#include "RenderDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "GameObject.h"
#include "CameraScript.h"
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

void RenderDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	// Anim Model
	{
		shared_ptr<class Model> m1 = make_shared<Model>();
		m1->ReadModel(L"Kachujin/Kachujin");
		m1->ReadMaterial(L"Kachujin/Kachujin");
		// animation
		m1->ReadAnimation(L"Kachujin/Idle");
		m1->ReadAnimation(L"Kachujin/Run");
		m1->ReadAnimation(L"Kachujin/Slash");

		for (int32 i = 0; i < 50; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
			obj->AddComponent(make_shared<ModelAnimator>(_shader));

			{
				obj->GetModelAnimator()->SetModel(m1);
				obj->GetModelAnimator()->SetPass(2);
			}

			// 생성한 오브젝트를 배열에 추가
			_objs.push_back(obj);
		}
	}
	
	// Model
	{
		shared_ptr<class Model> m2 = make_shared<Model>();
		m2->ReadModel(L"Tower/Tower");
		m2->ReadMaterial(L"Tower/Tower");

		// 동일한 object 10000개 생성 -> FPS 41까지 내려감
		for (int32 i = 0; i < 100; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
			obj->AddComponent(make_shared<ModelRenderer>(_shader));

			{
				obj->GetModelRenderer()->SetModel(m2);
				obj->GetModelRenderer()->SetPass(1);
			}

			// 생성한 오브젝트를 배열에 추가
			_objs.push_back(obj);
		}
	}
	
	// Mesh
	{
		{
			shared_ptr<Material> material = make_shared<Material>();
			material->SetShader(_shader);
			auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
			material->SetDiffuseMap(texture);
			MaterialDesc& desc = material->GetMaterialDesc();
			desc.ambient = Vec4(1.f);
			desc.diffuse = Vec4(1.f);
			desc.specular = Vec4(1.f);
			RESOURCES->Add(L"Veigar", material);
		}

		// 동일한 object 10000개 생성 -> FPS 41까지 내려감
		for (int32 i = 0; i < 100; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0, rand() % 100));
			obj->AddComponent(make_shared<MeshRenderer>());

			// Set Material
			obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));

			// Set Mesh
			auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
			obj->GetMeshRenderer()->SetPass(0);

			// 생성한 오브젝트를 배열에 추가
			_objs.push_back(obj);
		}
	}
	

	RENDER->Init(_shader);
}

void RenderDemo::Update()
{
	_camera->Update();
	RENDER->Update();

	// Light 정보
	{
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);
		RENDER->PushLightData(lightDesc);
	}

	INSTANCING->Render(_objs);
}

void RenderDemo::Render()
{
}
