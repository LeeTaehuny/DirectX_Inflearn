// 외부에서 연결하기 위한 변수들
matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;

// VertexInput 단계에서 들어오는 구조체를 정의합니다.
struct VertexInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

// VertexOutput으로 나가는 구조체를 정의합니다.
struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

// VS
VertexOutput VS(VertexInput input)
{
	VertexOutput output;

	// 상수 버퍼의 내용을 적용합니다.
	output.position = mul(input.position, World);
	output.position = mul(output.position, View);
	output.position = mul(output.position, Projection);
	output.uv = input.uv;

	return output;
}

// RS
RasterizerState FillModeWireFrame
{
	FillMode = Wireframe;
};

// SamplerState
// * 복사
SamplerState Sampler0
{
	AddressU = Wrap;
	AddressV = Wrap;
};

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	// 생성한 SmaplerState를 기준으로 텍스처의 해당 uv 좌표에 있는 색상을 꺼내줍니다.
	return Texture0.Sample(Sampler0, input.uv);	
}

// Effect11
technique11 T0
{
	// 기본 모드
	pass P0
	{
		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS()
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	// Wireframe 모드
	pass P1
	{
		SetRasterizerState(FillModeWireFrame);

		// VS 설정 - 버전 5.0, 메인함수 : VS()
		SetVertexShader(CompileShader(vs_5_0, VS()));
		// PS 설정 - 버전 5.0, 메인함수 : PS()
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};