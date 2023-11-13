
// VertexInput 단계에서 들어오는 구조체를 정의합니다.
struct VertexInput
{
	float4 position : POSITION;
};

// VertexOutput으로 나가는 구조체를 정의합니다.
struct VertexOutput
{
	float4 position : SV_POSITION;
};

// VS
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;

	return output;
}

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

float4 PS2(VertexOutput input) : SV_TARGET
{
	return float4(0, 1, 0, 1);
}

float4 PS3(VertexOutput input) : SV_TARGET
{
	return float4(0, 0, 1, 1);
}

// Effect11
technique11 T0
{
	pass P0
	{
		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS()
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS2()
		SetPixelShader(CompileShader(ps_5_0, PS2()));
	}
};

technique11 T1
{
	pass P0
	{
		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS3()
		SetPixelShader(CompileShader(ps_5_0, PS3()));
	}
};