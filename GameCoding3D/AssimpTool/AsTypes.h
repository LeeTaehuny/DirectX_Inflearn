#pragma once

// 메모리에 들고 있는 데이터를 우리만의 데이터로 변경해주기 위한 구조체를 가지는 헤더입니다.

// VertexType (VertexTextureNormalTangentData)
using VertexType = VertexTextureNormalTangentData;

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
