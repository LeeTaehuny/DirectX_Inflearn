// 외부에서 연결하기 위한 변수들
matrix World;
matrix View;
matrix Projection;
Texture2D Texture0;
// 빛의 방향을 저장하기 위한 변수
float3 LightDir;

// VertexInput 단계에서 들어오는 구조체를 정의합니다.
struct VertexInput
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// VertexOutput으로 나가는 구조체를 정의합니다.
struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
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

	// Normal 좌표는 물체가 이동(회전)하면 변경됩니다.
	// * 물체의 World(회전값만 존재하는)좌표를 적용해줍니다.
	// * 물체의 이동 정보는 행렬의 마지막 줄에 저장되어 있습니다.
	//   -> 회전만 적용하기 위해서는 마지막 줄을 제거하면 됩니다.
	output.normal = mul(input.normal, (float3x3)World);

	return output;
}

// RS
RasterizerState FillModeWireFrame
{
	FillMode = Wireframe;
};

// SamplerState
SamplerState Sampler0;

// PS
float4 PS(VertexOutput input) : SV_TARGET
{
	// 노말 벡터를 입력받은 것으로부터 추출합니다.
	// * 확인을 위해 normalize() 함수를 사용했습니다.
	float3 normal = normalize(input.normal);
	// 내적을 위한 빛의 방향을 재설정해줍니다.
	// * 광원 -> 물체의 표면에서 물체의 표면 -> 광원 방향으로 수정합니다.
	float3 light = -LightDir;

	// 내적을 통해 cos값을 구해줍니다.
	// * A x B x cos(각도)
	//   -> A와 B가 1이므로 cos(각도)의 값이 추출됩니다.
	//   -> 0 ~ 1 사이의 값이 나오며 0도일 때 1, 90도일때 0이 추출됩니다.
	// * 빛이 하얀색(1, 1, 1, 1)에 해당 수치를 곱하면?
	//   -> dot의 결과에 따라 색이 어두워지는 것을 확인할 수 있습니다.
	// * TEST 빛의 색상이 하얀색이도록 설정
	//return float4(1, 1, 1, 1) * dot(light, normal);

	// 생성한 SmaplerState를 기준으로 텍스처의 해당 uv 좌표에 있는 색상을 꺼내줍니다.
	return Texture0.Sample(Sampler0, input.uv) * dot(light, normal);
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