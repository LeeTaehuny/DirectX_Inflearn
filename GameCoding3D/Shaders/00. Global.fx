// pragma once와 동일한 역할
#ifndef _GLOBAL_FX_
#define _GLOBAL_FX_

///////////////////
/// ConstBuffer ///
///////////////////

// WVP 정보들을 모든 셰이더에서 사용할 수 있도록 상수 버퍼를 분리해 생성해주도록 하겠습니다.
// * VP
cbuffer GlobalBuffer
{
	// View, Projection
	matrix V;
	matrix P;
	// V x P와 VP는 동일하므로 연산한 결과를 묶어서 같이 넘겨주도록 합니다.
	matrix VP;
	// View Inverse
	matrix VInv;
};

// * W
cbuffer TransformBuffer
{
	// World 
	// * World 정보는 오브젝트별로 다르기 때문에 매 번 갱신이 필요합니다.
	// * VP와 다른 상수 버퍼에 저장해주도록 합니다.
	matrix W;
};

//////////////////
/// VertexData ///
//////////////////

// 정점 정보들은 여러 가지가 존재하며, 각각의 버전마다 관리하기 편하도록 전역 공간에서 구조체를 선언해주도록 하겠습니다.
// * 일반
struct Vertex
{
	float4 position : POSITION;
};

// * 색상
struct VertexColor
{
	float4 position : POSITION;
	float4 color : COLOR;
};

// * 텍스처
struct VertexTexture
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
};

// * 텍스처 노말
struct VertexTextureNormal
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// * 텍스처 노말 탄젠트
struct VertexTextureNormalTangent
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

// * 애니메이션 용도
struct VertexTextureNormalTangentBlend
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

	float4 blendIndices : BLEND_INDICES;
	float4 blendWeights : BLEND_WEIGHTS;
};

////////////////////
/// VertexOutput ///
////////////////////

// VS의 결과물로 나올 구조체를 미리 용도별로 구분해 선언해주도록 하겠습니다.
// * 일반
struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// * Mesh
struct MeshOutput
{
	float4 position : SV_POSITION;
	// 조명연산에 필요한 월드 좌표를 저장하기 위한 변수
	float3 worldPosition : POSITION1;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	// Tangent Space
	float3 tangent : TANGENT;
};

////////////////////
/// SamplerState ///
////////////////////

// 자주 사용할 Sampling 규칙들을 미리 선언해줍니다.
// * Linear
SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

// * Point
SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

///////////////////////
/// RasterizerState ///
///////////////////////

// 자주 사용할 RasterizerState를 미리 선언해주도록 합니다.
RasterizerState FillModeWireFrame
{
	FillMode = WireFrame;
};

RasterizerState FrontCounterClockwiseTrue
{
	FrontCounterClockwise = true;
};

////////////////
// BlendState //
////////////////

BlendState AlphaBlend
{
	AlphaToCoverageEnable = false;

	BlendEnable[0] = true;
	SrcBlend[0] = SRC_ALPHA;
	DestBlend[0] = INV_SRC_ALPHA;
	BlendOp[0] = ADD;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = Zero;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

BlendState AlphaBlendAlphaToCoverageEnable
{
	AlphaToCoverageEnable = true;

	BlendEnable[0] = true;
	SrcBlend[0] = SRC_ALPHA;
	DestBlend[0] = INV_SRC_ALPHA;
	BlendOp[0] = ADD;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = Zero;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

BlendState AdditiveBlend
{
	AlphaToCoverageEnable = true;

	BlendEnable[0] = true;
	SrcBlend[0] = One;
	DestBlend[0] = One;
	BlendOp[0] = ADD;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = Zero;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

BlendState AdditiveBlendAlphaToCoverageEnable
{
	AlphaToCoverageEnable = true;

	BlendEnable[0] = true;
	SrcBlend[0] = One;
	DestBlend[0] = One;
	BlendOp[0] = ADD;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = Zero;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

/////////////
/// Macro ///
/////////////

// technique 및 pass로 구분하는 것들을 매크로로 선언해 반복적인 작업을 줄여줍니다.
#define PASS_VP(name, vs, ps)						\
pass name											\
{													\
	SetVertexShader(CompileShader(vs_5_0, vs()));	\
	SetPixelShader(CompileShader(ps_5_0, ps()));	\
}													\

#define PASS_RS_VP(name, rs, vs, ps)				\
pass name											\
{													\
	SetRasterizerState(rs);							\
	SetVertexShader(CompileShader(vs_5_0, vs()));	\
	SetPixelShader(CompileShader(ps_5_0, ps()));	\
}													\

#define PASS_BS_VP(name, bs, vs, ps)				\
pass name											\
{													\
	SetBlendState(bs, float4(0, 0, 0, 0), 0xFF);	\
    SetVertexShader(CompileShader(vs_5_0, vs()));	\
    SetPixelShader(CompileShader(ps_5_0, ps()));	\
}													\

////////////////
/// Function ///
////////////////

float3 CameraPosition()
{
	return VInv._41_42_43;
}

#endif