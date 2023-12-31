﻿#pragma once
#include "IExecute.h"

class RawBufferDemo : public IExecute
{
	// 출력 데이터 형식
	struct Output
	{
		uint32 groupID[3];
		uint32 groupThreadID[3];
		uint32 dispatchThreadID[3];
		uint32 groupIndex;
	};

public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader> _shader;

private:

};

