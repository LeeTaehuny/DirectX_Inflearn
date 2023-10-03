#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device) : _device(device)
{
}

Texture::~Texture()
{
}

void Texture::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	// 이미지를 불러옵니다. (DirectXTex 라이브러리 사용)
	HRESULT hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	// SRV를 생성합니다.
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);
}
