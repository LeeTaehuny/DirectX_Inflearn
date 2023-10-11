#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Camera.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Graphics.h"
#include "Game.h"
#include "ResourceManager.h"
#include "Mesh.h"

SceneManager::SceneManager(shared_ptr<Graphics> graphics) : _graphics(graphics)
{
}

void SceneManager::Init()
{
	if (_activeScene == nullptr) return;

	_activeScene->Awake();
	_activeScene->Start();
}

void SceneManager::Update()
{
	if (_activeScene == nullptr) return;

	_activeScene->Update();	
	_activeScene->LateUpdate();

	// 물리 관련..
	_activeScene->FixedUpdate();
}

void SceneManager::LoadScene(wstring sceneName)
{
	// Resource...

	_activeScene = LoadTestScene();
	Init();
}

shared_ptr<Scene> SceneManager::LoadTestScene()
{
	shared_ptr<Scene> scene = make_shared<Scene>();

	// TODO : 씬 불러오기
	{
		// Camera
		{
			shared_ptr<GameObject> camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			{
				camera->GetOrAddTransform();
				camera->AddComponent(make_shared<Camera>());

				scene->AddGameObject(camera);
			}
		}

		// Monster
		{
			shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			{
				monster->GetOrAddTransform();

				auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
				monster->AddComponent(meshRenderer);

				auto material = RESOURCES->Get<Material>(L"Default");
				meshRenderer->SetMaterial(material);

				auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
				meshRenderer->SetMesh(mesh);

				scene->AddGameObject(monster);
			}
		}
	}

	return scene;
}
