#include "pch.h"
#include "BillBoardDemo.h"
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
#include "Button.h"
#include "Billboard.h"

void BillBoardDemo::Init()
{
	RESOURCES->Init();
	_shader = make_shared<Shader>(L"28. BillBoardDemo.fx");

	// Camera
	{
		auto camera = make_shared<GameObject>();
		camera->GetOrAddTransform()->SetPosition(Vec3{ 0.f, 0.f, -5.f });
		camera->AddComponent(make_shared<Camera>());
		camera->AddComponent(make_shared<CameraScript>());
		camera->GetCamera()->SetCullingMaskLayerOnOff(Layer_UI, true);
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

	// Mesh
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
		obj->GetOrAddTransform()->SetScale(Vec3(2.f));
		obj->AddComponent(make_shared<MeshRenderer>());
		{
			obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));
		}
		{
			auto mesh = RESOURCES->Get<Mesh>(L"Quad");
			obj->GetMeshRenderer()->SetMesh(mesh);
			obj->GetMeshRenderer()->SetPass(0);
		}

		obj->AddComponent(make_shared<BillBoardTest>());

		CUR_SCENE->Add(obj);
	}

	// Billboard
	{
		auto obj = make_shared<GameObject>();
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0.f));
		obj->AddComponent(make_shared<Billboard>());
		{
			// Material
			{
				shared_ptr<Material> material = make_shared<Material>();
				material->SetShader(_shader);
				auto texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Textures\\grass.png");
				//auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
				material->SetDiffuseMap(texture);
				MaterialDesc& desc = material->GetMaterialDesc();
				desc.ambient = Vec4(1.f);
				desc.diffuse = Vec4(1.f);
				desc.specular = Vec4(1.f);
				RESOURCES->Add(L"Grass", material);

				obj->GetBillboard()->SetMaterial(material);
			}
		}

		for (int32 i = 0; i < 500; i++)
		{

			Vec2 scale = Vec2(1 + rand() % 3, 1 + rand() % 3);
			Vec2 position = Vec2(-100 + rand() % 200, -100 + rand() % 200);

			obj->GetBillboard()->Add(Vec3(position.x, scale.y * 0.5f, position.y), scale);
		}

		CUR_SCENE->Add(obj);
	}
}

void BillBoardDemo::Update()
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

void BillBoardDemo::Render()
{
}

void BillBoardTest::Update()
{
	auto go = GetGameObject();

	// 카메라의 위치를 가져옵니다.
	Vec3 cameraPos = CUR_SCENE->GetMainCamera()->GetTransform()->GetPosition();
	// 자신의 위치를 가져옵니다.
	Vec3 myPos = GetTransform()->GetPosition();

	// 자신이 바라볼 방향을 계산합니다.
	Vec3 forward = cameraPos - myPos;
	forward.Normalize();

	// 임의의 업 방향을 구해줍니다. (외적을 통한 Right 벡터를 구하기 위함)
	Vec3 up = Vec3(0, 1, 0);

	// 자신이 바라보는 방향의 매트릭스를 생성합니다.
	// * 자신의 위치, 앞 방향, 위 방향
	Matrix lookMatrix = Matrix::CreateWorld(myPos, forward, up);

	// 매트릭스를 쿼터니언으로 변환시켜줍니다. 
	// * Decompose : 하나의 매트릭스를 다시 SRT로 분해해주는 함수
	Vec3 S, T;
	Quaternion R;
	lookMatrix.Decompose(S, R, T);

	// 오일러 각으로 변환시켜줍니다. (Vec3)
	Vec3 rot = Transform::ToEulerAngles(R);

	// Transform에 저장합니다.
	GetTransform()->SetRotation(rot);
}
