#pragma once

// RawBuffer를 사용하는 과정
// 1. RawBuffer를 통해 입력을 받아오기
// 2. ComputeShader에 연산을 요청
// 3. 연산이 완료되면 결과를 다시 받아오기
class RawBuffer
{
public:
	// InputData는 생성자에서 받아 저장합니다.
	RawBuffer(void* inputData, uint32 inputByte, uint32 outputByte);
	~RawBuffer();

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
	// Getter
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
	// * 어떤 데이터가 들어올지 예측 불가하므로 void* 형으로 선언합니다.
	void* _inputData;

	// 입력받은 정보의 크기를 저장하기 위한 변수를 선언합니다.
	uint32 _inputByte = 0;
	// 출력해야 하는 정보의 크기를 저장하기 위한 변수를 선언합니다.
	uint32 _outputByte = 0;
};

