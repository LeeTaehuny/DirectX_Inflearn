#include "pch.h"
#include "GeometryHelper.h"


void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 정점들을 저장하기 위한 벡터 컨테이너를 생성합니다.
	vector<VertexTextureData> vertices;

	// 사이즈 조정
	vertices.resize(4);

	// 위치 정보와 uv 좌표를 설정합니다.
	vertices[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	vertices[0].uv = Vec2(0.0f, 1.0f);	
	vertices[1].position = Vec3(-0.5f, 0.5f, 0.0f);
	vertices[1].uv = Vec2(0.0f, 0.0f);
	vertices[2].position = Vec3(0.5f, -0.5f, 0.0f);
	vertices[2].uv = Vec2(1.0f, 1.0f);
	vertices[3].position = Vec3(0.5f, 0.5f, 0.0f);
	vertices[3].uv = Vec2(1.0f, 0.0f);

	// 정점들의 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };

	// Geometry에 해당 정점들을 저장합니다.
	geometry->SetVertices(vertices);
	// Geometry에 해당 인덱스들을 저장합니다.
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	// 정점들을 저장하기 위한 벡터 컨테이너를 생성합니다.
	vector<VertexColorData> vertices;

	// 사이즈 조정
	vertices.resize(4);

	// 위치 정보와 색상 정보를 설정합니다.
	vertices[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	vertices[0].color = color;
	vertices[1].position = Vec3(-0.5f, 0.5f, 0.0f);
	vertices[1].color = color;
	vertices[2].position = Vec3(0.5f, -0.5f, 0.0f);
	vertices[2].color = color;
	vertices[3].position = Vec3(0.5f, 0.5f, 0.0f);
	vertices[3].color = color;

	// 정점들의 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };

	// Geometry에 해당 정점들을 저장합니다.
	geometry->SetVertices(vertices);
	// Geometry에 해당 인덱스들을 저장합니다.
	geometry->SetIndices(indices);
}
