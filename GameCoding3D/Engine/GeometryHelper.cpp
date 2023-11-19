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

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<VertexTextureData> vtx;
	// 사이즈를 4(사각형)로 설정합니다.
	vtx.resize(4);

	// 1 3
	// 0 2
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	vtx[0].uv = Vec2(0.0f, 1.0f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.0f);
	vtx[1].uv = Vec2(0.0f, 0.0f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.0f);
	vtx[2].uv = Vec2(1.0f, 1.0f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.0f);
	vtx[3].uv = Vec2(1.0f, 0.0f);

	// geometry에 정점들을 설정합니다.
	geometry->SetVertices(vtx);

	// 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	// * 시계방향 맞춰주기 (기본값 : 시계방향) - 방향이 다르면 컬링에 의해 걸러질 수 있음
	vector<uint32> idx = { 0, 1, 2, 2, 1, 3 };

	// geometry에 인덱스 정보들을 설정합니다.
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// width
	float w2 = 0.5f;
	// height
	float h2 = 0.5f;
	// depth
	float d2 = 0.5f;

	// 24개의 정점을 생성합니다.
	vector<VertexTextureData> vtx(24);

	// 앞면
	{
		vtx[0] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f) };
		vtx[1] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f) };
		vtx[2] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f) };
		vtx[3] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	}
	// 뒷면
	{
		vtx[4] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f) };
		vtx[5] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f) };
		vtx[6] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f) };
		vtx[7] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f) };
	}
	// 윗면
	{
		vtx[8] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f) };
		vtx[9] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
		vtx[10] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
		vtx[11] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f) };
	}
	// 아랫면
	{
		vtx[12] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
		vtx[13] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
		vtx[14] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f) };
		vtx[15] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f) };
	}
	// 왼쪽면
	{
		vtx[16] = VertexTextureData{ Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f) };
		vtx[17] = VertexTextureData{ Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f) };
		vtx[18] = VertexTextureData{ Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f) };
		vtx[19] = VertexTextureData{ Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f) };
	}
	// 오른쪽면
	{
		vtx[20] = VertexTextureData{ Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f) };
		vtx[21] = VertexTextureData{ Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f) };
		vtx[22] = VertexTextureData{ Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f) };
		vtx[23] = VertexTextureData{ Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f) };
	}

	// geometry에 정점들을 설정합니다.
	geometry->SetVertices(vtx);

	// 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> idx(36);

	// 앞면
	{
		idx[0] = 0; idx[1] = 1; idx[2] = 2;
		idx[3] = 0; idx[4] = 2; idx[5] = 3;
	}
	// 뒷면
	{
		idx[6] = 4; idx[7] = 5; idx[8] = 6;
		idx[9] = 4; idx[10] = 6; idx[11] = 7;
	}
	// 윗면
	{
		idx[12] = 8; idx[13] = 9; idx[14] = 10;
		idx[15] = 8; idx[16] = 10; idx[17] = 11;
	}
	// 아랫면
	{
		idx[18] = 12; idx[19] = 13; idx[20] = 14;
		idx[21] = 12; idx[22] = 14; idx[23] = 15;
	}
	// 왼쪽면
	{
		idx[24] = 16; idx[25] = 17; idx[26] = 18;
		idx[27] = 16; idx[28] = 18; idx[29] = 19;
	}
	// 오른쪽면
	{
		idx[30] = 20; idx[31] = 21; idx[32] = 22;
		idx[33] = 20; idx[34] = 22; idx[35] = 23;
	}

	// geometry에 인덱스 정보들을 설정합니다.
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// 옵션(인자로 받기 or 기준 정하기)
	// * 구의 반지름
	float radius = 0.5f;
	// * 가로 분할
	uint32 stackCount = 20;
	// * 세로 분할
	uint32 sliceCount = 20;

	// 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<VertexTextureData> vtx;

	VertexTextureData v;

	// 북극 정점 추가
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	vtx.push_back(v);

	// 가로분할, 세로분할 각도 구하기
	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	// 각도에 따른 기준 uv좌표값 설정
	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 고리마다 돌면서 정점 계산 (북극/남극 단일점은 고리가 아님)
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리에 위치한 정점들을 대상으로 순회하며 해당 정점의 uv좌표 설정
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			vtx.push_back(v);
		}
	}

	// 남극 정점 추가 (마무리)
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	vtx.push_back(v);

	// geometry에 정점들을 설정합니다.
	geometry->SetVertices(vtx);

	// 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> idx(36);

	// 북극 인덱스 연결 (삼각형 1개)
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스 연결 (삼각형 2개)
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스 (삼각형 1개)
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	// geometry에 인덱스 정보들을 설정합니다.
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ)
{
	// 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<VertexTextureData> vtx;

	// 생성할 그리드의 사이즈만큼 정점을 추가합니다.
	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			// 정점 데이터 설정
			VertexTextureData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(z));

			vtx.push_back(v);
		}
	}

	// geometry에 정점들을 설정합니다.
	geometry->SetVertices(vtx);

	// 인덱스 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> idx;

	// 생성할 그리드의 사이즈만큼 인덱스를 생성합니다.
	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	// geometry에 인덱스 정보들을 설정합니다.
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	vector<VertexTextureNormalData> vtx;
	vtx.resize(4);

	// Quad의 Normal Vector는 간단히 Look Vector의 반대 방향(-1.0f)으로 생성할 수 있습니다.
	vtx[0].position = Vec3(-0.5f, -0.5f, 0.f);
	vtx[0].uv = Vec2(0.f, 1.f);
	vtx[0].normal = Vec3(0.f, 0.f, -1.f);
	vtx[1].position = Vec3(-0.5f, 0.5f, 0.f);
	vtx[1].uv = Vec2(0.f, 0.f);
	vtx[1].normal = Vec3(0.f, 0.f, -1.f);
	vtx[2].position = Vec3(0.5f, -0.5f, 0.f);
	vtx[2].uv = Vec2(1.f, 1.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	vtx[3].position = Vec3(0.5f, 0.5f, 0.f);
	vtx[3].uv = Vec2(1.f, 0.f);
	vtx[2].normal = Vec3(0.f, 0.f, -1.f);
	geometry->SetVertices(vtx);

	vector<uint32> idx = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(idx);
}

