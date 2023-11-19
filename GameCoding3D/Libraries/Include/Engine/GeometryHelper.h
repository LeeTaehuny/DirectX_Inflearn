#pragma once
#include "Geometry.h"
#include "VertexData.h"

class GeometryHelper
{
public:
	// VertexColorData 형식을 사용해 사각형을 만들기 위한 함수를 선언합니다.
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);

	// VertexTextureData 형식을 사용해 도형을 만들기 위한 함수들을 선언합니다.
	// * 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexTextureData>> geometry);
	// * 큐브
	static void CreateCube(shared_ptr<Geometry<VertexTextureData>> geometry);
	// * 구
	static void CreateSphere(shared_ptr<Geometry<VertexTextureData>> geometry);
	// * 그리드
	static void CreateGrid(shared_ptr<Geometry<VertexTextureData>> geometry, int32 sizeX, int32 sizeZ);

	// VertexTextureNormalData 형식을 사용해 도형을 만들기 위한 함수들을 선언합니다.
	// * 사각형
	static void CreateQuad(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// * 큐브
	static void CreateCube(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// * 구
	static void CreateSphere(shared_ptr<Geometry<VertexTextureNormalData>> geometry);
	// * 그리드
	static void CreateGrid(shared_ptr<Geometry<VertexTextureNormalData>> geometry, int32 sizeX, int32 sizeZ);

};

