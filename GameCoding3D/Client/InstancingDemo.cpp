﻿#include "pch.h"
#include "InstancingDemo.h"
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

void InstancingDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"19. InstancingDemo.fx");

	// Camera
	_camera = make_shared<GameObject>();
	_camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
	_camera->AddComponent(make_shared<Camera>());
	_camera->AddComponent(make_shared<CameraScript>());

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

		// 복사하기 위한 정보를 저장합니다. (INSTANCING)
		_material = material;
	}

	// 동일한 object 10000개 생성 -> FPS 41까지 내려감
	for(int32 i = 0; i < 10000; i++)
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetPosition(Vec3(rand() % 100, 0, rand() % 100));
		obj->AddComponent(make_shared<MeshRenderer>());

		// Set Material
		obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));

		// Set Mesh
		auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
		obj->GetMeshRenderer()->SetMesh(mesh);		
		// 복사하기 위한 정보를 저장합니다. (INSTANCING)
		_mesh = mesh;

		// 생성한 오브젝트를 배열에 추가
		_objs.push_back(obj);
	}

	RENDER->Init(_shader);

	// 복사하기 위한 정보를 저장합니다. (INSTANCING)
	{
		_instanceBuffer = make_shared<VertexBuffer>();

		// 모든 오브젝트를 순회하며 W에 대한 정보들을 저장합니다.
		for (auto& obj : _objs)
		{
			Matrix world = obj->GetTransform()->GetWorldMatrix();
			_worlds.push_back(world);
		}

		// 버퍼를 생성합니다.
		_instanceBuffer->Create(_worlds, 1);
	}
}

void InstancingDemo::Update()
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

	// Objs 업데이트
	//for (auto& obj : _objs)
	//{
	//	obj->Update();
	//}

	_material->Update();

	// 저장했던 정보들을 한 번에 Push하여 Render
	{
		_mesh->GetVertexBuffer()->PushData();
		_instanceBuffer->PushData();
		_mesh->GetIndexBuffer()->PushData();

		_shader->DrawIndexedInstanced(0, 0, _mesh->GetIndexBuffer()->GetCount(), _objs.size());
	}
}

void InstancingDemo::Render()
{
}
