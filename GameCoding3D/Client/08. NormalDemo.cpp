#include "pch.h"
#include "08. NormalDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"

void NormalDemo::Init()
{
	// 셰이더 fx를 생성합니다.
	_shader = make_shared<Shader>(L"07. Normal.fx");

	/** Object */
	{
		// 기하학 정보를 생성합니다.
		_geometry = make_shared<Geometry<VertexTextureNormalData>>();

		// 도형 만들기
		{
			// GeometryHelper에 추가한 함수를 통해 그리드의 기하학적인 정보를 세팅합니다.
			GeometryHelper::CreateCube(_geometry);
			//GeometryHelper::CreateSphere(_geometry);
		}

		// 정점 버퍼를 생성합니다.
		_vertexBuffer = make_shared<VertexBuffer>();
		_vertexBuffer->Create(_geometry->GetVertices());

		// 인덱스 버퍼를 생성합니다.
		_indexBuffer = make_shared<IndexBuffer>();
		_indexBuffer->Create(_geometry->GetIndices());
	}

	/** 카메라 */
	{
		// 카메라를 저장하기 위한 게임 오브젝트를 생성합니다.
		_camera = make_shared<GameObject>();
		// 해당 게임 오브젝트에 Transform 컴포넌트를 추가합니다.
		_camera->GetOrAddTransform();
		// 해당 게임 오브젝트에 카메라 컴포넌트를 추가합니다.
		_camera->AddComponent(make_shared<Camera>());
		// 생성한 카메라의 이동 스크립트(CameraScript)를 추가합니다.
		_camera->AddComponent(make_shared<CameraScript>());
	}

	/** 텍스처 */
	{
		// 텍스처를 로드와 동시에 저장합니다.
		_texture = RESOURCES->Load<Texture>(L"Veigar", L"..\\Resources\\Textures\\veigar.jpg");
	}

}

void NormalDemo::Update()
{
	_camera->Update();
}

void NormalDemo::Render()
{
	// 행렬들을 Shader에 적용합니다.
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// 셰이더 파일에 텍스처를 전달합니다.
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());
	// 셰이더 파일에 빛의 방향 벡터를 전달합니다.
	_shader->GetVector("LightDir")->SetFloatVector((float*)&_lightDir);

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// IA 단계에 VertexBuffer를 연결시켜줍니다.
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	// IA 단계에 IndexBuffer를 연결시켜줍니다.
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 화면에 그려줍니다.
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
