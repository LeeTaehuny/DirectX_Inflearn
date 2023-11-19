#include "pch.h"
#include "07. HeightMapDemo.h"
#include "GeometryHelper.h"
#include "Camera.h"
#include "CameraScript.h"

void HeightMapDemo::Init()
{
	// 셰이더 fx를 생성합니다.
	_shader = make_shared<Shader>(L"06. Terrain.fx");

	/** 텍스처 */
	{
		// 맵의 높이 정보를 불러와 저장합니다.
		_heightMap = RESOURCES->Load<Texture>(L"Height", L"..\\Resources\\Textures\\Terrain\\height.png");
		// 텍스처를 로드와 동시에 저장합니다.
		_texture = RESOURCES->Load<Texture>(L"Grass", L"..\\Resources\\Textures\\Terrain\\grass.jpg");
	}
	// 맵의 정보를 추출합니다.
	const int32 width = _heightMap->GetSize().x;
	const int32 height = _heightMap->GetSize().y;

	// 임시 테스트용으로 heightMap의 정보를 추출합니다.
	// * 현재 heightMap은 8bit(R)로만 구성되어 있음, 만약 다른 포맷이라면 변경해줘야 함
	const DirectX::ScratchImage& info = _heightMap->GetInfo();
	uint8* pixelBuffer = info.GetPixels();


	/** Object */
	{
		// 기하학 정보를 생성합니다.
		_geometry = make_shared<Geometry<VertexTextureData>>();

		// 그리드 만들기
		{
			// GeometryHelper에 추가한 함수를 통해 그리드의 기하학적인 정보를 세팅합니다.
			GeometryHelper::CreateGrid(_geometry, width, height);
		}

		// CPU 선처리
		{
			// 정점들의 정보를 얻어와 높이 맵을 기준으로 조작합니다.
			// * GetVertices() 함수의 리턴값은 const 형식입니다.
			// * const 형식을 제거하기 위해 const_cast<>()를 사용합니다.
			vector<VertexTextureData>& v = const_cast<vector<VertexTextureData>&>(_geometry->GetVertices());

			// 모든 정점을 순회하며 조작하기 위해 높이와 너비를 기준으로 인덱스를 맞춰줍니다.
			for (int32 z = 0; z < height; z++)
			{
				for (int32 x = 0; x < width; x++)
				{
					// 1차원 배열에서의 인덱스로 변환합니다.
					int32 idx = width * z + x;

					// 높이 정보를 저장합니다.
					// * 기존에 저장했던 heightMap의 pixelBuffer의 특정 인덱스에 있는 값을 적용합니다.
					// * 최대 높이가 25라고 가정했을 때의 보정 방법을 적용합니다.
					uint8 height = pixelBuffer[idx] / 255.0f * 25.0f;

					// 실제 사용중인 정점 정보들의 해당 인덱스에 접근해 높이를 변경합니다.
					v[idx].position.y = height;
				}
			}
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

		// 카메라의 시작 좌표를 수정합니다.
		_camera->GetTransform()->SetPosition(Vec3(0.0f, 5.0f, 0.0f));
		_camera->GetTransform()->SetRotation(Vec3(25.0f, 0.0f, 0.0f));
	}
}

void HeightMapDemo::Update()
{
	_camera->Update();
}

void HeightMapDemo::Render()
{
	// 행렬들을 Shader에 적용합니다.
	_shader->GetMatrix("World")->SetMatrix((float*)&_world);
	_shader->GetMatrix("View")->SetMatrix((float*)&Camera::S_MatView);
	_shader->GetMatrix("Projection")->SetMatrix((float*)&Camera::S_MatProjection);
	// 셰이더 파일에 텍스처를 전달합니다.
	_shader->GetSRV("Texture0")->SetResource(_texture->GetComPtr().Get());

	uint32 stride = _vertexBuffer->GetStride();
	uint32 offset = _vertexBuffer->GetOffset();

	// IA 단계에 VertexBuffer를 연결시켜줍니다.
	DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
	// IA 단계에 IndexBuffer를 연결시켜줍니다.
	DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);

	// 화면에 그려줍니다.
	_shader->DrawIndexed(0, 0, _indexBuffer->GetCount(), 0, 0);
}
