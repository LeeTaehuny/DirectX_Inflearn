#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

void Scene::Awake()
{
	for (const shared_ptr<GameObject>& gameObjcet : _gameObjects)
	{
		gameObjcet->Awake();
	}
}

void Scene::Start()
{
	for (const shared_ptr<GameObject>& gameObjcet : _gameObjects)
	{
		gameObjcet->Start();
	}
}

void Scene::Update()
{
	for (const shared_ptr<GameObject>& gameObjcet : _gameObjects)
	{
		gameObjcet->Update();
	}
}

void Scene::LateUpdate()
{
	for (const shared_ptr<GameObject>& gameObjcet : _gameObjects)
	{
		gameObjcet->LateUpdate();
	}
}

void Scene::FixedUpdate()
{
	for (const shared_ptr<GameObject>& gameObjcet : _gameObjects)
	{
		gameObjcet->FixedUpdate();
	}
}

void Scene::AddGameObject(shared_ptr<GameObject> gameObject)
{
	_gameObjects.push_back(gameObject);
}

void Scene::RemoveGameObject(shared_ptr<GameObject> gameObject)
{
	auto findIt = std::find(_gameObjects.begin(), _gameObjects.end(), gameObject);

	if (findIt != _gameObjects.end())
	{
		_gameObjects.erase(findIt);
	}
}
