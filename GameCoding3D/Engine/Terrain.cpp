#include "pch.h"
#include "Terrain.h"
#include "MeshRenderer.h"
#include "Camera.h"

Terrain::Terrain() : Super(ComponentType::Terrain)
{
}

Terrain::~Terrain()
{
}

void Terrain::Create(int32 sizeX, int32 sizeZ, shared_ptr<Material> material)
{
	// 전달받은 변수를 저장합니다.
	_sizeX = sizeX;
	_sizeZ = sizeZ;

	// 게임 오브젝트(부모)를 받아옵니다.
	auto go = _gameObject.lock();
	// 부모에 Transform 정보를 추가합니다.
	go->GetOrAddTransform();

	// MeshRenderer 컴포넌트가 존재하지 않는다면 출력이 되지 않습니다.
	// * 가지고 있지 않는경우 새로 생성해주도록 합니다.
	if (go->GetMeshRenderer() == nullptr)
	{
		// MeshRenderer 컴포넌트를 추가해줍니다.
		go->AddComponent(make_shared<MeshRenderer>());
	}

	// 전달받은 값을 토대로 Grid를 생성해줍니다. (Mesh 생성)
	_mesh = make_shared<Mesh>();
	_mesh->CreateGrid(sizeX, sizeZ);

	// MeshRenderer 컴포넌트에 필요한 정보들을 넣어줍니다.
	go->GetMeshRenderer()->SetMesh(_mesh);
	go->GetMeshRenderer()->SetPass(0);
	go->GetMeshRenderer()->SetMaterial(material);
}

bool Terrain::Pick(int32 screenX, int32 screenY, Vec3& pickPos, float& distance)
{
	// Viewport에서 만든 UnProject()를 사용해 구현해보도록 하겠습니다.

	// W, V, P 변환 행렬을 가져옵니다.
	Matrix W = GetTransform()->GetWorldMatrix();
	Matrix V = Camera::S_MatView;
	Matrix P = Camera::S_MatProjection;

	// Viewport에 만들어둔 함수를 통해 2D 좌표를 3D 좌표로 변환해줍니다.
	// * viewport를 가져옵니다.
	Viewport& vp = GRAPHICS->GetViewport();
	// * UnProject() 함수를 통해 3D(World)상의 좌표(near, far)를 가져옵니다.
	Vec3 n = vp.UnProject(Vec3(screenX, screenY, 0.0f), W, V, P);
	Vec3 f = vp.UnProject(Vec3(screenX, screenY, 1.0f), W, V, P);

	// 위에서 가져온 좌표들을 이용해 Ray를 생성합니다.
	Vec3 start = n;
	Vec3 direction = f - n;
	direction.Normalize();

	// * near 위치의 좌표에서 far 좌표 방향으로 발사되는 광선
	Ray ray = Ray(start, direction);

	// Grid의 정점들을 가져옵니다.
	const auto& vertices = _mesh->GetGeometry()->GetVertices();

	// 정점들을 순회하며 삼각형마다 Picking하는 작업을 진행해줍니다.
	for (int32 z = 0; z < _sizeZ; z++)
	{
		for (int32 x = 0; x < _sizeX; x++)
		{
			// * Grid를 만들 때는 인덱스를 통해 정점들을 배치하였습니다.
			// * 인덱스는 삼각형 2개를 만들기 위해 6개가 필요하지만 실질적으로 들어가는 정점의 개수는 4개입니다.
			// * 인덱스와 맞는 정점을 불러오기 위해 인덱스 배열을 만들어 인덱스 정보를 저장해줍니다.
			uint32 index[4];
			index[0] = (_sizeX + 1) * z + x;
			index[1] = (_sizeX + 1) * z + (x + 1);
			index[2] = (_sizeX + 1) * (z + 1) + x;
			index[3] = (_sizeX + 1) * (z + 1) + (x + 1);

			// 위에서 만들어둔 인덱스를 바탕으로 인덱스에 해당되는 정점의 위치를 찾아주도록 합니다.
			Vec3 p[4];
			for (int32 i = 0; i < 4; i++)
			{
				p[i] = vertices[index[i]].position;
			}

			// 삼각형마다 충돌이 발생했는지 체크해주도록 합니다.
			// [2]
			//  |  \
			// [0] - [1]
			if (ray.Intersects(p[0], p[1], p[2], OUT distance))
			{
				pickPos = ray.position + ray.direction * distance;
				return true;
			}

			// [2] - [3]
			//     \  |
			//       [1]
			if (ray.Intersects(p[3], p[1], p[2], OUT distance))
			{
				pickPos = ray.position + ray.direction * distance;
				return true;
			}
		}
	}

	// 전부 실패한 경우 충돌하지 않았다고 반환합니다.
	return false;
}
