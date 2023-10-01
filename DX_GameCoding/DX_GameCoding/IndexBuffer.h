#pragma once

// 정점들의 인덱스 정보를 저장하는 버퍼 클래스
class IndexBuffer
{
public:
	IndexBuffer(ComPtr<ID3D11Device> device);
	~IndexBuffer();

	// IndexBuffer를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11Buffer> GetComPtr() { return _indexBuffer; }
	// Stride, Offset, Count를 반환하기 위한 함수를 정의합니다.
	uint32 GetStride() { return _stride; }
	uint32 GetOffset() { return _offset; }
	uint32 GetCount() { return _count; }

	// IndexBuffer를 생성하기 위한 함수를 선언합니다.
	// * indices 배열에 대한 정보가 필요하므로 매개변수로 받아줍니다.
	// * 항상 index의 정보는 uint32 타입으로 관리할 것이므로, 템플릿 함수로 만들 필요가 없습니다.
	void Create(const vector<uint32>& indices);

private:
	// IndexBuffer를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// 정점의 순서들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _indexBuffer;

	uint32 _stride = 0;
	uint32 _offset = 0;
	uint32 _count = 0;
};

