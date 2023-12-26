#pragma once

/*********************/
/****** Point3D ******/
/*********************/
using Point3D = Vec3;

/*********************/
/****** Line3D *******/
/*********************/
struct Line3D
{
	// 시작점, 끝 점
	Point3D start = Point3D(0.0f);
	Point3D end = Point3D(0.0f);

	// 선분의 길이
	float Length() { return Vec3::Distance(start, end); }
	// 선분의 길이 제곱
	float LengthSq() { return Vec3::DistanceSquared(start, end); }
};

/*********************/
/******* Ray3D *******/
/*********************/
struct Ray3D
{
	// 시작점
	Point3D origin = Point3D(0.0f);
	// 방향
	Vec3 direction = Vec3(0.0f);

	// 방향 벡터(정규화)
	void NormalizeDirection() { direction.Normalize(); }
	// Ray 생성 함수
	static Ray3D FromPoints(const Point3D& from, const Point3D& to) { return Ray3D(from, to - from); }
};

/*********************/
/***** Sphere3D ******/
/*********************/
struct Sphere3D
{
	// 위치, 반지름
	Point3D position = Vec3(0.0f);
	float radius = 1.0f;
};

/*********************/
/****** AABB3D *******/
/*********************/
struct AABB3D
{
	// 위치
	Point3D position = Vec3(0.0f);
	// half size
	Vec3 size = Vec3(1.0f);

	// 정보를 전달받아 AABB3D 도형을 만들어주는 헬퍼 함수
	static AABB3D FromMinMax(const Vec3& min, const Vec3& max)
	{
		return AABB3D((min + max) / 2, (max - min) / 2);
	}

	// 사각형을 구성하는 꼭짓점 중 가장 작은(위치) 점을 구해주기 위한 헬퍼 함수
	static Vec3 GetMin(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;

		return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	// 사각형을 구성하는 꼭짓점 중 가장 큰(위치) 점을 구해주기 위한 헬퍼 함수
	static Vec3 GetMax(const AABB3D& aabb)
	{
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;

		return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}
};

/*********************/
/******* OBB3D *******/
/*********************/
struct OBB3D
{
	// 위치
	Point3D position = Vec3(0.0f);
	// half size
	Vec3 size = Vec3(1.0f);
	// 회전량 (Matrix or Quaternion)
	Matrix orientation;
};

/*********************/
/****** Plane3D ******/
/*********************/
struct Plane3D
{
	// 노멀 벡터
	Vec3 normal;
	// 원점에서 Plane까지의 거리
	float distance;
};

/*********************/
/**** Triangle3D *****/
/*********************/
struct Triangle3D
{
	// 내부에 들어간 애들은 한 공간에 저장(분석 다르게 가능)
	union
	{
		// 3개의 점을 나타내기 위한 3가지 방법
		struct
		{
			Point3D a;
			Point3D b;
			Point3D c;
		};
		Point3D points[3];
		float values[9];
	};
};

struct Interval3D
{
	float min;
	float max;
};