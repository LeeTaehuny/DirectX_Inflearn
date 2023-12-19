#pragma once
#include "IExecute.h"

class TextureBufferDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

private:
	shared_ptr<Shader> _shader;

private:
	// Compute Shader를 통해 텍스처를 가공 후 반환해주기 위한 함수를 선언합니다.
	ComPtr<ID3D11ShaderResourceView> MakeComputeShaderTexture();
};

