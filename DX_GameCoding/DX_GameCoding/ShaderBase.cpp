#include "pch.h"
#include "ShaderBase.h"

ShaderBase::ShaderBase(ComPtr<ID3D11Device> device) : _device(device)
{

}

ShaderBase::~ShaderBase()
{
}

void ShaderBase::LoadShaderFromFile(const wstring& path, const string& name, const string& version)
{
	_path = path;
	_name = name;

	// 비트 플래그 설정 (Debug 모드, 최적화 건너 뛰기)
	const uint32 complieFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// HLSL(High Level Shader Language) 코드를 지정된 대상에 대한 바이트코드로 컴파일합니다.
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		complieFlag,
		0,
		_blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}

VertexShader::VertexShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);

	// VS를 생성합니다.
	HRESULT hr = _device->CreateVertexShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

PixelShader::PixelShader(ComPtr<ID3D11Device> device) : Super(device)
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Create(const wstring& path, const string& name, const string& version)
{
	LoadShaderFromFile(path, name, version);

	// PS를 생성합니다.
	HRESULT hr = _device->CreatePixelShader(_blob->GetBufferPointer(), _blob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}
