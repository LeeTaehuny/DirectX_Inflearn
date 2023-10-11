#pragma once
#include "ResourceBase.h"

class Shader;
class Texture;

class Material : public ResourceBase
{
	using Super = ResourceBase;
public:
	Material();
	virtual ~Material();

	// 반환 함수들을 정의합니다.
	shared_ptr<Shader> GetShader() { return _shader; }
	shared_ptr<Texture> GetTexture() { return _texture; }

	// 셰이더를 설정하기 위한 함수를 정의합니다.
	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	// 텍스처를 설정하기 위한 함수를 정의합니다.
	void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

private:
	shared_ptr<Shader> _shader;

	// 셰이더에 넘기는 온갖 인자들
	// SRV
	shared_ptr<Texture> _texture;
};

