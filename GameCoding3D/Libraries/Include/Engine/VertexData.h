#pragma once

struct VertexData
{
	Vec3 position = { 0, 0, 0 };
};

struct VertexTextureData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
};

struct VertexColorData
{
	Vec3 position = { 0, 0, 0 };
	Color color = { 0, 0, 0, 0 };
};

struct VertexTextureNormalData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
};

struct VertexTextureNormalTangentData
{
	Vec3 position = { 0, 0, 0 };
	Vec2 uv = { 0, 0 };
	Vec3 normal = { 0, 0, 0 };
	Vec3 tangent = { 0, 0, 0 };
	// Bi-Normal 정보를 넣어줄 수도 있지만 두 벡터를 알면 나머지 하나는 외적을 통해 구해줄 수 있으므로 필수 X
};