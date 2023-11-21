#include "pch.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Game.h"
#include "Mesh.h"
#include "Shader.h"

MeshRenderer::MeshRenderer() : Super(ComponentType::MeshRenderer)
{

}

MeshRenderer::~MeshRenderer()
{

}

//void MeshRenderer::Update()
//{
//	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
//		return;
//
//	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
//
//	// 현재는 MeshRenderer에서 WVP 행렬을 적용시키고 있습니다.
//	// * 하지만 World와 View/Projection이 같이 수정되는 부분이 이상합니다.
//	// * RenderManager를 만들어 일단은 공용적으로 케어할 수 있게 빼주도록 하겠습니다.
//
//	// MeshRenderer 컴포넌트에서 처리할 내용은 각각의 메시가 가지고 있는 World 정보입니다.
//	// * 공용적인 V, P는 다른곳에서 셰이더에 Push하도록 하겠습니다.
//	auto world = GetTransform()->GetWorldMatrix();
//	RENDER->PushTransformData(TransformDesc{ world });
//
//	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
//	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();
//
//	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
//	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
//}

void MeshRenderer::Update()
{
	if (_mesh == nullptr || _texture == nullptr || _shader == nullptr)
		return;

	_shader->GetSRV("DiffuseMap")->SetResource(_texture->GetComPtr().Get());

	// 현재는 MeshRenderer에서 WVP 행렬을 적용시키고 있습니다.
	// * 하지만 World와 View/Projection이 같이 수정되는 부분이 이상합니다.
	// * RenderManager를 만들어 일단은 공용적으로 케어할 수 있게 빼주도록 하겠습니다.

	// MeshRenderer 컴포넌트에서 처리할 내용은 각각의 메시가 가지고 있는 World 정보입니다.
	// * 공용적인 V, P는 다른곳에서 셰이더에 Push하도록 하겠습니다.
	auto world = GetTransform()->GetWorldMatrix();
	RENDER->PushTransformData(TransformDesc{ world });

	uint32 stride = _mesh->GetVertexBuffer()->GetStride();
	uint32 offset = _mesh->GetVertexBuffer()->GetOffset();

	DC->IASetVertexBuffers(0, 1, _mesh->GetVertexBuffer()->GetComPtr().GetAddressOf(), &stride, &offset);
	DC->IASetIndexBuffer(_mesh->GetIndexBuffer()->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	_shader->DrawIndexed(0, 0, _mesh->GetIndexBuffer()->GetCount(), 0, 0);
}
