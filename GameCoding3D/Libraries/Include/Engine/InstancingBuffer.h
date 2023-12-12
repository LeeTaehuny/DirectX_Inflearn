#pragma once

class VertexBuffer;

// 인스턴싱에 필요한 데이터를 저장하기 위한 구조체를 선언합니다. 
struct InstancingData
{
	// 월드 변환 행렬
	Matrix world;
};

#define MAX_MESH_INSTANCE 500

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

public:
	// 인스턴싱 데이터를 초기화하기 위한 함수를 선언합니다.
	void ClearData();
	// 인스턴싱 데이터를 추가하기 위한 함수를 선언합니다.
	void AddData(InstancingData& data);
	// 셰이더에 인스턴싱 데이터를 전달하기 위한 함수를 선언합니다.
	void PushData();

// Getter
public:
	uint32 GetCount() { return static_cast<uint32>(_data.size()); }
	shared_ptr<VertexBuffer> GetBuffer() { return _instanceBuffer; }

private:
	// 버퍼를 생성하기 위한 함수를 선언합니다.
	void CreateBuffer(uint32 maxCount = MAX_MESH_INSTANCE);

private:
	//uint64						_instanceId = 0;

	// 셰이더에 넘겨주기 위한 버퍼를 저장하기 위한 변수를 선언합니다.
	shared_ptr<VertexBuffer>	_instanceBuffer;

	// 버퍼의 크기(인스턴싱 수)를 저장하기 위한 변수를 선언합니다.
	uint32						_maxCount = 0;

	vector<InstancingData>		_data;
};

