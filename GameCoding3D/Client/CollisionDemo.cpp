#include "pch.h"
#include "CollisionDemo.h"
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
#include "Light.h"
#include "TextureBuffer.h"
#include "Viewport.h"
#include "SphereCollider.h"
#include "Scene.h"
#include "AABBBoxCollider.h"
#include "OBBBoxCollider.h"
#include "Terrain.h"

void CollisionDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"23. RenderDemo.fx");

	// Camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraScript>());

		CUR_SCENE->Add(camera);
	}

	// Light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());

		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(1.f, 0.f, 1.f);

		light->GetLight()->SetLightDesc(lightDesc);

		CUR_SCENE->Add(light);
	}
	
	// Material
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

	// Terrain
	{
		auto obj = make_shared<GameObject>();
		obj->AddComponent(make_shared <Terrain>());
		obj->GetTerrain()->Create(10, 10, RESOURCES->Get<Material>(L"Veigar"));

		// 생성한 오브젝트를 배열에 추가
		CUR_SCENE->Add(obj);
	}

	// Mesh 1
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(3.f, 0.f, 0.f));
		obj->AddComponent(make_shared<MeshRenderer>());

		// Set Material
		obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));

		// Set Mesh
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(0);

		// Collider
		auto collider = make_shared<SphereCollider>();
		collider->SetRadius(0.5f);
		obj->AddComponent(collider);

		// Script
		//obj->AddComponent(make_shared<MoveScript>());

		// 생성한 오브젝트를 배열에 추가
		CUR_SCENE->Add(obj);
	}

	// Mesh 2
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
		obj->AddComponent(make_shared<MeshRenderer>());

		// Set Material
		obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));

		// Set Mesh
		auto mesh = RESOURCES->Get<Mesh>(L"Cube");
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetPass(0);

		// Collider AABB
		//{
		//	auto collider = make_shared<AABBBoxCollider>();
		//	collider->GetBoundingBox().Extents = Vec3(0.5f);
		//	obj->AddComponent(collider);
		//
		//}

		// Collider OBB
		{
			obj->GetOrAddTransform()->SetRotation(Vec3(30, 45, 0));

			auto collider = make_shared<OBBBoxCollider>();
			collider->GetBoundingBox().Extents = Vec3(0.5f);
			collider->GetBoundingBox().Orientation = Quaternion::CreateFromYawPitchRoll(45, 0, 30);
			obj->AddComponent(collider);
		}

		// 생성한 오브젝트를 배열에 추가
		CUR_SCENE->Add(obj);
	}
}

void CollisionDemo::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		// Screen 좌표
		int32 mouseX = INPUT->GetMousePos().x;
		int32 mouseY = INPUT->GetMousePos().y;

		// Picking (선택하기)
		auto pickObj = CUR_SCENE->Pick(mouseX, mouseY);

		// 만약 선택된 물체가 존재한다면?
		if (pickObj)
		{
			// 해당 물체를 삭제합니다.
			CUR_SCENE->Remove(pickObj);
		}
	}
}

void CollisionDemo::Render()
{
}

//void MoveScript::Update()
//{
//	auto pos = GetTransform()->GetPosition();
//	pos.x -= DT * 1.0f;
//	GetTransform()->SetPosition(pos);
//}
