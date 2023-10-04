﻿#pragma once
#include <vector>

#include "GameObject.h"

class Game
{
public:
	Game();
	~Game();

public:
	// 초기화 함수를 선언합니다.
	void	Init(HWND hwnd);
	// 업데이트 함수를 선언합니다.
	void	Update();
	// 렌더 함수를 선언합니다.
	void	Render();

private:
	// 윈도우 핸들 번호를 저장하기 위한 변수를 선언합니다.
	HWND	_hwnd = 0;

	// 윈도우 창 크기를 저장하기 위한 변수를 선언합니다.
	// uint32	_width = 0;
	// uint32	_height = 0;

	// Graphics 타입의 Shared_ptr를 선언합니다.
	shared_ptr<Graphics> _graphics;
	// Pipeline 타입의 Shared_ptr를 선언합니다.
	shared_ptr<Pipeline> _pipeline;

	// GameObject 타입의 shared_ptr를 선언합니다.
	shared_ptr<GameObject> _gameObject;

};

