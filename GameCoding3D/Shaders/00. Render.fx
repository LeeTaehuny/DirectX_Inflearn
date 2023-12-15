// pragma once와 동일한 역할
#ifndef _RENDER_FX_
#define _RENDER_FX_

#include "00. Global.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

/*********************** Mesh Render *********************/

struct VertexMesh
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

	// INSTANCING
	uint instanceID : SV_INSTANCEID;
	matrix world :INST;
};

// VS
MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;

	//output.position = mul(input.position, W);
	output.position = mul(input.position, input.world); // W
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);

	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

/*********************** Model Render *********************/

struct VertexModel
{
	float4 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

	// Animation
	float4 blendIndices : BLEND_INDICES;
	float4 blendWeights : BLEND_WEIGHTS;

	// INSTANCING
	uint instanceID : SV_INSTANCEID;
	matrix world :INST;
};

cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

uint BoneIndex;

// VS
MeshOutput VS_Model(VertexModel input)
{
	MeshOutput output;

	// Model의 여러 계층구조의 정점들을 root 기준으로 하는 정점으로 변환(root의 로컬)
	output.position = mul(input.position, BoneTransforms[BoneIndex]);

	output.position = mul(output.position, input.world); // W
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);

	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

/*********************** Anim Render *********************/

// 키 프레임 정보를 관리하기 위한 구조체를 생성합니다.
struct KeyframeDesc
{
	int animIndex;
	uint currFrame;
	uint nextFrame;
	float ratio;
	float sumTime;
	float speed;
	float2 padding;
};

// 두가지 애니메이션을 섞어 관리하기 위한 구조체를 생성합니다.
struct TweenFrameDesc
{
	float tweenDuration;
	float tweenRatio;
	float tweenSumtime;
	float padding;

	KeyframeDesc curr;
	KeyframeDesc next;
};

// 두가지 키 프레임 정보를 저장하기 위한 상수버퍼를 정의합니다.
cbuffer TweenBuffer
{
	TweenFrameDesc TweenFrames[MAX_MODEL_INSTANCE];
};

// 이전에 생성한 애니메이션과 관련된 정보가 들어있는 텍스처를 저장하기 위한 변수를 선언합니다.
Texture2DArray TransformMap;


// 애니메이션의 transform 정보를 적용시키기 위한 함수를 정의합니다. (보간 O)
matrix GetAnimationMatrix(VertexModel input)
{
	// 정점으로부터 index, weight 정보들을 우선 추출합니다.
	float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
	float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

	// Keyframe에서 현재 값을 추출해 저장하기 위한 배열을 선언합니다.
	// * 애니메이션이 1개 -> 2개로 늘어났습니다.
	// * 애니메이션 사이 보간 & 애니메이션 끼리 보간을 하기 위해 배열로 선언합니다.
	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2]; // 비율

	// 각각의 애니메이션에 대한 정보를 채워줍니다.
	animIndex[0] = TweenFrames[input.instanceID].curr.animIndex;
	currFrame[0] = TweenFrames[input.instanceID].curr.currFrame;
	nextFrame[0] = TweenFrames[input.instanceID].curr.nextFrame;
	ratio[0] = TweenFrames[input.instanceID].curr.ratio;

	animIndex[1] = TweenFrames[input.instanceID].next.animIndex;
	currFrame[1] = TweenFrames[input.instanceID].next.currFrame;
	nextFrame[1] = TweenFrames[input.instanceID].next.nextFrame;
	ratio[1] = TweenFrames[input.instanceID].next.ratio;

	// TransformMap에서 4개의 정보를 추출합니다.
	// * 저장할 때 4칸을 기준으로 1개의 행렬 정보가 생성되기 때문입니다.
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;

	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		// 현재 4개의 정보를 추출합니다. (16byte)
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));

		// 추출한 4개의 정보를 토대로 하나의 행렬을 만들어줍니다. (64byte)
		curr = matrix(c0, c1, c2, c3);

		// 다음 프레임 4개의 정보를 추출합니다. (16byte)
		n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));

		// 추출한 4개의 정보를 토대로 하나의 행렬을 만들어줍니다. (64byte)
		next = matrix(n0, n1, n2, n3);

		// 보간 행렬 생성
		matrix result = lerp(curr, next, ratio[0]);

		// 애니메이션 2개를 섞어야 하므로 다음 애니메이션이 존재하는지 체크합니다.
		if (animIndex[1] >= 0)
		{
			c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
			c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
			c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
			c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
			curr = matrix(c0, c1, c2, c3);

			n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
			n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
			n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
			n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
			next = matrix(n0, n1, n2, n3);

			// 보간 행렬 생성
			matrix nextResult = lerp(curr, next, ratio[1]);

			// 이전 애니메이션과 현재 애니메이션을 섞어주기(Blend)
			result = lerp(result, nextResult, TweenFrames[input.instanceID].tweenRatio);
		}

		// 최종적으로 가중치를 행렬에 적용해 더해줍니다.
		transform += mul(weights[i], result);
	}
	// -> 1바퀴 순회하면 한 정점의 Trnsform 정보가 나오게 됩니다. (가중치 값 고려한 정보)

	return transform;
}

// VS
MeshOutput VS_Animation(VertexModel input)
{
	MeshOutput output;

	Matrix m = GetAnimationMatrix(input);
	output.position = mul(input.position, m);

	output.position = mul(output.position, input.world); // W
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);

	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)input.world);
	output.tangent = mul(input.tangent, (float3x3)input.world);

	return output;
}

#endif