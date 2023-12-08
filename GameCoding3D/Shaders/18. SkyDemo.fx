#include "00. Global.fx"
#include "00. Light.fx"

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

// VS
VS_OUT VS(VertexTextureNormalTangent input)
{
	VS_OUT output;

	// Local -> World -> View -> Projection 단계를 거쳐 정점을 이동시켜 출력했습니다.
	// * SkyBox는 카메라와 항상 같이 움직여야 합니다.
	//   -> 즉, SkyBox의 변환 행렬을 굳이 World -> View로 진행시키는 것이 아닌 그대로 cilpPos로 보내면 카메라와 항상 같이 있다고 판별할 것입니다. (원점)
	//   -> 하지만, 카메라가 회전할 때에 따라 우리가 SkyBox를 바라보는 각도를 변경해줘야 하기 때문에 회전값은 적용시켜야 합니다.
	//   -> 즉, 4x4 Matrix의 마지막 요소를 0으로 설정해 회전값만을 받아오도록 설정합니다. (translation X, Rotation O)
	//      (float4(input.position.xyz, 0)으로 설정하면 이동하지 않지만 SkyBox 회전만 받아오겠다는 의미입니다.)

	float4 viewPos = mul(float4(input.position.xyz, 0), V);
	float4 clipPos = mul(viewPos, P);

	// Projection 행렬인 clipPos를 그대로 적용하는 것이 아닌 x와 y는 [-1 ~ 1] 사이의 값을 갖도록, z는 [0 ~ 1] 사이의 값을 갖도록 보정해줘야 합니다.
	// * 지금까지는 Rasterizer에서 w에 z값을 보존해 나눠주어 계산하였습니다.
	//   -> (x/w, y/w, z/w, 1)
	// * 여기서 clipPos의 z를 w로 설정한다면?
	//   -> (x/w, y/w, w/w, w/w)이 됩니다.
	//   -> 즉, 무슨 일이 있어도 z는 1이 되며, 가장 뒤쪽에 있다고 판별하게 될 것입니다.
	//   -> 우리는 1이면 출력을 안하는 것으로 설정해둬서.. 1과 가까운 0.9999...를 넣어주면 가장 뒤로 출력해줄 것입니다.
	//      *** 물체가 SkyBox 뒤에 있어도... 앞에 있는 것으로 인식 ***

	output.position = clipPos.xyzw;
	output.position.z = output.position.w * 0.999999f;

	output.uv = input.uv;

	return output;
}

// PS
float4 PS(VS_OUT input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

// Effect11
technique11 T0
{
	pass P0
	{
		SetRasterizerState(FrontCounterClockwiseTrue);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};