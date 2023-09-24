#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	// �ʱ�ȭ �Լ��� �����մϴ�.
	void	Init(HWND hwnd);
	// ������Ʈ �Լ��� �����մϴ�.
	void	Update();
	// ���� �Լ��� �����մϴ�.
	void	Render();

private:
	// ������ �ڵ� ��ȣ�� �����ϱ� ���� ������ �����մϴ�.
	HWND	_hwnd;

	// ������ â ũ�⸦ �����ϱ� ���� ������ �����մϴ�.
	uint32	_width = 0;
	uint32	_height = 0;

private:
	// DX
};

