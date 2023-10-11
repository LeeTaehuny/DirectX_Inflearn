#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Shader.h"
#include "Texture.h"
#include "ResourceBase.h"

ResourceManager::ResourceManager(ComPtr<ID3D11Device> device) : _device(device)
{
}

void ResourceManager::Init()
{
	// test
	CreateDefaultTexture();
	CreateDefaultMesh();
	CreateDefaultShader();
	CreateDefaultMaterial();
	CreateDefaultAnimation();
}

void ResourceManager::CreateDefaultTexture()
{
	// 텍스처 생성(리소스 매니저에 추가)
	{
		// 텍스처를 저장하기 위한 포인터 변수를 선언합니다.
		auto texture = make_shared<Texture>(_device);
		// 텍스처의 이름을 지정합니다.
		texture->SetName(L"Snake");
		// 텍스처를 생성합니다.
		texture->Create(L"Snake.bmp");
		// 텍스처를 리소스 목록에 추가합니다.
		Add(texture->GetName(), texture);
	}

	// 애니메이션에 사용될 텍스처 생성(리소스 매니저에 추가)
	{
		// 텍스처를 저장하기 위한 포인터 변수를 선언합니다.
		auto texture = make_shared<Texture>(_device);
		// 텍스처의 이름을 지정합니다.
		texture->SetName(L"test");
		// 텍스처를 생성합니다.
		texture->Create(L"test.png");
		// 텍스처를 리소스 목록에 추가합니다.
		Add(texture->GetName(), texture);
	}
}

void ResourceManager::CreateDefaultMesh()
{
	// 메시 생성(리소스 매니저에 추가)
	{
		// 메시를 생성합니다.
		shared_ptr<Mesh> mesh = make_shared<Mesh>(_device);
		// 이름을 설정합니다.
		mesh->SetName(L"Rectangle");
		// 사각형 정보를 생성합니다.
		mesh->CreateDefaultRectangle();
		// 메시를 리소스 매니저에 추가합니다.
		Add(mesh->GetName(), mesh);
	}

}

void ResourceManager::CreateDefaultShader()
{
	// VertexShader 객체를 생성합니다.
	shared_ptr<VertexShader> vertexShader = make_shared<VertexShader>(_device);
	// VertexShader 생성
	vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

	// InputLayout 객체를 생성합니다.
	shared_ptr<InputLayout> inputLayout = make_shared<InputLayout>(_device);
	// InputLayout 생성
	inputLayout->Create(VertexTextureData::descs, vertexShader->GetBlob());

	// PixelShader 객체를 생성합니다.
	shared_ptr<PixelShader> pixelShader = make_shared<PixelShader>(_device);
	// PixelShader 생성
	pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	// 셰이더 생성(리소스 매니저에 추가)
	{
		// 셰이더 객체를 생성합니다.
		shared_ptr<Shader> shader = make_shared<Shader>();
		// 이름을 설정합니다.
		shader->SetName(L"Default");
		// 셰이더들을 세팅합니다.
		shader->SetVertexShader(vertexShader);
		shader->SetInputLayout(inputLayout);
		shader->SetPixelShader(pixelShader);

		// 셰이더를 리소스 매니저에 추가합니다.
		Add(shader->GetName(), shader);
	}
}

void ResourceManager::CreateDefaultMaterial()
{
	// 머터리얼 생성(리소스 매니저에 추가)
	{
		// 머터리얼을 생성합니다.
		shared_ptr<Material> material = make_shared<Material>();
		// 이름을 설정합니다.
		material->SetName(L"Default");
		// 셰이더를 설정합니다.
		material->SetShader(Get<Shader>(L"Default"));
		// 텍스처를 설정합니다.
		material->SetTexture(Get<Texture>(L"test"));

		// 머터리얼을 리소스 매니저에 추가합니다.
		Add(material->GetName(), material);
	}

}

void ResourceManager::CreateDefaultAnimation()
{
	// 애니메이션 생성(리소스 매니저에 추가)
	{
		shared_ptr<Animation> animation = make_shared<Animation>();
		animation->SetName(L"SnakeAnim");
		animation->SetTexture(Get<Texture>(L"Snake"));
		animation->SetLoop(true);

		animation->AddKeyframe(Keyframe{ Vec2{0.0f, 0.0f}, Vec2{100.0f, 100.0f}, 0.1f });
		animation->AddKeyframe(Keyframe{ Vec2{100.0f, 0.0f}, Vec2{100.0f, 100.0f}, 0.1f });
		animation->AddKeyframe(Keyframe{ Vec2{200.0f, 0.0f}, Vec2{100.0f, 100.0f}, 0.1f });
		animation->AddKeyframe(Keyframe{ Vec2{300.0f, 0.0f}, Vec2{100.0f, 100.0f}, 0.1f });

		// 애니메이션을 리소스 매니저에 추가합니다.
		Add(animation->GetName(), animation);
	}
}
