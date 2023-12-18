#pragma once
#include "IExecute.h"

class GroupDemo : public IExecute
{
	// 입력 데이터 형식
	struct Input
	{
		float value;
	};

	// 출력 데이터 형식
	struct Output
	{
		uint32 groupID[3];
		uint32 groupThreadID[3];
		uint32 dispatchThreadID[3];
		uint32 groupIndex;
		// 데이터를 받아오기 위한 변수
		float value;
	};

public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader> _shader;

private:

};

