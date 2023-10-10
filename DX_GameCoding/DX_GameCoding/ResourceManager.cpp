#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "Shader.h"
#include "Texture.h"

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
		texture->SetName(L"test");
		// 텍스처를 생성합니다.
		texture->Create(L"test.png");
		// 텍스처를 리소스 목록에 추가합니다.
		Add(texture->GetName(), texture);
	}
}

void ResourceManager::CreateDefaultMesh()
{

}

void ResourceManager::CreateDefaultShader()
{

}

void ResourceManager::CreateDefaultMaterial()
{

}

void ResourceManager::CreateDefaultAnimation()
{

}
