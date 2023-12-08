#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500

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
	TweenFrameDesc TweenFrames;
};

// 모델의 뼈 정보들을 저장하기 위한 상수버퍼를 정의합니다.
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

// 현재 렌더링하는 물체의 Bone Index를 관리하기 위한 전역 버퍼를 생성합니다.
uint BoneIndex;
// 이전에 생성한 애니메이션과 관련된 정보가 들어있는 텍스처를 저장하기 위한 변수를 선언합니다.
Texture2DArray TransformMap;

// 애니메이션의 transform 정보를 적용시키기 위한 함수를 정의합니다. (보간 O)
matrix GetAnimationMatrix(VertexTextureNormalTangentBlend input)
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
	animIndex[0] = TweenFrames.curr.animIndex;
	currFrame[0] = TweenFrames.curr.currFrame;
	nextFrame[0] = TweenFrames.curr.nextFrame;
	ratio[0] = TweenFrames.curr.ratio;

	animIndex[1] = TweenFrames.next.animIndex;
	currFrame[1] = TweenFrames.next.currFrame;
	nextFrame[1] = TweenFrames.next.nextFrame;
	ratio[1] = TweenFrames.next.ratio;

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
		n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex	[0], 0));
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
			result = lerp(result, nextResult, TweenFrames.tweenRatio);
		}

		// 최종적으로 가중치를 행렬에 적용해 더해줍니다.
		transform += mul(weights[i], result);
	}
	// -> 1바퀴 순회하면 한 정점의 Trnsform 정보가 나오게 됩니다. (가중치 값 고려한 정보)

	return transform;
}


// VS
MeshOutput VS(VertexTextureNormalTangentBlend input)
{
	MeshOutput output;

	Matrix m = GetAnimationMatrix(input);
	output.position = mul(input.position, m);

	output.position = mul(output.position, W);
	output.worldPosition = output.position.xyz;
	output.position = mul(output.position, VP);
	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
	output.tangent = mul(input.tangent, (float3x3)W);

	return output;
}

// PS
float4 PS(MeshOutput input) : SV_TARGET
{
	float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

	return color;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

// Effect11
technique11 T0
{
	// 기본 모드
	PASS_VP(P0, VS, PS)
	PASS_RS_VP(P1, FillModeWireFrame, VS, PS_RED)
};