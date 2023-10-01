#pragma once

// 정점들에 대한 정보를 저장하는 버퍼 클래스
class VertexBuffer
{
public:
	VertexBuffer(ComPtr<ID3D11Device> device);
	~VertexBuffer();

	// VertexBuffer를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11Buffer> GetComPtr() {return _vertexBuffer; }
	// Stride, Offset, Count를 반환하기 위한 함수를 정의합니다.
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	// VertexBuffer를 생성하기 위한 함수를 정의합니다.
	// * Vertex 배열에 대한 정보가 필요하므로 매개변수로 받아줍니다.
	// * Vertex 구조체의 정보는 나중에 여러 버전으로 나뉠 수 있습니다.
	//   -> template 함수로 만들어줍니다.
	template<typename T>
	void Create(const vector<T>& vertices)
	{
		// stride : 들어오는 타입의 크기
		_stride = sizeof(T);
		// count  : 정점들의 숫자
		_count = static_cast<uint32>(vertices.size());

		// 버퍼 생성에 사용될 DESC를 생성합니다.
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		// * D3D11_USAGE_IMMUTABLE : GPU만 읽을 수 있는 방식으로 사용하겠다고 설정
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// * D3D11_BIND_VERTEX_BUFFER : VertexBuffer를 바인딩하는 용도로 사용하겠다고 설정
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// * desc의 바이트 크기를 설정합니다. (데이터 유형의 크기 * 수)
		desc.ByteWidth = (uint32)(_stride * _count);

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		// 첫 번째 데이터의 시작 주소를 저장합니다.
		data.pSysMem = vertices.data();

		// 버퍼를 생성해줍니다. (_vertexBuffer에 결과물을 저장합니다.)
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

private:
	// VertexBuffer를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// 정점 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _vertexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

