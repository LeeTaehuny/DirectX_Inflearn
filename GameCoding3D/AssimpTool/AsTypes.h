#pragma once

// 메모리에 들고 있는 데이터를 우리만의 데이터로 변경해주기 위한 구조체를 가지는 헤더입니다.

using VertexType = VertexTextureNormalTangentBlendData;

// 우리가 사용하기 위한 구조체 생성
struct asBone
{
	// 이름
	string name;
	// 뼈의 순서
	int32 index = -1;
	// 부모의 순서
	int32 parent = -1;
	
	// SRT 정보
	Matrix transform;
};

struct asMesh
{
	// 이름
	string name;
	// Assimp에서 로드한 결과물(Mesh)
	aiMesh* mesh;
	// 정점 정보
	vector<VertexType> vertices;
	// 인덱스 정보
	vector<uint32> indices;

	// Mesh와 연결된 bone, material 정보를 연결하기 위한 정보
	int32 boneIndex;
	string materialName;
};

struct asMaterial
{
	// 이름
	string name;
	// 색상
	Color ambient;
	Color diffuse;
	Color specular;
	Color emissive;
	// 이미지 파일 경로
	string diffuseFile;
	string specularFile;
	string normalFile;
};

// Animation
struct asBlendWeight
{
	Vec4 indices = Vec4(0, 0, 0, 0);
	Vec4 weights = Vec4(0, 0, 0, 0);

	// 위 변수에 값을 세팅하기 위한 함수를 정의합니다.
	void Set(uint32 index, uint32 boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
			case 0: indices.x = i; weights.x = w; break;
			case 1: indices.y = i; weights.y = w; break;
			case 2: indices.z = i; weights.z = w; break;
			case 3: indices.w = i; weights.w = w; break;
		}
	}
};

struct asBoneWeights
{
	void AddWeights(uint32 boneIndex, float weight)
	{
		// 예외처리
		if (weight <= 0.0f) return;

		// 가중치가 높은 Bone을 앞으로 오게 배치해줍니다.
		auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
			[weight](const Pair& p) { return weight > p.second; });

		boneWeights.insert(findIt, Pair(boneIndex, weight));
	}

	// 뼈의 모든 가중치의 합이 1이 되도록 하기 위한 연산을 진행해주는 함수입니다.
	void Normalize()
	{
		// 최대 4개의 뼈와 연결할 것이므로 최대를 4로 지정합니다.
		if (boneWeights.size() >= 4)
			boneWeights.resize(4);

		// 최종적인 가중치의 합을 저장하기 위한 변수를 선언합니다.
		float totalWeight = 0.0f;
		// 모든 가중치 정보를 순회하며 합을 구해줍니다.
		for (const auto& item : boneWeights)
		{
			totalWeight += item.second;
		}

		// 모든 합은 1이 되어야 하므로 모든 가중치의 합을 원본 가중치에 나눠 비율을 맞춰줍니다.
		for (auto& item : boneWeights)
		{
			item.second /= totalWeight;
		}
	}

	// 최종적인 정보는 float4 형태이므로 asBlendWeight로 변환하기 위한 함수를 정의합니다.
	asBlendWeight GetBlendWeights()
	{
		asBlendWeight blendWeights;

		for (uint32 i = 0; i < boneWeights.size(); i++)
		{
			// 예외처리
			if (i >= 4) break;

			blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
		}

		return blendWeights;
	}

	using Pair = pair<int32, float>;
	vector<Pair> boneWeights;
};

// 키 프레임 데이터를 저장하기 위한 구조체를 생성합니다.
struct asKeyframeData
{
	// 시간
	float time;
	
	// SRT
	Vec3 scale;
	Quaternion rotation;
	Vec3 translation;
};

// 하나의 프레임을 저장하기 위한 구조체를 생성합니다.
struct asKeyframe
{
	// Bone 이름
	string boneName;
	
	// 변환 행렬
	vector<asKeyframeData> transforms;
};

// 하나의 애니메이션을 저장하기 위한 구조체를 생성합니다.
struct asAnimation
{
	// 이름
	string name;
	// 프레임 수
	uint32 frameCount;
	// 프레임 재생율 (ex. 30 -> 1 / 30초마다 다음 프레임으로 넘기기)
	float frameRate;
	// 재생 시간
	float duration;

	// 재생할 프레임
	vector<shared_ptr<asKeyframe>> keyframes;
};

// 애니메이션 : 프레임마다 모든 뼈(bone)가 어떻게 변화(transform)하는지에 대한 정보를 가지고 있습니다.
// -> 즉, 1프레임을 재생 중이라면 모든 bone들을 각각의 finalMatrix(transform 적용)을 적용시켜 변화시켜줍니다.
// -> 스키닝 기법을 사용했으므로 해당 bone들과 연결된 Vertex 또한 매 프레임 변화합니다.

// Cache
struct asAnimationNode
{
	aiString name;
	vector<asKeyframeData> keyframe;
};