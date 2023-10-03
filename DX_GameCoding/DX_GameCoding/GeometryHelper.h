#pragma once

#include "Geometry.h"
#include "VertexData.h"

// 기본 도형들을 만들어주는 간단한 헬퍼 클래스입니다.
class GeometryHelper
{
public:
	static void CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry);
	static void CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color);
};

