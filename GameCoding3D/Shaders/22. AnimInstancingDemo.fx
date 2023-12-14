#include "00. Global.fx"
#include "00. Light.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 500
#define MAX_MODEL_INSTANCE 500

// Ű ������ ������ �����ϱ� ���� ����ü�� �����մϴ�.
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

// �ΰ��� �ִϸ��̼��� ���� �����ϱ� ���� ����ü�� �����մϴ�.
struct TweenFrameDesc
{
	float tweenDuration;
	float tweenRatio;
	float tweenSumtime;
	float padding;

	KeyframeDesc curr;
	KeyframeDesc next;
};

// �ΰ��� Ű ������ ������ �����ϱ� ���� ������۸� �����մϴ�.
cbuffer TweenBuffer
{
	TweenFrameDesc TweenFrames[MAX_MODEL_INSTANCE];
};

// ���� �� �������� �����ϱ� ���� ������۸� �����մϴ�.
cbuffer BoneBuffer
{
	matrix BoneTransforms[MAX_MODEL_TRANSFORMS];
};

// ���� �������ϴ� ��ü�� Bone Index�� �����ϱ� ���� ���� ���۸� �����մϴ�.
uint BoneIndex;
// ������ ������ �ִϸ��̼ǰ� ���õ� ������ ����ִ� �ؽ�ó�� �����ϱ� ���� ������ �����մϴ�.
Texture2DArray TransformMap;

struct VS_IN
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

struct VS_OUT
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION1;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

// �ִϸ��̼��� transform ������ �����Ű�� ���� �Լ��� �����մϴ�. (���� O)
matrix GetAnimationMatrix(VS_IN input)
{
	// �������κ��� index, weight �������� �켱 �����մϴ�.
	float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
	float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };

	// Keyframe���� ���� ���� ������ �����ϱ� ���� �迭�� �����մϴ�.
	// * �ִϸ��̼��� 1�� -> 2���� �þ���ϴ�.
	// * �ִϸ��̼� ���� ���� & �ִϸ��̼� ���� ������ �ϱ� ���� �迭�� �����մϴ�.
	int animIndex[2];
	int currFrame[2];
	int nextFrame[2];
	float ratio[2]; // ����

	// ������ �ִϸ��̼ǿ� ���� ������ ä���ݴϴ�.
	animIndex[0] = TweenFrames[input.instanceID].curr.animIndex;
	currFrame[0] = TweenFrames[input.instanceID].curr.currFrame;
	nextFrame[0] = TweenFrames[input.instanceID].curr.nextFrame;
	ratio[0] = TweenFrames[input.instanceID].curr.ratio;

	animIndex[1] = TweenFrames[input.instanceID].next.animIndex;
	currFrame[1] = TweenFrames[input.instanceID].next.currFrame;
	nextFrame[1] = TweenFrames[input.instanceID].next.nextFrame;
	ratio[1] = TweenFrames[input.instanceID].next.ratio;

	// TransformMap���� 4���� ������ �����մϴ�.
	// * ������ �� 4ĭ�� �������� 1���� ��� ������ �����Ǳ� �����Դϴ�.
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;

	matrix curr = 0;
	matrix next = 0;
	matrix transform = 0;

	for (int i = 0; i < 4; i++)
	{
		// ���� 4���� ������ �����մϴ�. (16byte)
		c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
		c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
		c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
		c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));

		// ������ 4���� ������ ���� �ϳ��� ����� ������ݴϴ�. (64byte)
		curr = matrix(c0, c1, c2, c3);

		// ���� ������ 4���� ������ �����մϴ�. (16byte)
		n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
		n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
		n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
		n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));

		// ������ 4���� ������ ���� �ϳ��� ����� ������ݴϴ�. (64byte)
		next = matrix(n0, n1, n2, n3);

		// ���� ��� ����
		matrix result = lerp(curr, next, ratio[0]);

		// �ִϸ��̼� 2���� ����� �ϹǷ� ���� �ִϸ��̼��� �����ϴ��� üũ�մϴ�.
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

			// ���� ��� ����
			matrix nextResult = lerp(curr, next, ratio[1]);

			// ���� �ִϸ��̼ǰ� ���� �ִϸ��̼��� �����ֱ�(Blend)
			result = lerp(result, nextResult, TweenFrames[input.instanceID].tweenRatio);
		}

		// ���������� ����ġ�� ��Ŀ� ������ �����ݴϴ�.
		transform += mul(weights[i], result);
	}
	// -> 1���� ��ȸ�ϸ� �� ������ Trnsform ������ ������ �˴ϴ�. (����ġ �� ����� ����)

	return transform;
}

// VS
VS_OUT VS(VS_IN input)
{
	VS_OUT output;

	Matrix m = GetAnimationMatrix(input);
	output.position = mul(input.position, m);

	output.position = mul(output.position, input.world); // W
	output.worldPosition = output.position;
	output.position = mul(output.position, VP);

	output.uv = input.uv;
	output.normal = input.normal;

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
	PASS_VP(P0, VS, PS)
};