// ComPute Shader : 연산 용도의 셰이더

Texture2DArray<float4> Input;		// SRV
RWTexture2DArray<float4> Output;	// UAV

// 32, 32, 1 : 실질적인 thread의 개수 (1024)
[numthreads(32, 32, 1)]
// 입력 데이터로 SV_DispatchThreadID만 이용해보도록 하겠습니다.
void CS(uint3 id : SV_DispatchThreadID)
{
	// Input으로부터 id에 해당하는 색상을 가지고 옵니다.
	// * SV_DispatchThreadID은 스레드의 인덱스를 의미합니다.
	float4 color = Input.Load(int4(id, 0));

	// 1. 그대로 출력으로 전달
	// Output[id] = color;

	// 2. 색상 반전
	// Output[id] = 1.0f - color;

	// 3. 회색(전체 줄이기)
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