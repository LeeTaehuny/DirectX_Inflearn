#include "pch.h"
#include "MathUtils.h"

bool MathUtils::PointInSphere(const Point3D& point, const Sphere3D& sphere)
{
	// 점과 구체의 원점 사이의 거리(제곱) 구하기
	float magSq = (point - sphere.position).LengthSquared();
	// 구체의 반지름의 제곱 구하기
	float radSq = sphere.radius * sphere.radius;

	// magSq가 radSq보다 작거나 같으면 구체 안에 존재
	return magSq <= radSq;
}

Point3D MathUtils::ClosestPoint(const Sphere3D& sphere, const Point3D& point)
{
	// 구체의 원점에서 점으로 향하는 방향벡터 구하기
	Vec3 sphereToPointDir = (point - sphere.position);
	sphereToPointDir.Normalize();

	// 위치 + (방향벡터 * 반지름)
	return sphere.position + (sphereToPointDir * sphere.radius);
}

bool MathUtils::PointInAABB(const Point3D& point, const AABB3D& aabb)
{
	// Min, Max 좌표를 가져옵니다.
	Point3D min = AABB3D::GetMin(aabb);
	Point3D max = AABB3D::GetMax(aabb);

	// 포함되지 않는 경우를 체크합니다.
	if (point.x < min.x || point.y < min.y || point.z < min.z) return false;
	if (point.x > max.x || point.y > max.y || point.z > max.z) return false;

	return true;
}

Point3D MathUtils::ClosestPoint(const AABB3D& aabb, const Point3D& point)
{
	Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

	result.x = max(result.x, minPt.x);
	result.y = max(result.y, minPt.y);
	result.z = max(result.z, minPt.z);

	result.x = min(result.x, maxPt.x);
	result.y = min(result.y, maxPt.y);
	result.z = min(result.z, maxPt.z);

	return result;
}

bool MathUtils::PointInOBB(const Point3D& point, const OBB3D& obb)
{
	// OBB 상자의 중심으로부터 Point를 향하는 벡터를 만들어줍니다.
	Vec3 dir = point - obb.position;

	// OBB 상자의 각 축을 구해주도록 합니다.
	// * Matrix로 회전을 가지고 있었습니다.
	// * Right, Up, Look 벡터를 추출해 사용합니다.
	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());	// x축
	axis.push_back(obb.orientation.Up());		// y축
	axis.push_back(obb.orientation.Backward()); // z축
	
	// OBB 상자의 크기(half size)를 구해줍니다.
	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);


	for (int32 i = 0; i < 3; i++)
	{
		// 각 축에 dir 벡터를 투영시킨 결과 거리를 구해줍니다.
		float distance = dir.Dot(axis[i]);

		// 만약 거리가 해당 축의 half size(+/-)보다 크/작 다면 안에 존재하지 않는 경우입니다.
		if (distance > size[i]) return false;
		if (distance < -size[i]) return false;
	}

	// OBB 안에 존재하지 않는 경우를 전부 제외했으므로, 여기까지 오면 존재한다는 의미입니다.
	return true;
}

Point3D MathUtils::ClosestPoint(const OBB3D& obb, const Point3D& point)
{
	// 반환용 Point를 초기화합니다. (OBB 상자의 원점)
	Point3D result = obb.position;
	// OBB 상자의 원점으로부터 Point로 향하는 벡터를 구해줍니다.
	Vec3 dir = point - obb.position;

	// OBB 상자의 각 축을 구해주도록 합니다.
	// * Matrix로 회전을 가지고 있었습니다.
	// * Right, Up, Look 벡터를 추출해 사용합니다.
	vector<Vec3> axis;
	axis.push_back(obb.orientation.Right());	// x축
	axis.push_back(obb.orientation.Up());		// y축
	axis.push_back(obb.orientation.Backward()); // z축

	// OBB 상자의 크기(half size)를 구해줍니다.
	vector<float> size;
	size.push_back(obb.size.x);
	size.push_back(obb.size.y);
	size.push_back(obb.size.z);

	for (int32 i = 0; i < 3; i++)
	{
		// 각 축에 dir 벡터를 투영시킨 결과 거리를 구해줍니다.
		float distance = dir.Dot(axis[i]);

		// 만약 거리가 해당 축의 half size(+/-)보다 크/작 다면 거리를 범위 안으로 조정해줍니다.
		if (distance > size[i])
		{
			distance = size[i];
		}

		if (distance < -size[i])
		{
			distance = -size[i];
		}

		// 구한 거리를 토대로 result 점을 만들어줍니다.
		result = result + (axis[i] * distance);
	}
	
	return result;
}

