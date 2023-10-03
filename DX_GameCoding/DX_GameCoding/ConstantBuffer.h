#pragma once

// ConstantBuffer는 물체의 Transform 정보에 따라 생성이 되며, 물체의 Transform 정보는 각각 상이할 수 있습니다.
// * 템플릿 문법을 적용해줍니다.
template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext) : _device(device), _deviceContext(deviceContext) { }
	~ConstantBuffer() { }

	// ConstantBuffer를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

	// ConstantBuffer를 생성하기 위한 함수를 정의합니다.
	void Create()
	{
		// 버퍼 생성에 사용될 DESC를 생성합니다.
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		// D3D11_USAGE_DYNAMIC : CPU_Write + GPU_Read
		desc.Usage = D3D11_USAGE_DYNAMIC;
		// D3D11_BIND_CONSTANT_BUFFER : 상수 버퍼 용도로 사용하겠다는 설정
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		// D3D11_CPU_ACCESS_WRITE : CPU의 쓰기를 허용한다는 플래그
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// 버퍼를 생성해줍니다. (_constantBuffer에 결과물을 저장합니다.)
		HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		CHECK(hr);
	}

	// ConstantBuffer에 데이터를 복사하기 위한 함수를 정의합니다.
	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		ZeroMemory(&subResource, sizeof(subResource));

		// _transformData를 매 프레임마다 _constantBuffer에 복사를 해주겠습니다.
		// * Map으로 데이터를 넣어줄 준비를 합니다.
		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

		// * 해당 위치에서 _transformData를 GPU에 복사해줍니다.
		::memcpy(subResource.pData, &data, sizeof(data));

		// * 사용이 끝났으므로 Unmap으로 Map을 해제합니다.
		_deviceContext->Unmap(_constantBuffer.Get(), 0);
	}

private:
	// ConstantBuffer를 생성하기 위한 Device와 Map, Unmap을 사용하기 위한 DeviceContext를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	// 상수 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _constantBuffer;
};

