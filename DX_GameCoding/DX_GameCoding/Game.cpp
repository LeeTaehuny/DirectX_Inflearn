#include "pch.h"
#include "Game.h"

#include "Camera.h"
#include "MeshRenderer.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	// 멤버 변수 초기화
	_hwnd = hwnd;

	// Graphics 객체를 생성합니다.
	_graphics = make_shared<Graphics>(hwnd);
	// Pipeline 객체를 생성합니다.
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());
	// GameObject 객체를 생성합니다.
	_monster = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		// Transform 컴포넌트를 추가합니다.
		_monster->GetOrAddTransform();
		//_monster->GetTransform()->SetScale(Vec3(100.0f, 100.0f, 1.0f));
		_monster->AddComponent(make_shared<MeshRenderer>(_graphics->GetDevice(), _graphics->GetDeviceContext()));
	}
	// GameObject 객체를 생성합니다.
	_camera = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
	{
		_camera->GetOrAddTransform();

		_camera->AddComponent(make_shared<Camera>());
	}
}

void Game::Update()
{
	_monster->Update();
	_camera->Update();
}

void Game::Render()
{
	// 렌더를 위한 준비 작업
	_graphics->RenderBegin();

	//_monster->Render(_pipeline);
	// TEMP
	_monster->GetMeshRenderer()->Render(_pipeline);

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
}