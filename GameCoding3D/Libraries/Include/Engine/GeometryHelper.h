#pragma once
#include "Geometry.h"
#include "VertexData.h"

class GeometryHelper
{
public:
	// VertexColorData 형식을 사용해 사각형을 만들기 위한 함수를 선언합니다.
	static void CreateQuad(shared_ptr<Geometry<VertexColorData>> geometry, Color color);
};

