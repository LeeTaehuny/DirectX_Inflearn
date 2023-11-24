#include "pch.h"
#include "Material.h"

Material::Material() : Super(ResourceType::Material)
{
}

Material::~Material()
{
}

void Material::Update()
{
	// 만약 셰이더가 존재하지 않는다면 반환합니다.
	if (_shader == nullptr) return;

	// Material 정보를 셰이더에 Push합니다.
	RENDER->PushMaterialData(_desc);

	// 만약 diffuseMap이 존재한다면?
	if (_diffuseMap)
	{
		// 해당 텍스처 정보를 설정합니다.
		_diffuseEffectBuffer->SetResource(_diffuseMap->GetComPtr().Get());
	}

	// 만약 normalMap이 존재한다면?
	if (_normalMap)
	{
		// 해당 텍스처 정보를 설정합니다.
		_normalEffectBuffer->SetResource(_normalMap->GetComPtr().Get());
	}

	// 만약 specularMap이 존재한다면?
	if (_specularMap)
	{
		// 해당 텍스처 정보를 설정합니다.
		_specularEffectBuffer->SetResource(_specularMap->GetComPtr().Get());
	}
}

shared_ptr<Material> Material::Clone()
{
	shared_ptr<Material> clone = make_shared<Material>();

	clone->_desc = _desc;
	clone->_shader = _shader;
	clone->_diffuseMap = _diffuseMap;
	clone->_normalMap = _normalMap;
	clone->_specularMap = _specularMap;
	clone->_diffuseEffectBuffer = _diffuseEffectBuffer;
	clone->_normalEffectBuffer = _normalEffectBuffer;
	clone->_specularEffectBuffer = _specularEffectBuffer;

	return clone;
}

void Material::SetShader(shared_ptr<Shader> shader)
{
	// 셰이더를 저장합니다.
	_shader = shader;

	// 해당 셰이더에 연결되어 있는 SRV를 찾아옵니다.
	_diffuseEffectBuffer = shader->GetSRV("DiffuseMap");
	_normalEffectBuffer = shader->GetSRV("NormalMap");
	_specularEffectBuffer = shader->GetSRV("SpecularMap");
}
