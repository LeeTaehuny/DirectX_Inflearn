#pragma once

// StructureBuffer : RawBuffer와 비슷하지만 입력 구조체를 우리의 형식으로 지정한 버퍼입니다.
class StructuredBuffer
{
public:
	StructuredBuffer(void* inputData, uint32 inputStride, uint32 inputCount, uint32 outputStride = 0, uint32 outputCount = 0);
	~StructuredBuffer();

public:
	// 버퍼들(+ View)을 생성하기 위한 함수
	void CreateBuffer();

	// 데이터를 input 버퍼에 저장시키기 위한 함수를 선언합니다.
	void CopyToInput(void* data);
	// result 버퍼에 들어있는 최종 데이터를 꺼내오기 위한 함수를 선언합니다.
	void CopyFromOutput(void* data);

private:
	void CreateInput();
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

public:
	uint32 GetInputByteWidth() { return _inputStride *_inputCount; }
	uint32 GetOutputByteWidth() { return _outputStride *_outputCount; }

public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

private:
	// 입력을 위한 리소스와 반환을 위한 리소스를 하나씩 만들어줍니다.
	ComPtr<ID3D11Buffer> _input;
	ComPtr<ID3D11Buffer> _output;

	// 리소스(Buffer)를 바로 이용하는 것이 아닌 리소스를 묘사하는 View를 만들어 이용합니다.
	// * 셰이더에 전달하기 위한 리소스 뷰
	ComPtr<ID3D11ShaderResourceView> _srv;
	// * 셰이더에서 받아오기 위한 엑세스 뷰
	ComPtr<ID3D11UnorderedAccessView> _uav;

	// 셰이더에서 받아온 결과는 _uav에 저장됩니다.
	// * 받아온 정보를 다시 꺼내오기 위한 버퍼를 선언합니다. (최종 결과를 취합하기 위함)
	ComPtr<ID3D11Buffer> _result;

private:
	// 입력받을 데이터를 저장하기 위한 변수를 선언합니다.
	void* _inputData;

	// 이제는 데이터의 형식을 이미 알고 있기 때문에 길이와 카운트만을 저장해주기 위한 변수를 선언합니다.
	uint32 _inputStride = 0;
	uint32 _inputCount = 0;
	uint32 _outputStride = 0;
	uint32 _outputCount = 0;
};

