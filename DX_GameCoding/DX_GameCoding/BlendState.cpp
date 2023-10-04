#include "pch.h"
#include "BlendState.h"

BlendState::BlendState(ComPtr<ID3D11Device> device) : _device(device)
{

}

BlendState::~BlendState()
{

}

void BlendState::Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc, float factor)
{
	_blendFactor = factor;

	// BlendState 생성에 사용될 정보를 생성합니다.
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0] = blendDesc;

	// BlendState를 생성합니다.
	HRESULT hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	CHECK(hr);
}