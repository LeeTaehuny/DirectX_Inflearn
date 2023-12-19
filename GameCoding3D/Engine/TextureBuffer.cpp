#include "pch.h"
#include "TextureBuffer.h"

TextureBuffer::TextureBuffer(ComPtr<ID3D11Texture2D> src)
{
	// _input 텍스쳐 생성
	CreateInput(src);

	CreateBuffer();
}

TextureBuffer::~TextureBuffer()
{

}

void TextureBuffer::CreateBuffer()
{
	CreateSRV();
	CreateOutput();
	CreateUAV();
	CreateResult();
}

void TextureBuffer::CreateInput(ComPtr<ID3D11Texture2D> src)
{
	// 전달받은 텍스쳐로부터 정보를 추출합니다.
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	_width = srcDesc.Width;
	_height = srcDesc.Height;
	_arraySize = srcDesc.ArraySize;
	_format = srcDesc.Format;

	// 추출한 정보를 토대로 _input 텍스쳐를 생성합니다.
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	CHECK(DEVICE->CreateTexture2D(&desc, NULL, _input.GetAddressOf()));
	DC->CopyResource(_input.Get(), src.Get());
}

void TextureBuffer::CreateSRV()
{
	// _input 텍스쳐를 묘사하는 ShaderResourceView를 생성합니다.
	// * _input 텍스쳐의 정보를 추출합니다.
	D3D11_TEXTURE2D_DESC desc;
	_input->GetDesc(&desc);

	// * 정보를 토대로 SRV를 생성합니다.
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	CHECK(DEVICE->CreateShaderResourceView(_input.Get(), &srvDesc, _srv.GetAddressOf()));
}

void TextureBuffer::CreateOutput()
{
	// 셰이더의 결과로 받아온 정보를 저장 위한 텍스쳐를 생성합니다.
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = _width;
	desc.Height = _height;
	desc.ArraySize = _arraySize;
	desc.Format = _format;
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	CHECK(DEVICE->CreateTexture2D(&desc, nullptr, _output.GetAddressOf()));
}

void TextureBuffer::CreateUAV()
{
	// 셰이더에서 받아오기 위한 엑세스 뷰를 생성합니다.
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uavDesc.Texture2DArray.ArraySize = _arraySize;

	CHECK(DEVICE->CreateUnorderedAccessView(_output.Get(), &uavDesc, _uav.GetAddressOf()));
}

void TextureBuffer::CreateResult()
{
	// _output의 내용을 토대로 실제 반환에 사용하기 위한 _outputSRV를 생성합니다.
	D3D11_TEXTURE2D_DESC desc;
	_output->GetDesc(&desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = _arraySize;

	CHECK(DEVICE->CreateShaderResourceView(_output.Get(), &srvDesc, _outputSRV.GetAddressOf()));
}
