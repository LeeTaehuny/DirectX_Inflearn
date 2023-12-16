// ComPute Shader : ���� �뵵�� ���̴�

// RawBuffer
// * RW(Read, Write) ����
RWByteAddressBuffer Output; // UAV

// �Է¹ޱ� ���� ����ü (��� ����)
struct ComputeInput
{
	uint3 groupID : SV_GroupID;
	uint3 groupThreadID : SV_GroupThreadID;
	uint3 dispatchThreadID : SV_DispatchThreadID;
	uint groupIndex : SV_GroupIndex;
};

// 10, 8, 3 : �������� thread�� ���� (10 * 8 * 3 = 240���� thread�� ����ϰڴٴ� �ǹ�)
[numthreads(10, 8, 3)]
void CS(ComputeInput input)
{
	// �ε����� �޾ƿ���
	uint index = input.groupIndex;
	// �ּ��� offset ��� (�� Input�� ũ��� uint3, uint3, uint3, uint�� 10���̸�, uint�� 4byte)
	uint outAddress = index * 10 * 4;

	// offset�� ���� ������ ����
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