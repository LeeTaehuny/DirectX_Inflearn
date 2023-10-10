#pragma once
#include <vector>

#include "GameObject.h"

class SceneManager;
class Graphics;
class Pipeline;
class InputManager;
class TimeManager;
class ResourceManager;

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

	shared_ptr<SceneManager>	GetSceneManager()		{ return _scene; }
	shared_ptr<InputManager>	GetInputManager()		{ return _input; }
	shared_ptr<TimeManager>		GetTimeManager()		{ return _time; }
	shared_ptr<ResourceManager> GetResourceManager()	{ return _resource; }

	shared_ptr<Pipeline> GetPipeline() { return _pipeline; }

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

private:
	// 씬 매니저를 관리하는 2번째 방법
	// -> 최종 게임에서 씬 매니저 자체를 들고 있게 만들기
	// -> Game을 전역으로 만들어 바로 사용이 가능하도록 해주면 됩니다.
	shared_ptr<SceneManager> _scene;
	shared_ptr<InputManager> _input;
	shared_ptr<TimeManager> _time;
	shared_ptr< ResourceManager> _resource;

};

extern unique_ptr<Game> GGame;
