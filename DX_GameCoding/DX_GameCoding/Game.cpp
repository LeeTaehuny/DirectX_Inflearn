#include "pch.h"
#include "Game.h"

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
	_width = GWinSizeX;
	_height = GWinSizeY;
}

void Game::Update()
{
}

void Game::Render()
{
}
