#pragma once

class GameObject;

class Scene
{
public:

	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();


public:
	// 게임 오브젝트를 추가하기 위한 함수를 선언합니다.
	void AddGameObject(shared_ptr<GameObject> gameObject);
	// 게임 오브젝트를 제거하기 위한 함수를 선언합니다.
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	// 게임 오브젝트를 반환하기 위한 함수를 정의합니다.
	const vector<shared_ptr<GameObject>>& GetGameObjcets() { return _gameObjects; }

private:
	// 게임 오브젝트들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<GameObject>> _gameObjects;
};

