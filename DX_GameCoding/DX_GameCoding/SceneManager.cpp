﻿#include "pch.h"
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
#include "Animator.h"

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

#include "CameraMove.h"
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
			{
				camera->AddComponent(make_shared<CameraMove>());
			}
		}

		// Monster
		{
			shared_ptr<GameObject> monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
			{
				monster->GetOrAddTransform()->SetPosition({ 1.0f, 1.0f, 0.0f });

				auto meshRenderer = make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext());
				monster->AddComponent(meshRenderer);

				auto material = RESOURCES->Get<Material>(L"Default");
				meshRenderer->SetMaterial(material);

				auto mesh = RESOURCES->Get<Mesh>(L"Rectangle");
				meshRenderer->SetMesh(mesh);
			}
			{
				auto animator = make_shared<Animator>();
				monster->AddComponent(animator);

				auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
				animator->SetAnimation(anim);
			}
			scene->AddGameObject(monster);
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
			}
			{
				auto animator = make_shared<Animator>();
				monster->AddComponent(animator);

				auto anim = RESOURCES->Get<Animation>(L"SnakeAnim");
				animator->SetAnimation(anim);
			}
			scene->AddGameObject(monster);
		}
	}

	return scene;
}
