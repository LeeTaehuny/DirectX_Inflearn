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

bool MathUtils::SphereSphere(const Sphere3D& s1, const Sphere3D& s2)
{
	// s1에서 s2까지의 거리를 구해줍니다.
	float sqDistance = (s1.position - s2.position).LengthSquared();

	// s1의 반지름과 s2의 반지름을 합해줍니다.
	float sum = s1.radius + s2.radius;

	// 반지름의 합이 거리보다 크다면 충돌한 것을 의미합니다.
	// * 제곱 환경
	return sqDistance <= sum * sum;
}

bool MathUtils::SphereAABB(const Sphere3D& s, const AABB3D& aabb)
{
	// 구의 중심과 AABB까지의 가장 가까운 점을 구해줍니다.
	Vec3 closestPt = ClosestPoint(aabb, s.position);

	// 구의 중심과 closestPt의 거리를 구해줍니다.
	float distancePt = (closestPt - s.position).LengthSquared();

	// 구의 반지름과 비교해서 반지름보다 거리가 작다면 충돌한 것을 의미합니다.
	// * 제곱 환경
	return distancePt <= s.radius * s.radius;
}

bool MathUtils::SphereOBB(const Sphere3D& s, const OBB3D& obb)
{
	// 구의 중심과 OBB까지의 가장 가까운 점을 구해줍니다.
	Vec3 closestPt = ClosestPoint(obb, s.position);

	// 구의 중심과 closestPt의 거리를 구해줍니다.
	float distancePt = (closestPt - s.position).LengthSquared();

	// 구의 반지름과 비교해서 반지름보다 거리가 작다면 충돌한 것을 의미합니다.
	// * 제곱 환경
	return distancePt <= s.radius * s.radius;
}

bool MathUtils::SpherePlane(const Sphere3D& s, const Plane3D& plane)
{
	// 구의 중심과 Plane까지의 가장 가까운 점을 구해줍니다.
	Vec3 closestPt = ClosestPoint(plane, s.position);

	// 구의 중심과 closestPt의 거리를 구해줍니다.
	float distancePt = (closestPt - s.position).LengthSquared();

	// 구의 반지름과 비교해서 반지름보다 거리가 작다면 충돌한 것을 의미합니다.
	// * 제곱 환경
	return distancePt <= s.radius * s.radius;
}

bool MathUtils::AABBAABB(const AABB3D& aabb1, const AABB3D& aabb2)
{
	// 각 AABB에 대한 Min, Max를 구해줍니다.
	Point3D aMin = AABB3D::GetMin(aabb1);
	Point3D aMax = AABB3D::GetMax(aabb1);
	Point3D bMin = AABB3D::GetMin(aabb2);
	Point3D bMax = AABB3D::GetMax(aabb2);

	// x, y, z 축에 대해 비교를 진행합니다.
	// * 3축이 전부 겹쳐있는 경우만 충돌이 발생합니다.
	bool result =
		(aMin.x <= bMax.x && aMax.x >= bMin.x) && // x축 겹침 비교
		(aMin.y <= bMax.y && aMax.y >= bMin.y) && // y축 겹침 비교
		(aMin.z <= bMax.z && aMax.z >= bMin.z);	  // z축 겹침 비교

	return result;
}

Interval3D MathUtils::GetInterval(const AABB3D& aabb, const Vec3& axis)
{
	// Min, Max 위치를 가져옵니다.
	Vec3 i = AABB3D::GetMin(aabb);
	Vec3 a = AABB3D::GetMax(aabb);

	// Min, Max에 따른 정점들의 위치를 구해줍니다. (8개)
	Vec3 vertex[8] =
	{
		Vec3(i.x, a.y, a.z),
		Vec3(i.x, a.y, i.z),
		Vec3(i.x, i.y, a.z),
		Vec3(i.x, i.y, i.z),

		Vec3(a.x, a.y, a.z),
		Vec3(a.x, a.y, i.z),
		Vec3(a.x, i.y, a.z),
		Vec3(a.x, i.y, i.z)
	};

	// 축에 따른 최대 최소값을 저장할 변수를 선언하고, 초기화합니다.
	Interval3D result;
	result.min = result.max = axis.Dot(vertex[0]);

	// 모든 정점을 순회하며 Min, Max를 구해줍니다.
	for (int i = 1; i < 8; i++)
	{
		// 해당 축으로 투영시킨 결과를 구해줍니다.
		float projection = axis.Dot(vertex[i]);
		// min, max를 비교해주도록 합니다.
		result.min = min(result.min, projection);
		result.max = max(result.max, projection);
	}

	return result;
}

