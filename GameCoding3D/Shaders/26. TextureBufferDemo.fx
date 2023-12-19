// ComPute Shader : ���� �뵵�� ���̴�

Texture2DArray<float4> Input;		// SRV
RWTexture2DArray<float4> Output;	// UAV

// 32, 32, 1 : �������� thread�� ���� (1024)
[numthreads(32, 32, 1)]
// �Է� �����ͷ� SV_DispatchThreadID�� �̿��غ����� �ϰڽ��ϴ�.
void CS(uint3 id : SV_DispatchThreadID)
{
	// Input���κ��� id�� �ش��ϴ� ������ ������ �ɴϴ�.
	// * SV_DispatchThreadID�� �������� �ε����� �ǹ��մϴ�.
	float4 color = Input.Load(int4(id, 0));

	// 1. �״�� ������� ����
	// Output[id] = color;

	// 2. ���� ����
	// Output[id] = 1.0f - color;

	// 3. ȸ��(��ü ���̱�)
	Output[id] = (color.r + color.g + color.b) / 3.0f;
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