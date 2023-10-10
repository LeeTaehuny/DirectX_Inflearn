#include "pch.h"
#include "Game.h"

#include "Camera.h"
#include "MeshRenderer.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Graphics.h"
#include "Pipeline.h"

unique_ptr<Game> GGame = make_unique<Game>();

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

	_input = make_shared<InputManager>();
	_input->Init(hwnd);

	_time = make_shared<TimeManager>();
	_time->Init();

	_scene = make_shared<SceneManager>(_graphics);
	_scene->Init();

	_resource = make_shared<ResourceManager>(_graphics->GetDevice());
	_resource->Init();

	SCENE->LoadScene(L"Test");
}

void Game::Update()
{
	// 렌더를 위한 준비 작업
	_graphics->RenderBegin();

	TIME->Update();
	INPUT->Update();
	SCENE->Update();

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
	
}

void Game::Render()
{

}