#include "pch.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h"

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader)
	: Super(ComponentType::Animator), _shader(shader)
{
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Update()
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
	RENDER->PushBoneData(boneDesc);

	// Transform
	auto world = GetTransform()->GetWorldMatrix();
	RENDER->PushTransformData(TransformDesc{ world });

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

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		// 셰이더 연결
		material->SetShader(_shader);
	}
}
