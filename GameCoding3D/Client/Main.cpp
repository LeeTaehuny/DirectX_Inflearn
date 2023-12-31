﻿#include "pch.h"
#include "Main.h"
#include "Engine/Game.h"

#include "BillBoardDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// 게임의 기본 정보를 설정합니다.
	GameDesc desc;
	desc.appName = L"GameCoding";
	desc.hInstance = hInstance;
	desc.vsync = false;
	desc.hWnd = NULL;
	desc.width = 800;
	desc.height = 600;
	desc.clearColor = Color(0.5f, 0.5f, 0.5f, 0.5f);

	desc.app = make_shared<BillBoardDemo>();

	GAME->Run(desc);

	return 0;
}