Interval3D MathUtils::GetInterval(const OBB3D& obb, const Vec3& axis)
{
	// 정점의 위치들을 저장하기 위한 변수를 배열을 생성합니다.
	Vec3 vertex[8];

	// half size와 Center를 저장합니다.
	Vec3 C = obb.position;	// Center(위치)
	Vec3 E = obb.size;		// Extents

	// obb를 기준으로 하는 3개의 축 정보를 저장합니다. (정점들의 좌표 연산용)
	vector<Vec3> A;
	A.push_back(obb.orientation.Right());	 // x축
	A.push_back(obb.orientation.Up());		 // y축
	A.push_back(obb.orientation.Backward()); // z축

	// 각 정점들의 좌표를 구해줍니다.
	vertex[0] = C + (A[0] * E.x) + (A[1] * E.y) + (A[2] * E.z);
	vertex[1] = C - (A[0] * E.x) + (A[1] * E.y) + (A[2] * E.z);
	vertex[2] = C + (A[0] * E.x) - (A[1] * E.y) + (A[2] * E.z);
	vertex[3] = C + (A[0] * E.x) + (A[1] * E.y) - (A[2] * E.z);
	vertex[4] = C - (A[0] * E.x) - (A[1] * E.y) - (A[2] * E.z);
	vertex[5] = C + (A[0] * E.x) - (A[1] * E.y) - (A[2] * E.z);
	vertex[6] = C - (A[0] * E.x) + (A[1] * E.y) - (A[2] * E.z);
	vertex[7] = C - (A[0] * E.x) - (A[1] * E.y) + (A[2] * E.z);

	// 축에 따른 최대 최소값을 저장할 변수를 선언하고, 초기화합니다.
	Interval3D result;
	result.min = result.max = axis.Dot(vertex[0]);

	// 모든 정점을 순회하며 Min, Max를 구해줍니다.
	for (int i = 1; i < 8; i++)
	{
		// 해당 축으로 투영시킨 결과를 구해줍니다.
		float projection = axis.Dot(vertex[i]);
		// min, max를 비교해주도록 합니다.
		result.min = min(result.min, projection);
		result.max = max(result.max, projection);
	}

	return result;
}

