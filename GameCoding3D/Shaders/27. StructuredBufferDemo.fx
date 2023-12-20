// ComPute Shader : 연산 용도의 셰이더

// 입력 데이터 구조체
struct InputDesc
{
	matrix input;
};

// 출력 데이터 구조체
struct OutputDesc
{
	matrix result;
};

StructuredBuffer<InputDesc> Input;		// SRV
RWStructuredBuffer<OutputDesc> Output;	// UAV

[numthreads(500, 1, 1)]
void CS(uint id : SV_GroupIndex)
{
	matrix result = Input[id].input * 2;
	
	Output[id].result = result;
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