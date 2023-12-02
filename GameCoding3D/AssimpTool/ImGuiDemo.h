#pragma once
#include "IExecute.h"

class ImGuiDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	void Test();

private:
	// 화면 On/Off용 변수
	bool show_demo_window = true;
	bool show_another_window = false;

	// 초기화 색상
	Vec4 clear_color = Vec4(0.0f);

};

