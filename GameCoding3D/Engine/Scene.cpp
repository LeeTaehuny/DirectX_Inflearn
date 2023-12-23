#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "BaseCollider.h"
#include "Camera.h"

#include "Terrain.h"

void Scene::Start()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Start();
	}
}

void Scene::Update()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->Update();
	}

	// INSTANCING
	vector<shared_ptr<GameObject>> temp;
	temp.insert(temp.end(), objects.begin(), objects.end());
	INSTANCING->Render(temp);
}

void Scene::LateUpdate()
{
	unordered_set<shared_ptr<GameObject>> objects = _objects;

	for (shared_ptr<GameObject> object : objects)
	{
		object->LateUpdate();
	}

	CheckCollision();
}

void Scene::Add(shared_ptr<GameObject> object)
{
	_objects.insert(object);

	if (object->GetCamera() != nullptr)
	{
		_cameras.insert(object);
	}

	if (object->GetLight() != nullptr)
	{
		_lights.insert(object);
	}
}

void Scene::Remove(shared_ptr<GameObject> object)
{
	_objects.erase(object);
	_cameras.erase(object);
	_lights.erase(object);
}

shared_ptr<GameObject> Scene::Pick(int32 screenX, int32 screenY)
{
	// 스크린 좌표를 3D 좌표로 변환해주도록 합니다.
	// * 카메라를 가져옵니다.
	shared_ptr<Camera> camera = GetCamera()->GetCamera();

	// viewport의 크기를 가져옵니다.
	float width = GRAPHICS->GetViewport().GetWidth();
	float height = GRAPHICS->GetViewport().GetHeight();

	// 카메라로부터 Projection 변환 행렬을 가져옵니다.
	Matrix projectionMatrix = camera->GetProjectionMatrix();
	// 카메라로부터 View 변환 행렬을 가져옵니다.
	Matrix viewMatrix = camera->GetViewMatrix();
	// view 변환 행렬의 역행렬을 구해줍니다.
	Matrix viewMatrixInv = viewMatrix.Invert();

	// x, y좌표를 view space의 좌표로 변경합니다.
	float viewX = (+2.0f * screenX / width - 1.0f) / projectionMatrix(0, 0);
	float viewY = (-2.0f * screenY / height + 1.0f) / projectionMatrix(1, 1);

	// 모든 게임 오브젝트를 가져옵니다.
	const auto& gameObjects = GetObjects();

	// 최소 거리를 저장하기 위한 변수
	float minDistance = FLT_MAX;
	
	// 최종 결과로 충돌된 오브젝트를 저장하기 위한 변수
	shared_ptr<GameObject> picked;

	// 모든 GameObject를 순회합니다.
	for (auto& gameObject : gameObjects)
	{
		// 해당 물체에 콜라이더가 존재하지 않으면 무시합니다.
		if (gameObject->GetCollider() == nullptr) continue;

		// ViewSpace에서 Ray를 정의합니다. (위치와 방향)
		// * rayOrigin : (0, 0, 0) -> view space 이므로 월드 공간에서는 카메라의 좌표를 의미합니다.
		Vec4 rayOrigin = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		Vec4 rayDir = Vec4(viewX, viewY, 1.0f, 0.0f);

		// view 공간에 있는 Ray을 world 공간으로 변환해줍니다. (위치와 방향)
		// (view Space에 view Matrix의 역행렬을 곱해 world Space로 변환)
		Vec3 worldRayOrigin = XMVector3TransformCoord(rayOrigin, viewMatrixInv);
		Vec3 worldRayDir = XMVector3TransformNormal(rayDir, viewMatrixInv);

		// worldRayDir은 순수한 방향 벡터이므로 정규화를 시켜줍니다.
		worldRayDir.Normalize();

		// 실제 world 공간에 위치한 Ray를 기존 정보를 토대로 생성합니다.
		Ray ray = Ray(worldRayOrigin, worldRayDir);

		// OutParams
		float distance = 0.0f;
		// 물체가 ray와 충돌하지 않는다면 무시합니다.
		if (gameObject->GetCollider()->Intersects(ray, OUT distance) == false) continue;

		// 만약 구해준 distance가 최소 거리보다 작다면?
		if (distance < minDistance)
		{
			// 최소 거리를 업데이트 합니다.
			minDistance = distance;
			// 충돌 결과 반환용 변수에 최소 거리 오브젝트를 저장합니다.
			picked = gameObject;
		}
	}

	// Terrain Picking(부하가 심함)
	{
		for (auto& gameObject : gameObjects)
		{
			// 만약 해당 오브젝트가 Terrain 컴포넌트를 들고 있지 않다면 무시하기
			if (gameObject->GetTerrain() == nullptr) continue;

			Vec3 pickPos;
			float distance = 0.0f;

			// 해당 오브젝트의 Terrain의 Pick 결과가 false라면 무시하기
			if (gameObject->GetTerrain()->Pick(screenX, screenY, OUT pickPos, OUT distance) == false) continue;
		
			// 만약 구해준 distance가 최소 거리보다 작다면?
			if (distance < minDistance)
			{
				// 최소 거리를 업데이트 합니다.
				minDistance = distance;
				// 충돌 결과 반환용 변수에 최소 거리 오브젝트를 저장합니다.
				picked = gameObject;
			}
		}
	}

	// 모든 물체를 순회했습니다.
	// * picked 변수에는 결국 ray와 충돌한 물체 중 최소 거리의 물체가 저장되어 있으며, 이를 반환해줍니다.
	return picked;
}

void Scene::CheckCollision()
{
	// 콜라이더들을 관리하기 위한 벡터 컨테이너를 생성합니다.
	vector<shared_ptr<BaseCollider>> colliders;

	// 모든 오브젝트를 순회하며 콜라이더를 벡터에 추가합니다.
	for (shared_ptr<GameObject> object : _objects)
	{
		// 만약 오브젝트가 콜라이더를 가지고 있지 않는다면 무시하기
		if (object->GetCollider() == nullptr) continue;

		// 콜라이더 추가
		colliders.push_back(object->GetCollider());
	}

	// 찾은 콜라이더들을 바탕으로 충돌이 일어나는지 체크해줍니다.
	// * BruteForce
	for (int32 i = 0; i < colliders.size(); i++)
	{
		for (int32 j = i + 1; j < colliders.size(); j++)
		{
			shared_ptr<BaseCollider>& other = colliders[j];

			if (colliders[i]->Intersects(other))
			{
				// 충돌이 일어난 경우 여기서 추가 코드들을 실행하면 됩니다.
				int a = 3;
			}
		}
	}
}
