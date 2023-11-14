
// VertexInput 단계에서 들어오는 구조체를 정의합니다.
struct VertexInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

// VertexOutput으로 나가는 구조체를 정의합니다.
struct VertexOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// VS
VertexOutput VS(VertexInput input)
{
	VertexOutput output;
	output.position = input.position;
	output.color = input.color;

	return output;
}

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	return input.color;
}

// RS - 와이어프레임모드 설정
RasterizerState FillModeWireFrame
{
	FillMode = Wireframe;
};

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

	// 와이어프레임 모드 출력
	pass P1
	{
		// RS 설정
		SetRasterizerState(FillModeWireFrame);

		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS()
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};