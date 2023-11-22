#include "pch.h"
#include "RenderManager.h"
#include "Camera.h"

void RenderManager::Init(shared_ptr<Shader> shader)
{
	// 전달받은 셰이더를 저장합니다.
	_shader = shader;

	// 버퍼들을 생성합니다.
	// * 전역 버퍼를 생성합니다. (V, P)
	_globalBuffer = make_shared<ConstantBuffer<GlobalDesc>>();
	_globalBuffer->Create();
	// * 셰이더에 저장된 전역 버퍼를 가져와 EffectBuffer에 저장합니다.
	_globalEffectBuffer = _shader->GetConstantBuffer("GlobalBuffer");

	// * 위치 버퍼를 생성합니다. (W)
	_transformBuffer = make_shared<ConstantBuffer<TransformDesc>>();
	_transformBuffer->Create();
	// * 셰이더에 저장된 위치 버퍼를 가져와 EffectBuffer에 저장합니다.
	_transformEffectBuffer = _shader->GetConstantBuffer("TransformBuffer");

	// * 빛 관련 버퍼들을 생성합니다.
	_lightBuffer = make_shared<ConstantBuffer<LightDesc>>();
	_lightBuffer->Create();

	_materialBuffer = make_shared<ConstantBuffer<MaterialDesc>>();
	_materialBuffer->Create();

	// * 셰이더에 저장된 위치 버퍼를 가져와 EffectBuffer에 저장합니다.
	_lightEffectBuffer = _shader->GetConstantBuffer("LightBuffer");
	_materialEffectBuffer = _shader->GetConstantBuffer("MaterialBuffer");

}

void RenderManager::Update()
{
	// 매 프레임마다 글로벌 정보가 업데이트되도록 설정해줍니다.
	PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
}

void RenderManager::PushGlobalData(const Matrix& view, const Matrix& projection)
{
	// 전달받은 전역 데이터들을 정보에 저장합니다. (CPU 연산)
	_globalDesc.P = projection;
	_globalDesc.V = view;
	_globalDesc.VP = view * projection;

	// CPU에서 연산이 완료된 데이터를 GPU에 복사합니다.
	_globalBuffer->CopyData(_globalDesc);
	// 복사된 데이터를 셰이더에 연결시켜줍니다. (셰이더에 밀어넣기)
	_globalEffectBuffer->SetConstantBuffer(_globalBuffer->GetComPtr().Get());
}

void RenderManager::PushTransformData(const TransformDesc& desc)
{
	// 전달받은 위치 데이터들을 정보에 저장합니다. (CPU 연산)
	_transformDesc = desc;

	// CPU에서 연산이 완료된 데이터를 GPU에 복사합니다.
	_transformBuffer->CopyData(_transformDesc);
	// 복사된 데이터를 셰이더에 연결시켜줍니다. (셰이더에 밀어넣기)
	_transformEffectBuffer->SetConstantBuffer(_transformBuffer->GetComPtr().Get());
}

void RenderManager::PushLightData(const LightDesc& desc)
{
	_lightDesc = desc;
	_lightBuffer->CopyData(_lightDesc);
	_lightEffectBuffer->SetConstantBuffer(_lightBuffer->GetComPtr().Get());
}

void RenderManager::PushMaterialData(const MaterialDesc& desc)
{
	_materialDesc = desc;
	_materialBuffer->CopyData(_materialDesc);
	_materialEffectBuffer->SetConstantBuffer(_materialBuffer->GetComPtr().Get());
}
