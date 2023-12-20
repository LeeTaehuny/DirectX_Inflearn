#include "pch.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h"
#include "Camera.h"
#include "Light.h"

ModelRenderer::ModelRenderer(shared_ptr<Shader> shader)
	: Super(ComponentType::ModelRenderer), _shader(shader)
{

}

ModelRenderer::~ModelRenderer()
{
}

// 계층구조 생각 O
void ModelRenderer::Update()
{
	if (_model == nullptr)
		return;

	// Bone들의 정보를 설정합니다.
	// * 저장하기 위한 변수 선언
	BoneDesc boneDesc;
	// * 저장할 Bone의 숫자 받아오기
	const uint32 boneCount = _model->GetBoneCount();
	// * 숫자만큼 순회하며 Bone 정보 삽입
	for (uint32 i = 0; i < boneCount; i++)
	{
		// 해당 인덱스에 맞는 Bone를 찾아옵니다.
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		// Bone 정보에 해당 bone의 Transform 정보를 저장합니다.
		boneDesc.transforms[i] = bone->transform;
	}

	// 저장한 Bone들의 정보를 셰이더에 Push합니다.
	_shader->PushBoneData(boneDesc);

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	_shader->PushTransformData(TransformDesc{ world });

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// 해당 메쉬의 BoneIndex를 셰이더에 전달합니다.
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		uint32 stride = mesh->vertexBuffer->GetStride();
		uint32 offset = mesh->vertexBuffer->GetOffset();

		DC->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
		DC->IASetIndexBuffer(mesh->indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

		_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
	}
}

void ModelRenderer::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		// 셰이더 연결
		material->SetShader(_shader);
	}
}

void ModelRenderer::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_model == nullptr)
		return;

	// GlobalData
	_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightobj = SCENE->GetCurrentScene()->GetLight();

	if (lightobj)
		_shader->PushLightData(lightobj->GetLight()->GetLightDesc());

	// Bone들의 정보를 설정합니다.
	// * 저장하기 위한 변수 선언
	BoneDesc boneDesc;
	// * 저장할 Bone의 숫자 받아오기
	const uint32 boneCount = _model->GetBoneCount();
	// * 숫자만큼 순회하며 Bone 정보 삽입
	for (uint32 i = 0; i < boneCount; i++)
	{
		// 해당 인덱스에 맞는 Bone를 찾아옵니다.
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		// Bone 정보에 해당 bone의 Transform 정보를 저장합니다.
		boneDesc.transforms[i] = bone->transform;
	}

	// 저장한 Bone들의 정보를 셰이더에 Push합니다.
	_shader->PushBoneData(boneDesc);

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// 해당 메쉬의 BoneIndex를 셰이더에 전달합니다.
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		// IA
		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		buffer->PushData();

		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}

InstanceID ModelRenderer::GetInstanceID()
{
	// InstanceID는 model와 shader의 주소값(uint64)으로 설정했습니다.
	// * 형식에 맞게 반환해줍니다.
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}
