#pragma once
#include "ResourceBase.h"

// Material : 셰이더에 전달하는 인자 모음
class Material : public ResourceBase
{
	using Super = ResourceBase;

public:
	Material();
	virtual ~Material();

	void Update();
	// 원본에 영향을 주지 않는 클론을 만들기 위한 함수
	shared_ptr<Material> Clone();

public:
	// 필요한 Getter 함수들을 정의합니다.
	// * 셰이더
	shared_ptr<Shader> GetShader() { return _shader; }
	// * 머터리얼의 정보
	MaterialDesc& GetMaterialDesc() { return _desc; }
	// * TextureMap
	shared_ptr<Texture> GetDiffuseMap() { return _diffuseMap; }
	shared_ptr<Texture> GetNormalMap() { return _normalMap; }
	shared_ptr<Texture> GetSpecularMap() { return _specularMap; }

	// 필요한 Setter 함수들을 정의합니다.
	// * 셰이더
	void SetShader(shared_ptr<Shader> shader);
	// * TextureMap
	void SetDiffuseMap(shared_ptr<Texture> diffuseMap) { _diffuseMap = diffuseMap; }
	void SetNormalMap(shared_ptr<Texture> normalMap) { _normalMap = normalMap; }
	void SetSpecularMap(shared_ptr<Texture> specularMap) { _specularMap = specularMap; }

private:
	friend class MeshRenderer;

	// Material이 가져야 할 정보들을 저장하기 위한 변수를 선언합니다.
	MaterialDesc _desc;

	// 셰이더에 값을 넘겨주기 위해서 셰이더를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Shader> _shader;
	// 여러 종류의 텍스처를 저장하기 위한 변수들을 선언합니다.
	// * diffuseMap
	shared_ptr<Texture> _diffuseMap;
	// * normalMap
	shared_ptr<Texture> _normalMap;
	// * specularMap
	shared_ptr<Texture> _specularMap;

	// Effect11과 연동하기 위한 각각의 이펙트 퍼버를 생성합니다.
	ComPtr<ID3DX11EffectShaderResourceVariable> _diffuseEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> _normalEffectBuffer;
	ComPtr<ID3DX11EffectShaderResourceVariable> _specularEffectBuffer;
};