bool MathUtils::PointInPlane(const Point3D& point, const Plane3D& plane)
{
	// 점과 plane의 normal 벡터를 내적한 결과를 저장합니다.
	float dot = point.Dot(plane.normal);

	// plane의 distance(원점에서 떨어져있는 거리)와 dot이 같으면 plane 내에 점이 존재한다는 의미입니다.
	return dot == plane.distance;
}

Point3D MathUtils::ClosestPoint(const Plane3D& plane, const Point3D& point)
{
	// 점과 plane의 normal 벡터를 내적한 결과를 저장합니다.
	float dot = point.Dot(plane.normal);

	// 내적한 결과에서 plane의 distance(원점에서 떨어져 있는 거리)를 빼줍니다.
	float distance = dot - plane.distance;

	// 점의 위치로부터 거리의 차이만큼을 빼주면 plane 상의 가장 가까운 점을 구할 수 있습니다.
	return point - (plane.normal * distance);
}

bool MathUtils::PointInLine(const Point3D& point, const Line3D& line)
{
	// 라인 상의 가장 가까운 점을 구해줍니다.
	Point3D closest = ClosestPoint(line, point);

	// 가장 가까운 점에서 입력받은 점의 거리가 0이면 점이 라인 위에 존재한다고 할 수 있습니다.
	float distanceSq = (closest - point).LengthSquared();

	return distanceSq == 0.0f;
}

Point3D MathUtils::ClosestPoint(const Line3D& line, const Point3D& point)
{
	// 라인 벡터를 구해줍니다.
	Vec3 lVec = line.end - line.start;

	// 라인의 시작점에서 Point로 향하는 벡터를 구해줍니다.
	Vec3 lToP = point - line.start;

	// lToP를 라인에 투영시킨 비율을 구해줍니다.
	// * lToP.Dot(lVec) = lVec의 크기 * lToP의 크기 * cos(각도)
	// * lVec.Dot(lVec) = lVec의 크기 * lVec의 크기 * cos(0);
	//   -> 나눠주면 상대적인 비율이 나옴
	float t = lToP.Dot(lVec) / lVec.Dot(lVec);

	// 비율이 0 ~ 1 사이라면 라인 내에 점이 존재한다는 의미입니다.
	// * 비율을 0 ~ 1 사이로 맞춰줍니다.
	t = fmaxf(t, 0.0f);
	t = fminf(t, 1.0f);

	// 맞춰준 비율에 따라 점 위치를 반환합니다.
	return line.start + (lVec * t);
}

bool MathUtils::PointInRay(const Point3D& point, const Ray3D& ray)
{
	// Point가 Ray의 시작 위치라면 true를 반환합니다.
	if (point == ray.origin) return true;

	// Ray의 시작위치에서 Point로 향하는 방향 벡터를 구해줍니다.
	Vec3 rayToPoint = point - ray.origin;
	rayToPoint.Normalize();

	// 위에서 구한 방향벡터와 ray의 방향벡터가 일치한다면 (내적 결과가 1) 위에 존재한다고 볼 수 있습니다.
	float diff = rayToPoint.Dot(ray.direction);

	return diff == 1.0f;
}

Point3D MathUtils::ClosestPoint(const Ray3D& ray, const Point3D& point)
{
	// Ray의 시작위치에서 Point로 향하는 벡터를 구해줍니다.
	Vec3 rayToPoint = point - ray.origin;

	// rayToPoint와 Ray의 방향벡터랑 내적한 결과를 구해줍니다.
	float t = rayToPoint.Dot(ray.direction);

	// Ray의 시작위치보다 뒤에 위치하지 않도록 최소 거리를 0으로 설정합니다.
	t = fmaxf(t, 0.0f);

	// 위치를 반환합니다.
	return ray.origin + (ray.direction * t);
}
