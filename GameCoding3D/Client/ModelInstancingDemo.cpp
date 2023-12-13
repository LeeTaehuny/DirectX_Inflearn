#include "pch.h"
#include "ModelInstancingDemo.h"
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

void ModelInstancingDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"21. ModelInstancingDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

	shared_ptr<class Model> m1 = make_shared<Model>();
	m1->ReadModel(L"Tower/Tower");
	m1->ReadMaterial(L"Tower/Tower");

	// 동일한 object 10000개 생성 -> FPS 41까지 내려감
	for(int32 i = 0; i < 10000; i++)
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0, rand() % 100));
		obj->GetOrAddTransform()->SetScale(Vec3(0.01f));
		obj->AddComponent(make_shared<ModelRenderer>(_shader));

		{
			obj->GetModelRenderer()->SetModel(m1);
		}

		// 생성한 오브젝트를 배열에 추가
		_objs.push_back(obj);
	}

	RENDER->Init(_shader);
}

void ModelInstancingDemo::Update()
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

void ModelInstancingDemo::Render()
{
}