bool MathUtils::OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis)
{
	// AABB를 axis 축에 투영시킨 결과를 저장합니다.
	Interval3D a = GetInterval(aabb, axis);
	// OBB를 axis 축에 투영시킨 결과를 저장합니다.
	Interval3D b = GetInterval(obb, axis);

	// 투영한 결과인 a, b가 서로 겹치는지 확인합니다.
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool MathUtils::AABBOBB(const AABB3D& aabb, const OBB3D& obb)
{
	// 충돌을 테스트 하기 위한 분리 축 15개(aabb 3개, obb 3개, aabb와 obb의 외적 9개)를 저장합니다.
	// * 우선은 초기값으로 aabb 3개, obb 3개의 축만 넣어주도록 합니다.
	Vec3 test[15] =
	{
		Vec3(1, 0, 0),	 // AABB Axis
		Vec3(0, 1, 0),	 // AABB Axis
		Vec3(0, 0, 1),	 // AABB Axis

		obb.orientation.Right(),	// OBB Axis
		obb.orientation.Up(),		// OBB Axis
		obb.orientation.Backward(),	// OBB Axis
	};

	// 나머지 축은 AABB의 축과 OBB의 축을 Cross(외적)하여 구해줍니다.
	for (int i = 0; i < 3; i++)
	{
		test[6 + i * 3 + 0] = test[i].Cross(test[0]);
		test[6 + i * 3 + 1] = test[i].Cross(test[1]);
		test[6 + i * 3 + 2] = test[i].Cross(test[2]);
	}

	// 찾은 15개의 축들을 순회하며 분리축이 존재하는지 체크합니다.
	for (int i = 0; i < 15; i++)
	{
		// 분리축(Interval이 떨어져 있는 축)이 존재한다면 충돌하지 않은 것을 의미합니다.
		if (OverlapOnAxis(aabb, obb, test[i]) == false)
		{
			return false;
		}
	}

	// 분리축이 존재하지 않는 경우이므로 충돌이 발생합니다.
	return true;
}

bool MathUtils::AABBPlane(const AABB3D& aabb, const Plane3D& plane)
{
	// plane과 AABB간의 최대 거리를 구해줍니다.
	float pLen =
		aabb.size.x * fabsf(plane.normal.x) +
		aabb.size.y * fabsf(plane.normal.y) +
		aabb.size.z * fabsf(plane.normal.z);

	// plane의 법선 벡터와 AABB 위치 벡터를 내적합니다.
	float dot = plane.normal.Dot(aabb.position);

	// plane과 AABB 중심 간의 거리와 평면이 얼마나 떨어져 있는지를 계산합니다.
	float dist = dot - plane.distance;

	// plane과 AABB 간의 거리(dist)가 pLen 값(plane과 AABB 간의 최대 거리) 이내에 있는지를 판별합니다.
	return fabsf(dist) <= pLen;
}

bool MathUtils::OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis)
{
	// OBB1를 axis 축에 투영시킨 결과를 저장합니다.
	Interval3D a = GetInterval(obb1, axis);
	// OBB2를 axis 축에 투영시킨 결과를 저장합니다.
	Interval3D b = GetInterval(obb2, axis);

	// 투영한 결과인 a, b가 서로 겹치는지 확인합니다.
	return ((b.min <= a.max) && (a.min <= b.max));
}

bool MathUtils::OBBOBB(const OBB3D& obb1, const OBB3D& obb2)
{
	// 충돌을 테스트 하기 위한 분리 축 15개(obb1 3개, obb2 3개, obb1와 obb2의 외적 9개)를 저장합니다.
	// * 우선은 초기값으로 aabb 3개, obb 3개의 축만 넣어주도록 합니다.
	Vec3 test[15] =
	{
		obb1.orientation.Right(),		// OBB1 Axis
		obb1.orientation.Up(),			// OBB1 Axis
		obb1.orientation.Backward(),	// OBB1 Axis

		obb2.orientation.Right(),		// OBB2 Axis
		obb2.orientation.Up(),			// OBB2 Axis
		obb2.orientation.Backward(),	// OBB2 Axis
	};

	// 나머지 축은 OBB1의 축과 OBB2의 축을 Cross(외적)하여 구해줍니다.
	for (int i = 0; i < 3; i++)
	{
		test[6 + i * 3 + 0] = test[i].Cross(test[0]);
		test[6 + i * 3 + 1] = test[i].Cross(test[1]);
		test[6 + i * 3 + 2] = test[i].Cross(test[2]);
	}

	// 찾은 15개의 축들을 순회하며 분리축이 존재하는지 체크합니다.
	for (int i = 0; i < 15; i++)
	{
		// 분리축(Interval이 떨어져 있는 축)이 존재한다면 충돌하지 않은 것을 의미합니다.
		if (OverlapOnAxis(obb1, obb2, test[i]) == false)
		{
			return false;
		}
	}

	// 분리축이 존재하지 않는 경우이므로 충돌이 발생합니다.
	return true;
}

bool MathUtils::PlanePlane(const Plane3D& plane1, const Plane3D& plane2)
{
	return plane1.normal.Dot(plane2.normal) == 1;
}
