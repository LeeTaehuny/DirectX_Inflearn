#pragma once
#include "ResourceBase.h"

class Shader : public ResourceBase
{
	using Super = ResourceBase;
public:
	Shader();
	virtual ~Shader();

	// 반환 함수들을 정의합니다.
	shared_ptr<InputLayout> GetInputLayout() { return _inputLayout; }
	shared_ptr<VertexShader> GetVertexShader() { return _vertexShader; }
	shared_ptr<PixelShader> GetPixelShader() { return _pixelShader; }

	// 설정 함수들을 정의합니다.
	void SetInputLayout(shared_ptr<InputLayout> inputLayout) { _inputLayout = inputLayout; }
	void SetVertexShader(shared_ptr<VertexShader> vertexShader) { _vertexShader = vertexShader; }
	void SetPixelShader(shared_ptr<PixelShader> pixelShader) { _pixelShader = pixelShader; }

private:
	// InputLayout
	shared_ptr<InputLayout> _inputLayout;
	// VS
	shared_ptr<VertexShader> _vertexShader;
	// PS
	shared_ptr<PixelShader> _pixelShader;
};

