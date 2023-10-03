#pragma once

// 기존 Struct.h 파일에서 관리하고 있던 정점의 정보를 저장하기 위한 클래스입니다.

// 정점에 대한 정보를 저장하기 위한 구조체를 선언합니다. (텍스처)
struct VertexTextureData
{
	// 위치 좌표 (x, y, z)
	Vec3 position = { 0, 0, 0 };
	// 텍스처 좌표 (u, v)
	Vec2 uv = { 0, 0 };

	// InputLayout과 Vertex 정보는 1 : 1로 매핑되어야 합니다.
	// * 해당 Vertex 정보를 저장하는 배열을 static으로 선언합니다.
	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

// 정점에 대한 정보를 저장하기 위한 구조체를 선언합니다. (색상)
struct VertexColorData
{
	// 위치 좌표 (x, y, z)
	Vec3 position = { 0, 0, 0 };
	// 색상 정보	(r, g, b, a)
	Color color = { 0, 0, 0, 0 };

	// InputLayout과 Vertex 정보는 1 : 1로 매핑되어야 합니다.
	// * 해당 Vertex 정보를 저장하는 배열을 static으로 선언합니다.
	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};