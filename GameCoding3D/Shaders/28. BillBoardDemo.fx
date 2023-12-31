#include "00. Global.fx"
#include "00. Light.fx"
#include "00. Render.fx"

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float2 scale : SCALE;
};

struct V_OUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

V_OUT VS(VertexInput input)
{
    V_OUT output;

    // World Space로 넘겨주기
    float4 position = mul(input.position, W);

    // 위 방향 구하기
    float3 up = float3(0, 1, 0);
    // 앞 방향 구하기(카메라 방향)
    //float3 forward = float3(0, 0, 1); // TODO
    float3 forward = position.xyz - CameraPosition(); // BillBoard

    // 위 방향과 앞 방향을 외적해 오른쪽 방향을 구하기
    float3 right = normalize(cross(up, forward));

    // 좌우 위치 수정 (방향에 따라, uv에 따라 사각형 만들기)
    position.xyz += (input.uv.x - 0.5f) * right * input.scale.x;
    // 상하 위치 수정
    position.xyz += (1.0f - input.uv.y - 0.5f) * up * input.scale.y;
    position.w = 1.0f;

    // VP 행렬 적용
    output.position = mul(mul(position, V), P);

    output.uv = input.uv;

    return output;
}

float4 PS(V_OUT input) : SV_Target
{
    float4 diffuse = DiffuseMap.Sample(LinearSampler, input.uv);

    clip(diffuse.a - 0.3f);
    if (diffuse.a < 0.3f)
        discard;

    return diffuse;
}

technique11 T0
{
    pass P0
    {
        //SetRasterizerState(FillModeWireFrame);
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
};