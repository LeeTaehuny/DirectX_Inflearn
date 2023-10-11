#pragma once
#include "ResourceBase.h"

class Texture : public ResourceBase
{
	using Super = ResourceBase;
public:
	Texture(ComPtr<ID3D11Device> device);
	~Texture();

	// ShaderResourceView를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return _shaderResourceView; }

	// ShaderResourceView를 생성하기 위한 함수를 선언합니다.
	void Create(const wstring& path);

	// 사이즈를 반환하기 위한 함수를 정의합니다.
	Vec2 GetSize() { return _size; }

private:
	// ShaderResourceView를 생성하기 위한 Device와 VS 단계에 연결하기 위한 DeviceContext를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	// ShaderResourceView를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;

	// 사이즈를 저장하기 위한 변수를 선언합니다.
	Vec2 _size = { 0.0f, 0.0f };
};

