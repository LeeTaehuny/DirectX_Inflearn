#pragma once

// Bone 정보를 담아줄 구조체를 생성합니다.
struct ModelBone
{
	// 이름
	wstring name;
	// 인덱스
	int32 index;
	// 부모의 인덱스
	int32 parentIndex;
	// Cache 용도
	// * 부모의 인덱스를 안다고 바로 구할 수 없는 구조이기 때문에 한 번 연결시켜주고, 포인터식으로 들고 있기 위함
	shared_ptr<ModelBone> parent;

	// Transform 정보
	Matrix transform;
	// Cache 용도
	vector<shared_ptr<ModelBone>> children;
};

// Mesh 정보를 담아줄 구조체를 생성합니다.
struct ModelMesh
{
	// 버퍼들을 생성하기 위한 함수를 선언합니다.
	void CreateBuffers();

	// 이름
	wstring name;

	// Mesh
	shared_ptr<Geometry<ModelVertexType>> geometry = make_shared<Geometry<ModelVertexType>>();
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;

	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	shared_ptr<ModelBone> bone; // Cache;
};

