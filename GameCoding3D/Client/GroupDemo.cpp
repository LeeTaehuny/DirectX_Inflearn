#include "pch.h"
#include "GroupDemo.h"
#include "RawBuffer.h"

void GroupDemo::Init()
{
	_shader = make_shared<Shader>(L"25. GroupDemo.fx");

	// 스레드의 개수를 구해줍니다.
	uint32 threadCount = 10 * 8 * 3;

	// 그룹 수를 구해줍니다.
	uint32 groupCount = 2 * 1 * 1;

	// 최종적으로 사용되는 스레드의 개수를 구해줍니다.
	uint32 count = groupCount * threadCount;

	// Input데이터 생성
	vector<Input> inputs(count);
	{
		for (int32 i = 0; i < count; i++)
		{
			inputs[i].value = rand() % 10000;
		}
	}

	// RawBuffer 생성
	// * 입력데이터 존재 (Input 구조체의 크기 * 수)
	// * 출력데이터 존재. 셰이더 파일의 input 데이터와 동일. (Output 구조체의 크기 * 스레드 개수)
	shared_ptr<RawBuffer> rawBuffer = make_shared<RawBuffer>(inputs.data(), sizeof(Input) * count, sizeof(Output) * count);

	// 셰이더 연동
	// * 셰이더 파일에 있는 Input(SRV)를 통해 정보를 전달해줍니다.
	_shader->GetSRV("Input")->SetResource(rawBuffer->GetSRV().Get());
	// * 셰이더 파일에 있는 Output(UAV)를 통해 정보를 받아와야 합니다.
	// * 전역 공간의 Output이라는 변수를 불러와 RawBuffer의 출력 전용 뷰를 세팅합니다.
	_shader->GetUAV("Output")->SetUnorderedAccessView(rawBuffer->GetUAV().Get());

	// Compute Shader 실행
	// * x, y, z : 스레드 그룹 지정
	_shader->Dispatch(0, 0, 2, 1, 1);

	// 결과 받아오기
	vector<Output> outputs(count);
	// * rawBuffer에 존재하던 데이터를 outputs 컨테이너로 복사
	rawBuffer->CopyFromOutput(outputs.data());

	// csv 파일로 만들어서 결과를 확인하기
	{
		FILE* file;
		::fopen_s(&file, "../RawBuffer2.csv", "w");

		::fprintf
		(
			file,
			"GroupID(X),GroupID(Y),GroupID(Z),GroupThreadID(X),GroupThreadID(Y),GroupThreadID(Z),DispatchThreadID(X),DispatchThreadID(Y),DispatchThreadID(Z),GroupIndex,Value\n"
		);

		for (uint32 i = 0; i < count; i++)
		{
			const Output& temp = outputs[i];

			::fprintf
			(
				file,
				"%d,%d,%d,	%d,%d,%d,	%d,%d,%d,	%d,%f\n",
				temp.groupID[0], temp.groupID[1], temp.groupID[2],
				temp.groupThreadID[0], temp.groupThreadID[1], temp.groupThreadID[2],
				temp.dispatchThreadID[0], temp.dispatchThreadID[1], temp.dispatchThreadID[2],
				temp.groupIndex, temp.value
			);
		}

		::fclose(file);
	}
}

void GroupDemo::Update()
{

}

void GroupDemo::Render()
{
}
