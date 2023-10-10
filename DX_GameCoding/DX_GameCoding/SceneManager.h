#pragma once

class Scene;
class Graphics;

class SceneManager
{
public:
	SceneManager(shared_ptr<Graphics> graphics);

public:
	// 현재 씬을 반환하기 위한 함수를 정의합니다.
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

	void Init();
	void Update();

	void LoadScene(wstring sceneName);


private:
	shared_ptr<Scene> LoadTestScene();

private:
	// Graphics 정보를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Graphics> _graphics;

private:
	// 현재 사용중인 씬을 저장하기 위한 변수를 선언합니다.
	shared_ptr<Scene> _activeScene;
};

