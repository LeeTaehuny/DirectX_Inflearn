#pragma once
#include "Primitive3D.h"

struct MathUtils
{
	/**********************************/
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
};

