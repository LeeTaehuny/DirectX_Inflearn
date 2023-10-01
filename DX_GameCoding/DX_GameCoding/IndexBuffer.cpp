#include "pch.h"
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(ComPtr<ID3D11Device> device) : _device(device)
{
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Create(const vector<uint32>& indices)
{
	// stride : 들어오는 타입의 크기
	_stride = sizeof(uint32);
	// count  : 인덱스의 숫자
	_count = static_cast<uint32>(indices.size());

	// 버퍼 생성에 사용될 DESC를 생성합니다.
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Usage = D3D11_USAGE_IMMUTABLE;
	// * D3D11_BIND_INDEX_BUFFER : IndexBuffer를 바인딩하는 용도로 사용하겠다고 설정
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = (uint32)(_stride * _count);

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	// 첫 번째 데이터의 시작 주소를 저장합니다.
	data.pSysMem = indices.data();

	// 버퍼를 생성해줍니다. (_indexBuffer에 결과물을 저장합니다.)
	HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
	CHECK(hr);
}
