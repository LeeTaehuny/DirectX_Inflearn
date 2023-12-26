#pragma once
#include "Primitive3D.h"

struct MathUtils
{
	/*********** Point Test ***********/

	// Sphere2Point
	// * 점이 구체 안에 존재하는지를 체크하기 위한 함수
	static bool PointInSphere(const Point3D& point, const Sphere3D& sphere);
	// * 점과 구체 사이의 가장 가까운 점(구체 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const Sphere3D& sphere, const Point3D& point);

	// AABB2Point
	// * 점이 AABB 안에 존재하는지를 체크하기 위한 함수
	static bool PointInAABB(const Point3D& point, const AABB3D& aabb);
	// * 점과 AABB 사이의 가장 가까운 점(AABB 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const AABB3D& aabb, const Point3D& point);

	// OBB2Point
	// * 점이 OBB 안에 존재하는지를 체크하기 위한 함수
	static bool PointInOBB(const Point3D& point, const OBB3D& obb);
	// * 점과 OBB 사이의 가장 가까운 점(OBB 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const OBB3D& obb, const Point3D& point);

	// Plane2Point
	// * 점이 Plane 안에 존재하는지를 체크하기 위한 함수
	static bool PointInPlane(const Point3D& point, const Plane3D& plane);
	// * 점과 Plane 사이의 가장 가까운 점(Plane 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const Plane3D& plane, const Point3D& point);

	// Line2Point
	// * 점이 Line 안에 존재하는지를 체크하기 위한 함수
	static bool PointInLine(const Point3D& point, const Line3D& line);
	// * 점과 Line 사이의 가장 가까운 점(Plane 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const Line3D& line, const Point3D& point);

	// Ray2Point
	// * 점이 Ray 안에 존재하는지를 체크하기 위한 함수
	static bool PointInRay(const Point3D& point, const Ray3D& ray);
	// * 점과 Ray 사이의 가장 가까운 점(Ray 범위 내)을 구하는 함수
	static Point3D ClosestPoint(const Ray3D& ray, const Point3D& point);


	/*********** Intersection ***********/

	// Sphere2Sphere
	static bool SphereSphere(const Sphere3D& s1, const Sphere3D& s2);
	// Sphere2AABB
	static bool SphereAABB(const Sphere3D& s, const AABB3D& aabb);
	// Sphere2OBB
	static bool SphereOBB(const Sphere3D& s, const OBB3D& obb);
	// Sphere2Plane
	static bool SpherePlane(const Sphere3D& s, const Plane3D& plane);

	
	// AABB2AABB
	static bool AABBAABB(const AABB3D& aabb1, const AABB3D& aabb2);
	
	// AABB2OBB
	// * 어떤 축에 Box를 투영시킨 결과 Interval(min, max)을 구하는 함수 2종
	static Interval3D GetInterval(const AABB3D& aabb, const Vec3& axis);
	static Interval3D GetInterval(const OBB3D& obb, const Vec3& axis);
	// * 축에 따라 AABB와 OBB의 투영 결과가 겹치는지 판별하는 함수
	static bool OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis);

	static bool AABBOBB(const AABB3D& aabb, const OBB3D& obb);

	// AABB2Plane
	static bool AABBPlane(const AABB3D& aabb, const Plane3D& plane);


	// OBB2OBB
	// * 축에 따라 OBB와 OBB의 투영 결과가 겹치는지 판별하는 함수
	static bool OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis);
	static bool OBBOBB(const OBB3D& obb1, const OBB3D& obb2);


	// Plane2Plane
	static bool PlanePlane(const Plane3D& plane1, const Plane3D& plane2);
};

