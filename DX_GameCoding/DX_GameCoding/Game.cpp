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
	// ��� ���� �ʱ�ȭ
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
