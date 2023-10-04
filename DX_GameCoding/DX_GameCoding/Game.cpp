#include "pch.h"
#include "Game.h"
//#include "Graphics.h"
//#include "VertexBuffer.h"

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
	_gameObject = make_shared<GameObject>(_graphics->GetDevice(), _graphics->GetDeviceContext());
}

void Game::Update()
{
	_gameObject->Update();
}

void Game::Render()
{
	// 렌더를 위한 준비 작업
	_graphics->RenderBegin();

	_gameObject->Render(_pipeline);

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
}