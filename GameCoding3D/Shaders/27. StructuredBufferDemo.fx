// ComPute Shader : ���� �뵵�� ���̴�

// �Է� ������ ����ü
struct InputDesc
{
	matrix input;
};

// ��� ������ ����ü
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