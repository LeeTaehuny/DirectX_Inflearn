#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	// 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<VertexColorData> vtx;
	// 사이즈를 4(사각형)로 설정합니다.
	vtx.resize(4);

	// 1 3
	// 0 2
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	vtx[0].color = color;
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.0f);
	vtx[1].color = color;
	vtx[2].position = Vec3(0.5f, -0.5f, 0.0f);
	vtx[2].color = color;
	vtx[3].position = Vec3(0.5f, 0.5f, 0.0f);
	vtx[3].color = color;

	// geometry에 정점들을 설정합니다.
	geometry->SetVertices(vtx);

	// 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	// * 시계방향 맞춰주기 (기본값 : 시계방향) - 방향이 다르면 컬링에 의해 걸러질 수 있음
	vector<uint32> idx = { 0, 1, 2, 2, 1, 3 };

	// geometry에 인덱스 정보들을 설정합니다.
	geometry->SetIndices(idx);
}
