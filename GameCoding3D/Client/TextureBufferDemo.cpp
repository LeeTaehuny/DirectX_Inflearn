#include "pch.h"
#include "TextureBufferDemo.h"
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

void TextureBufferDemo::Init()
{
	// 텍스쳐 조정
	auto newSrv = MakeComputeShaderTexture();

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
			
			// Compute Shader의 연산 결과로 나온 SRV를 텍스쳐에 지정하기
			auto texture = make_shared<Texture>();
			texture->SetSRV(newSrv);

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
			CUR_SCENE->Add(obj);
		}
	}
	
	//RENDER->Init(_shader);
}

void TextureBufferDemo::Update()
{

}

void TextureBufferDemo::Render()
{
}

ComPtr<ID3D11ShaderResourceView> TextureBufferDemo::MakeComputeShaderTexture()
{
	// Compute Shader를 생성합니다.
	auto shader = make_shared<Shader>(L"26. TextureBufferDemo.fx");

	// 사용할 원본 텍스쳐를 로드합니다.
	auto texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
	// 텍스처 버퍼를 생성합니다.
	shared_ptr<TextureBuffer> textureBuffer = make_shared<TextureBuffer>(texture->GetTexture2D());

	// 셰이더에 연동해줍니다.
	// * 입력 데이터 연동
	shader->GetSRV("Input")->SetResource(textureBuffer->GetSRV().Get());
	// * 출력 데이터 연동
	shader->GetUAV("Output")->SetUnorderedAccessView(textureBuffer->GetUAV().Get());

	// Compute Shader를 실행합니다. (Dispatch)
	// * 텍스처 버퍼에 저장된 원본 텍스처의 정보를 가져옵니다.
	uint32 width = textureBuffer->GetWidth();
	uint32 height = textureBuffer->GetHeight();
	uint32 arraySize = textureBuffer->GetArraySize();

	// * 스레드 개수를 지정합니다.
	//	 -> 소수점 단위로 올려주기 위한 연산을 진행합니다. (32 기준)
	uint32 x = max(1, (width + 31) / 32);
	uint32 y = max(1, (height + 31) / 32);

	// * Compute Shader 실행
	shader->Dispatch(0, 0, x, y, arraySize);

	// 결과를 반환해줍니다.
	return textureBuffer->GetOutputSRV();
}
