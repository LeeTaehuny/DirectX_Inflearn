#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device) : _device(device)
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& desc, ComPtr<ID3DBlob> blob)
{
	// layout의 개수를 저장합니다.
	const int32 count = static_cast<int32>(desc.size());

	// 입력 버퍼 데이터를 설명하는 입력 레이아웃 개체를 만듭니다.
	// * desc.data() : desc의 시작 위치
	_device->CreateInputLayout(desc.data(), count, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
}
