#include "pch.h"
#include "ViewportDemo.h"
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

void ViewportDemo::Init()
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

		for (int32 i = 0; i < 1; i++)
		{
			auto obj = make_shared<GameObject>();
			obj->GetOrAddTransform()->SetPosition(Vec3(0.0f, 0.0f, 0.0f));
			obj->AddComponent(make_shared<MeshRenderer>());

			// Set Material
			obj->GetMeshRenderer()->SetMaterial(RESOURCES->Get<Material>(L"Veigar"));

			// Set Mesh
			auto mesh = RESOURCES->Get<Mesh>(L"Sphere");
			obj->GetMeshRenderer()->SetMesh(mesh);
			obj->GetMeshRenderer()->SetPass(0);

			// 생성한 오브젝트를 배열에 추가
			CUR_SCENE->Add(obj);
		}
	}
	
	//RENDER->Init(_shader);
}

void ViewportDemo::Update()
{
	// viewport 조정 실습
	static float width = 800.0f;
	static float height = 600.0f;
	static float x = 0.0f;
	static float y = 0.0f;

	ImGui::InputFloat("Width", &width, 10.0f);
	ImGui::InputFloat("height", &height, 10.0f);
	ImGui::InputFloat("x", &x, 10.0f);
	ImGui::InputFloat("y", &y, 10.0f);

	GRAPHICS->SetViewport(width, height, x, y);

	// 임의의 좌표가 있다고 가정했을 때 해당 좌표가 2D화면에서 어떤 좌표가 되며, 원상복구가 되는지 테스트
	static Vec3 pos = Vec3(2.0f, 0.0f, 0.0f);
	ImGui::InputFloat3("pos", (float*)&pos);

	Viewport& vp = GRAPHICS->GetViewport();
	Vec3 pos2D = vp.Project(pos, Matrix::Identity, Camera::S_MatView, Camera::S_MatProjection);
	ImGui::InputFloat3("pos2D", (float*)&pos2D);
	
	{
		Vec3 temp = vp.UnProject(pos2D, Matrix::Identity, Camera::S_MatView, Camera::S_MatProjection);
		ImGui::InputFloat3("Recalc", (float*)&temp);
	}
}

void ViewportDemo::Render()
{
}
