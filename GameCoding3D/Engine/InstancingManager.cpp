#include "pch.h"
#include "InstancingManager.h"

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{

	// 이전 프레임의 데이터를 초기화합니다.
	ClearData();

	// 새로운 데이터를 출력합니다.
	RenderMeshRender(gameObjects);
}

void InstancingManager::ClearData()
{
	// 매 프레임 map을 순회하며 shared_ptr<InstancingBuffer>의 _data를 초기화합니다.
	for (auto& pair : _buffers)
	{
		pair.second->ClearData();
	}
}

void InstancingManager::RenderMeshRender(vector<shared_ptr<GameObject>> gameObjects)
{
	map<InstanceID, vector<shared_ptr<GameObject>>> cache;

	// 매 프레임 MeshRenderer 컴포넌트를 가지고 있는 오브젝트들을 찾습니다. (분류 단계)
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		// MeshRenderer 컴포넌트가 존재하지 않으면 건너뛰기
		if (gameObject->GetMeshRenderer() == nullptr)
		{
			continue;
		}

		// MeshRenderer의 InstanceID를 가져와 해당 아이디 키에 오브젝트를 저장합니다.
		const InstanceID instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	// 위 분류 작업을 통해 인스턴스 ID가 같은 물체들끼리 정리되었습니다.
	// * 순회하며 렌더링해주도록 합니다.
	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		// 인스턴스 아이디를 가져옵니다.
		const InstanceID instanceId = pair.first;

		// 모든 데이터들을 순회하며 값을 저장합니다.
		for (int32 i = 0; i < vec.size(); i++)
		{
			const shared_ptr<GameObject>& gameObject = vec[i];
			InstancingData data;

			// 월드 변환 행렬을 넣어줍니다.
			data.world = gameObject->GetTransform()->GetWorldMatrix();

			// 버퍼에 데이터를 추가합니다.
			AddData(instanceId, data);
		}
		
		// 모든 데이터를 저장했습니다.
		// * 실제로 화면에 그려줍니다. (그려주는 것은 한 번만 실행하도록 합니다.)
		shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
		vec[0]->GetMeshRenderer()->RenderInstancing(buffer);
	}
}

void InstancingManager::AddData(InstanceID instanceId, InstancingData& data)
{
	// 버퍼에 이미 해당 데이터가 존재하는지 체크합니다.
	// * 만약 없는 경우라면 새로 생성해줍니다.
	if (_buffers.find(instanceId) == _buffers.end())
	{
		_buffers[instanceId] = make_shared<InstancingBuffer>();
	}

	// 데이터를 추가합니다.
	_buffers[instanceId]->AddData(data);
}
