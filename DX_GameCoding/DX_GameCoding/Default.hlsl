// VertexShader에 들어오는 구조체를 생성합니다.
struct VS_INPUT
{
	// 생성할 Input Layout을 보면
	// position은 POSITION으로, uv는 TEXCOORD로 읽기로 지정
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

// VertexShader에서 출력되는 구조체를 생성합니다.
struct VS_OUTPUT
{
	// SV : System Value - 무조건 있어야 하는 값
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

// VS_INPUT에서 들어오는 것은 도형에 대한 기하학적인 모습이라고 생각할 수 있습니다.
// * 추가적인 정보를 받아 줄 상수 버퍼를 등록합니다. (b : 버퍼의 약자)
cbuffer TransfromData : register(b0)
{
	float4 offset;
}

// VertexShader의 메인 함수를 정의합니다.
// * VS_OUTPUT : 리턴 타입
// * VS		   : 함수 명
// * (VS_INPUT input) : 입력 매개변수
VS_OUTPUT VS(VS_INPUT input)
{
	// 복잡한 연산 과정이 필요하지만 지금은 간단히 return 시켜줍니다.
	// * 출력 타입의 구조체를 생성합니다.
	VS_OUTPUT output;

	// * 출력할 구조체의 정보를 채워줍니다.
	// ** 위에서 들어온 추가적인 정보도 연산에 추가해줍니다.
	output.position = input.position + offset;
	output.uv = input.uv;

	// * 만들어진 구조체를 반환합니다.
	return output;
}

// 렌더링 파이프라인이 시작되면 IA단계에서 VS로 넘어가게 되는데 이 때 위에서 선언한 메인 함수로 들어온다고 생각하면 됩니다.
// * VS 단계의 메인 함수에서는 무조건 정점 단위로 실행이 됩니다.

// 렌더링 파이프라인
// * IA - VS - RS - PS - OM (핵심단계들)
//  -> 위 단계들은 포맷(Input Layout)에 맞게 기하 도형들이 넘어온다고 생각하면 됩니다.

// IA : 입력
// VS : 정점 단위로 연산
// RS : 지금은 삼각형을 만들거라고 입력했기 때문에 3개의 정점 사이 영역만 인지하고, 아닌 영역은 제거해줍니다.
//      + 각 정점의 색상이 다르면 중앙 위치의 색은 기본적으로 정점들의 색을 섞어서 보간해 결과를 만들어줍니다.
// PS : RS로부터 받은 정보들 토대로 조명(색상)과 관련된 연산을 수행 

// VS(정점) -> RS(처리) -> PS(픽셀)


// t0 레지스터에 texture0을 등록합니다.
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
// s0 레지스터에 sampler0을 등록합니다.
SamplerState sampler0 : register(s0);

// PixelShader의 메인 함수를 정의합니다.
// * (VS_OUTPUT input) : VS 단계에서 리턴해준 output을 입력 파라미터로 전달 받습니다.
// * SV_Target		   : PS의 결과물이 SV_Target(렌더 타겟)에 전달합니다.
float4 PS(VS_OUTPUT input) : SV_Target
{
	// texture0번의 uv좌표에 해당하는 색상 정보를 저장합니다.
	float4 color = texture0.Sample(sampler0, input.uv);

	// uv좌표에 해당하는 색상 정보를 반환합니다.
	return color;
}