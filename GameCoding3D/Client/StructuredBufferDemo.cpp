#include "pch.h"
#include "StructuredBufferDemo.h"
#include "RawBuffer.h"
#include "StructuredBuffer.h"

void StructuredBufferDemo::Init()
{
	_shader = make_shared<Shader>(L"27. StructuredBufferDemo.fx");

	// 입력 데이터를 생성합니다.
	vector<Matrix> inputs(500, Matrix::Identity);

	// 버퍼를 생성합니다.
	auto buffer = make_shared<StructuredBuffer>(inputs.data(), sizeof(Matrix), 500, sizeof(Matrix), 500);

	// 셰이더에 연결해줍니다.
	// * 입력 데이터 연결
	_shader->GetSRV("Input")->SetResource(buffer->GetSRV().Get());
	// * 출력 데이터 연결
	_shader->GetUAV("Output")->SetUnorderedAccessView(buffer->GetUAV().Get());

	// 셰이더를 실행합니다.
	_shader->Dispatch(0, 0, 1, 1, 1);

	// 결과를 확인하기 위한 배열을 생성합니다.
	vector<Matrix> outputs(500);
	buffer->CopyFromOutput(outputs.data());

}

void StructuredBufferDemo::Update()
{

}

void StructuredBufferDemo::Render()
{
}
