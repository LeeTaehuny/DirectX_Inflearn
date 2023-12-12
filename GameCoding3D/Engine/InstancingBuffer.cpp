#include "pch.h"
#include "InstancingBuffer.h"

InstancingBuffer::InstancingBuffer()
{
	// 버퍼를 생성합니다.
	CreateBuffer(MAX_MESH_INSTANCE);
}

InstancingBuffer::~InstancingBuffer()
{
}

void InstancingBuffer::ClearData()
{
	// 인스턴싱 데이터를 초기화합니다.
	// * 데이터가 매 프레임 변경이 될 수 있으므로 매 번 지우고 다시 받아와 작업하는 방식으로 동작합니다.
	_data.clear();
}

void InstancingBuffer::AddData(InstancingData& data)
{
	// 데이터를 추가합니다.
	_data.push_back(data);
}

void InstancingBuffer::PushData()
{
	// 현재 저장되어 있는 데이터의 수를 받아옵니다.
	const uint32 dataCount = GetCount();

	// 만약 현재 저장된 데이터 수가 최대 데이터 수보다 크다면?
	if (dataCount > _maxCount)
	{
		// 버퍼의 사이즈를 늘려줍니다.
		CreateBuffer(dataCount);
	}

	// 데이터를 삽입합니다.
	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(_instanceBuffer->GetComPtr().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	// 데이터 복사
	{
		// subResource에 _data를 복사합니다.
		::memcpy(subResource.pData, _data.data(), sizeof(InstancingData) * dataCount);
	}

	DC->Unmap(_instanceBuffer->GetComPtr().Get(), 0);

	// GPU에 전달
	_instanceBuffer->PushData();
}

void InstancingBuffer::CreateBuffer(uint32 maxCount)
{
	_maxCount = maxCount;

	// 버퍼를 생성합니다.
	_instanceBuffer = make_shared<VertexBuffer>();

	// 버퍼에 전달하기 위한 임시 벡터
	vector<InstancingData> temp(maxCount);

	// 버퍼를 생성합니다.
	// -> CPUWrite를 켜줘야 매 프레임 수정이 가능합니다.
	// -> true로 설정해 cpu에서 매 프레임 갱신이 가능하도록 설정합니다.
	_instanceBuffer->Create(temp, 1, true);
}
