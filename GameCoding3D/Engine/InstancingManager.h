#pragma once
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "ModelAnimator.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	// 이전 프레임에 등록된 데이터들을 초기화하기 위한 함수를 선언합니다.
	void ClearData();

private:
	// MeshRenderer 컴포넌트를 가지고 있는 오브젝트들만을 출력하기 위한 함수를 선언합니다.
	void RenderMeshRenderer(vector<shared_ptr<GameObject>> gameObjects);

	// ModelRenderer 컴포넌트를 가지고 있는 오브젝트들만을 출력하기 위한 함수를 선언합니다.
	void RenderModelRenderer(vector<shared_ptr<GameObject>> gameObjects);

	// ModelAnimator 컴포넌트를 가지고 있는 오브젝트들만을 출력하기 위한 함수를 선언합니다.
	void RenderAnimRenderer(vector<shared_ptr<GameObject>> gameObjects);

private:
	// 버퍼에 데이터를 추가하기 위한 함수를 선언합니다.
	void AddData(InstanceID instanceId, InstancingData& data);

private:
	// 인스턴싱 버퍼를 저장하기 위한 맵 컨테이너를 선언합니다.
	// * 아이디에 따른 버퍼 관리
	map<InstanceID, shared_ptr<InstancingBuffer>> _buffers;
};