void GeometryHelper::CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;

	vector<VertexTextureNormalData> vtx(24);

	// 앞면
	vtx[0] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[1] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[2] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
	vtx[3] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
	// 뒷면
	vtx[4] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[5] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[6] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	vtx[7] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
	// 윗면
	vtx[8] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[9] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[10] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	vtx[11] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f));
	// 아랫면
	vtx[12] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[13] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[14] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	vtx[15] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f));
	// 왼쪽면
	vtx[16] = VertexTextureNormalData(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[17] = VertexTextureNormalData(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[18] = VertexTextureNormalData(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
	vtx[19] = VertexTextureNormalData(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
	// 오른쪽면
	vtx[20] = VertexTextureNormalData(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[21] = VertexTextureNormalData(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[22] = VertexTextureNormalData(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
	vtx[23] = VertexTextureNormalData(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f));

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// 앞면
	idx[0] = 0; idx[1] = 1; idx[2] = 2;
	idx[3] = 0; idx[4] = 2; idx[5] = 3;
	// 뒷면
	idx[6] = 4; idx[7] = 5; idx[8] = 6;
	idx[9] = 4; idx[10] = 6; idx[11] = 7;
	// 윗면
	idx[12] = 8; idx[13] = 9; idx[14] = 10;
	idx[15] = 8; idx[16] = 10; idx[17] = 11;
	// 아랫면
	idx[18] = 12; idx[19] = 13; idx[20] = 14;
	idx[21] = 12; idx[22] = 14; idx[23] = 15;
	// 왼쪽면
	idx[24] = 16; idx[25] = 17; idx[26] = 18;
	idx[27] = 16; idx[28] = 18; idx[29] = 19;
	// 오른쪽면
	idx[30] = 20; idx[31] = 21; idx[32] = 22;
	idx[33] = 20; idx[34] = 22; idx[35] = 23;

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry)
{
	// 구의 반지름
	float radius = 0.5f;
	// 가로 분할
	uint32 stackCount = 20;
	// 세로 분할
	uint32 sliceCount = 20;

	vector<VertexTextureNormalData> vtx;

	VertexTextureNormalData v;

	/** 구의 경우에는 원점부터 해당 정점까지의 벡터를 단위벡터로 만든 것이 Normal Vector입니다. */
	// 즉, 기준점의 위치는 0, 0, 0 이므로 정점의 위치를 Normalize하면 Normal Vector가 생성됩니다.

	// 북극점 추가
	v.position = Vec3(0.0f, radius, 0.0f);
	v.uv = Vec2(0.5f, 0.0f);
	v.normal = v.position;
	v.normal.Normalize();
	vtx.push_back(v);

	float stackAngle = XM_PI / stackCount;
	float sliceAngle = XM_2PI / sliceCount;

	float deltaU = 1.f / static_cast<float>(sliceCount);
	float deltaV = 1.f / static_cast<float>(stackCount);

	// 모든 고리를 순회하며 정점을 계산
	for (uint32 y = 1; y <= stackCount - 1; ++y)
	{
		float phi = y * stackAngle;

		// 고리마다 나눠질 정점을 계산
		for (uint32 x = 0; x <= sliceCount; ++x)
		{
			float theta = x * sliceAngle;

			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			v.uv = Vec2(deltaU * x, deltaV * y);

			v.normal = v.position;
			v.normal.Normalize();

			vtx.push_back(v);
		}
	}

	// 남극점 추가
	v.position = Vec3(0.0f, -radius, 0.0f);
	v.uv = Vec2(0.5f, 1.0f);
	v.normal = v.position;
	v.normal.Normalize();
	vtx.push_back(v);

	geometry->SetVertices(vtx);

	vector<uint32> idx(36);

	// 북극 인덱스
	for (uint32 i = 0; i <= sliceCount; ++i)
	{
		//  [0]
		//   |  \
		//  [i+1]-[i+2]
		idx.push_back(0);
		idx.push_back(i + 2);
		idx.push_back(i + 1);
	}

	// 몸통 인덱스
	uint32 ringVertexCount = sliceCount + 1;
	for (uint32 y = 0; y < stackCount - 2; ++y)
	{
		for (uint32 x = 0; x < sliceCount; ++x)
		{
			//  [y, x]-[y, x+1]
			//  |		/
			//  [y+1, x]
			idx.push_back(1 + (y)*ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			//		 [y, x+1]
			//		 /	  |
			//  [y+1, x]-[y+1, x+1]
			idx.push_back(1 + (y + 1) * ringVertexCount + (x));
			idx.push_back(1 + (y)*ringVertexCount + (x + 1));
			idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
		}
	}

	// 남극 인덱스
	uint32 bottomIndex = static_cast<uint32>(vtx.size()) - 1;
	uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
	for (uint32 i = 0; i < sliceCount; ++i)
	{
		//  [last+i]-[last+i+1]
		//  |      /
		//  [bottom]
		idx.push_back(bottomIndex);
		idx.push_back(lastRingStartIndex + i);
		idx.push_back(lastRingStartIndex + i + 1);
	}

	geometry->SetIndices(idx);
}

void GeometryHelper::CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX, int32 sizeZ)
{
	vector<VertexTextureNormalData> vtx;

	for (int32 z = 0; z < sizeZ + 1; z++)
	{
		for (int32 x = 0; x < sizeX + 1; x++)
		{
			VertexTextureNormalData v;
			v.position = Vec3(static_cast<float>(x), 0, static_cast<float>(z));
			v.uv = Vec2(static_cast<float>(x), static_cast<float>(sizeZ - z));
			v.normal = Vec3(0.f, 1.f, 0.f);

			vtx.push_back(v);
		}
	}

	geometry->SetVertices(vtx);

	vector<uint32> idx;

	for (int32 z = 0; z < sizeZ; z++)
	{
		for (int32 x = 0; x < sizeX; x++)
		{
			//  [0]
			//   |	\
			//  [2] - [1]
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z)+(x));
			//  [1] - [2]
			//   	\  |
			//		  [0]
			idx.push_back((sizeX + 1) * (z)+(x + 1));
			idx.push_back((sizeX + 1) * (z + 1) + (x));
			idx.push_back((sizeX + 1) * (z + 1) + (x + 1));
		}
	}

	geometry->SetIndices(idx);
}
