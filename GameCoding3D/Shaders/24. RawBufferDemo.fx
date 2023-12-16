// ComPute Shader : 연산 용도의 셰이더

// RawBuffer
// * RW(Read, Write) 가능
RWByteAddressBuffer Output; // UAV

// 입력받기 위한 구조체 (상수 버퍼)
struct ComputeInput
{
	uint3 groupID : SV_GroupID;
	uint3 groupThreadID : SV_GroupThreadID;
	uint3 dispatchThreadID : SV_DispatchThreadID;
	uint groupIndex : SV_GroupIndex;
};

// 10, 8, 3 : 실질적인 thread의 개수 (10 * 8 * 3 = 240개의 thread를 사용하겠다는 의미)
[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
	// 인덱스를 받아오기
	uint index = input.groupIndex;
	// 주소의 offset 계산 (한 Input의 크기는 uint3, uint3, uint3, uint로 10개이며, uint는 4byte)
	uint outAddress = index * 10 * 4;

	// offset에 맞춰 데이터 저장
	Output.Store3(outAddress + 0, input.groupID);
	Output.Store3(outAddress + 12, input.groupThreadID);
	Output.Store3(outAddress + 24, input.dispatchThreadID);
	Output.Store(outAddress + 36, input.groupIndex);
}

// Effect11
technique11 T0
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};