#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include "Utils.h"
#include "tinyxml2.h"
#include "FileUtils.h"

Converter::Converter()
{
	_importer = make_shared<Assimp::Importer>();
}

Converter::~Converter()
{
}

void Converter::ReadAssetFile(wstring file)
{
	// 파일의 최종 경로를 저장합니다.
	wstring fileStr = _assetPath + file;

	auto p = std::filesystem::path(fileStr);

	// 파일이 존재하는지 확인합니다.
	assert(std::filesystem::exists(p));

	// Assimp 라이브러리를 사용하여 3D 모델 파일을 읽어옵니다.
	_scene = _importer->ReadFile(
		Utils::ToString(fileStr), 
		aiProcess_ConvertToLeftHanded | // 모델 데이터를 왼손 좌표계로 변환
		aiProcess_Triangulate |			// 다른 형태의 폴리곤을 삼각형으로 변환
		aiProcess_GenUVCoords |			// UV 좌표를 생성
		aiProcess_GenNormals |			// 각 정점의 법선 벡터를 자동으로 생성
		aiProcess_CalcTangentSpace      // 텍스처 좌표와 법선 벡터에 대한 접선 벡터를 계산
	);

	// 성공적으로 읽어왔는지 확인합니다.
	assert(_scene != nullptr);
}

void Converter::ExportModelData(wstring savePath)
{
	// 최종 경로를 설정합니다.
	wstring finalPath = _modelPath + savePath + L".Mesh";

	// Model 정보를 읽어옵니다. (계층 구조)
	// * 트리 형태를 읽어올 때는 재귀함수를 사용하는 것이 좋습니다.
	// * 처음에는 RootNode, -1, -1로 시작합니다.
	ReadModelData(_scene->mRootNode, -1, -1);

	// Skin 정보를 읽어옵니다.
	// * 기존에 VertexData에 추가했던 blendIndices(뼈대 번호[최대 4개]), blendWeights(비율[각각의 비율]) 정보
	// * 즉, 스키닝을 할 때는 정점 자체에 연관있는 뼈대 정보를 같이 기록한다는 것이 중요합니다.
	ReadSkinData();

	// 결과물을 편하게 보기 위해 csv 파일로 만들어줍니다.
	{
		FILE* file;
		::fopen_s(&file, "../Vertices.csv", "w");

		for (shared_ptr<asBone>& bone : _bones)
		{
			string name = bone->name;
			::fprintf(file, "%d,%s\n", bone->index, bone->name.c_str());
		}

		::fprintf(file, "\n");

		for (shared_ptr<asMesh>& mesh : _meshes)
		{
			string name = mesh->name;
			::printf("%s\n", name.c_str());

			for (UINT i = 0; i < mesh->vertices.size(); i++)
			{
				Vec3 p = mesh->vertices[i].position;
				Vec4 indices = mesh->vertices[i].blendIndices;
				Vec4 weights = mesh->vertices[i].blendWeights;

				::fprintf(file, "%f,%f,%f,", p.x, p.y, p.z);
				::fprintf(file, "%f,%f,%f,%f,", indices.x, indices.y, indices.z, indices.w);
				::fprintf(file, "%f,%f,%f,%f\n", weights.x, weights.y, weights.z, weights.w);
			}
		}

		::fclose(file);
	}

	// 최종적으로 읽어온 Model을 finalPath에 저장합니다.
	WriteModelFile(finalPath);
}

void Converter::ExportMaterialData(wstring savePath)
{
	// 최종 경로를 설정합니다.
	wstring finalPath = _texturePath + savePath + L".xml";

	// Material 정보를 읽어옵니다.
	ReadMaterialData();

	// 최종적으로 읽어온 Material을 finalPath에 저장합니다.
	WriteMaterialData(finalPath);
}

void Converter::ReadModelData(aiNode* node, int32 index, int32 parent)
{
	// Bone 정보를 저장하기 위한 변수를 선언합니다.
	shared_ptr<asBone> bone = make_shared<asBone>();
	// 전달받은 인자에 따라 데이터를 삽입합니다.
	bone->index = index;
	bone->parent = parent;
	bone->name = node->mName.C_Str();

	// 상대적인 Transform을 저장합니다. (직속 부모 기준)
	// * 해당 노드의 SRT 정보를 가져옵니다. (0번째 행렬[4x4] 주소 복사)
	Matrix transform(node->mTransformation[0]);
	// * FBX 포맷에서는 트랜스폼 정보가 뒤집혀 있으므로 다시 뒤집어 뼈대의 트랜스폼으로 설정합니다.
	bone->transform = transform.Transpose();

	// 위에서 구한 Transform 정보는 직속 부모를 기준으로 하는 Transform입니다.
	// -> 이를 해당 물체의 트랜스폼(Local)을 만들고 싶다면?
	// -> 부모의 Transform을 하나씩 타고 올라가며 곱해 주면 됩니다.
	{
		// Root (Local)
		Matrix matParent = Matrix::Identity;

		// 만약 부모가 존재한다면?
		if (parent >= 0)
		{
			// 부모의 Transform 정보를 저장합니다.
			// * 부모의 Transform 정보는 직속 부모부터 Root까지의 변환 행렬이므로, 하나씩 타고 올라가며 연산할 필요가 없습니다.
			//   (루트부터 재귀적으로 내려가고 있기 때문에 이미 부모의 transform에는 Root까지의 변환 행렬이 저장되어 있음)
			matParent = _bones[parent]->transform;
		}

		// Local (Root) Transform
		// * 자신의 Transform 정보에 부모의 Transform 정보를 곱해 Root를 기준으로 하는 Transform을 저장합니다.
		bone->transform = bone->transform * matParent;
	}
	// 생성한 bone을 추가해줍니다.
	_bones.push_back(bone);

	// Mesh 데이터를 읽어옵니다.
	ReadMeshData(node, index);

	// 해당 노드의 자식 수만큼 반복합니다. (재귀)
	for (uint32 i = 0; i < node->mNumChildren; i++)
		ReadModelData(node->mChildren[i], _bones.size(), index);
}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
	// Mesh가 존재하지 않으면 return 합니다.
	if (node->mNumMeshes < 1)
		return;

	// Mesh를 저장하기 위한 변수를 생성합니다.
	shared_ptr<asMesh> mesh = make_shared<asMesh>();
	// 전달받은 인자에 따라 데이터를 삽입합니다.
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	// sub Mesh가 여러 개 존재할 수 있습니다. Mesh의 수만큼 반복합니다.
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		// 해당 Mesh의 인덱스를 저장합니다.
		uint32 index = node->mMeshes[i];
		// 저장한 인덱스를 토대로 원본 Mesh를 가져옵니다.
		const aiMesh* srcMesh = _scene->mMeshes[index];

		// Mesh를 그리기 위한 Material 정보 또한 FBX에서 들고 있습니다.
		// * 우리만의 데이터로 변경해줍니다.

		// Material Name을 가져와 설정합니다.
		const aiMaterial* material = _scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->materialName = material->GetName().C_Str();

		// 지금까지 저장된 정점의 개수를 불러옵니다.
		// * sub Mesh가 여러개인 경우 인덱스 번호가 겹치지 않도록 하기 위해 마지막에 정점의 개수를 더해주는 용도로 사용할 것입니다.
		const uint32 startVertex = mesh->vertices.size();

		// 정점들을 순회하며 정보를 추출합니다.
		for (uint32 v = 0; v < srcMesh->mNumVertices; v++)
		{
			// Vertex를 저장하기 위한 변수를 선언합니다. (VertexTextureNormalTangentBlendData)
			VertexType vertex;
			// Position
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vec3));

			// UV
			if (srcMesh->HasTextureCoords(0))
				::memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Vec2));

			// Normal
			if (srcMesh->HasNormals())
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vec3));

			// ... 나중에 추가로 필요한 정보를 채워줄 수 있습니다.

			// 추출한 정보를 mesh의 Vertices 배열에 추가합니다.
			mesh->vertices.push_back(vertex);
		}

		// Index를 기준으로 순회합니다.
		for (uint32 f = 0; f < srcMesh->mNumFaces; f++)
		{
			// 인덱스 정보를 추출합니다.
			aiFace& face = srcMesh->mFaces[f];

			for (uint32 k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.push_back(face.mIndices[k] + startVertex);
			}
		}
	}

	// 생성한 mesh를 추가해줍니다.
	_meshes.push_back(mesh);
}

void Converter::ReadMaterialData()
{
	// Material의 수만큼 반복합니다.
	for (uint32 i = 0; i < _scene->mNumMaterials; i++)
	{
		// 원본 머터리얼을 받아옵니다.
		aiMaterial* srcMaterial = _scene->mMaterials[i];

		// Custom 타입의 머터리얼을 생성합니다. (실제 저장 용도)
		shared_ptr<asMaterial> material = make_shared<asMaterial>();

		// Custom 타입의 머터리얼에 정보를 채워줍니다.
		{
			// * 이름
			material->name = srcMaterial->GetName().C_Str();

			// * 색상
			aiColor3D color;
			// -> Ambient
			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			material->ambient = Color(color.r, color.g, color.b, 1.0f);
			// -> Diffuse
			srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			material->diffuse = Color(color.r, color.g, color.b, 1.0f);
			// -> Specular
			srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			material->specular = Color(color.r, color.g, color.b, 1.0f);
			//    Specular의 경우 pow를 통해 강도를 연산해준 것처럼 w에 해당 값이 들어오게 됩니다.
			srcMaterial->Get(AI_MATKEY_SHININESS, material->specular.w);
			// -> Emissive
			srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			material->emissive = Color(color.r, color.g, color.b, 1.0f);

			// * 사용할 텍스처 경로
			aiString file;
			// -> Diffuse Texture
			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			material->diffuseFile = file.C_Str();
			// -> Specular Texture
			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			material->specularFile = file.C_Str();
			// -> Normal Texture
			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			material->normalFile = file.C_Str();
		}

		// 생성한 머터리얼 정보를 추가합니다.
		_materials.push_back(material);
	}
}

void Converter::WriteModelFile(wstring finalPath)
{
	// 경로를 설정합니다.
	auto path = filesystem::path(finalPath);

	// 폴더를 생성합니다.
	filesystem::create_directory(path.parent_path());

	// 파일을 열어줍니다.
	shared_ptr<FileUtils> file = make_shared<FileUtils>();
	file->Open(finalPath, FileMode::Write);

	// 실제 파일안에 데이터를 넣어줍니다.
	// * Bone Data
	file->Write<uint32>(_bones.size());
	for (shared_ptr<asBone>& bone : _bones)
	{
		file->Write<int32>(bone->index);
		file->Write<string>(bone->name);
		file->Write<int32>(bone->parent);
		file->Write<Matrix>(bone->transform);
	}

	// * Mesh Data
	file->Write<uint32>(_meshes.size());
	for (shared_ptr<asMesh>& meshData : _meshes)
	{
		file->Write<string>(meshData->name);
		file->Write<int32>(meshData->boneIndex);
		file->Write<string>(meshData->materialName);

		// Vertex Data
		file->Write<uint32>(meshData->vertices.size());
		file->Write(&meshData->vertices[0], sizeof(VertexType) * meshData->vertices.size());

		// Index Data
		file->Write<uint32>(meshData->indices.size());
		file->Write(&meshData->indices[0], sizeof(uint32) * meshData->indices.size());
	}
}

void Converter::WriteMaterialData(wstring finalPath)
{
	// 전체 경로를 설정합니다.
	auto path = filesystem::path(finalPath);

	// 해당 경로의 바로 상위에 폴더를 생성합니다. (폴더가 없을 시 생성)
	filesystem::create_directory(path.parent_path());
	// 생성한 폴더의 이름을 가져옵니다.
	string folder = path.parent_path().string();

	// XML 생성
	{
		// 문서를 생성합니다.
		shared_ptr<tinyxml2::XMLDocument> document = make_shared<tinyxml2::XMLDocument>();
		// 포맷 설정
		tinyxml2::XMLDeclaration* decl = document->NewDeclaration();
		document->LinkEndChild(decl);

		// 새로운 요소를 생성하고 연결합니다.
		tinyxml2::XMLElement* root = document->NewElement("Materials");
		document->LinkEndChild(root);

		// 내부 데이터들을 채워줍니다.
		for (shared_ptr<asMaterial> material : _materials)
		{
			tinyxml2::XMLElement* node = document->NewElement("Material");
			root->LinkEndChild(node);

			tinyxml2::XMLElement* element = nullptr;

			element = document->NewElement("Name");
			element->SetText(material->name.c_str());
			node->LinkEndChild(element);

			element = document->NewElement("DiffuseFile");
			element->SetText(WriteTexture(folder, material->diffuseFile).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("SpecularFile");
			element->SetText(WriteTexture(folder, material->specularFile).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("NormalFile");
			element->SetText(WriteTexture(folder, material->normalFile).c_str());
			node->LinkEndChild(element);

			element = document->NewElement("Ambient");
			element->SetAttribute("R", material->ambient.x);
			element->SetAttribute("G", material->ambient.y);
			element->SetAttribute("B", material->ambient.z);
			element->SetAttribute("A", material->ambient.w);
			node->LinkEndChild(element);

			element = document->NewElement("Diffuse");
			element->SetAttribute("R", material->diffuse.x);
			element->SetAttribute("G", material->diffuse.y);
			element->SetAttribute("B", material->diffuse.z);
			element->SetAttribute("A", material->diffuse.w);
			node->LinkEndChild(element);

			element = document->NewElement("Specular");
			element->SetAttribute("R", material->specular.x);
			element->SetAttribute("G", material->specular.y);
			element->SetAttribute("B", material->specular.z);
			element->SetAttribute("A", material->specular.w);
			node->LinkEndChild(element);

			element = document->NewElement("Emissive");
			element->SetAttribute("R", material->emissive.x);
			element->SetAttribute("G", material->emissive.y);
			element->SetAttribute("B", material->emissive.z);
			element->SetAttribute("A", material->emissive.w);
			node->LinkEndChild(element);
		}

		// 최종 경로에 파일을 저장합니다.
		document->SaveFile(Utils::ToString(finalPath).c_str());
	}
}

void Converter::ReadSkinData()
{
	// 메쉬를 순회합니다.
	for (uint32 i = 0; i < _scene->mNumMeshes; i++)
	{
		// 인덱스에 맞는 원본 메시를 가져옵니다.
		aiMesh* srcMesh = _scene->mMeshes[i];
		// 만약 원본 메시의 뼈가 존재하지 않는다면? -> 스킨 데이터가 없는 경우이므로 해당 메시를 건너뜁니다.
		if (srcMesh->HasBones() == false) continue;

		// 우리만의 타입으로 메시를 가져옵니다.
		shared_ptr<asMesh> mesh = _meshes[i];

		// 정점마다 가지고 있는 BoneWeight를 저장하기 위한 임시 벡터를 선언합니다.
		vector<asBoneWeights> tempVertexBoneWeights;
		// 해당 메시의 bone 수만큼 초기화합니다.
		tempVertexBoneWeights.resize(mesh->vertices.size());

		// 해당 메쉬의 Bone을 하나씩 순회하며 연관된 Vertex_ID(뼈), Weight(가중치)를 구해서 저장합니다.
		for (uint32 b = 0; b < srcMesh->mNumBones; b++)
		{
			// 인덱스에 맞는 원본 Bone을 가져옵니다.
			aiBone* srcMeshBone = srcMesh->mBones[b];
			// 위에서 가져온 Bone이 몇 번째 Bone인지를 구해줍니다.
			uint32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());

			// 해당 Bone에 들어있는 가중치 정보들을 순회합니다.
			for (uint32 w = 0; w < srcMeshBone->mNumWeights; w++)
			{
				// 몇 번째 뼈에 영향을 받고(vertexId) 비율은 얼마인지(weight)를 추출합니다.
				uint32 index = srcMeshBone->mWeights[w].mVertexId;
				float weight = srcMeshBone->mWeights[w].mWeight;

				// 인덱스에 맞는 정점의 BoneWeight에 정보를 추가합니다.
				tempVertexBoneWeights[index].AddWeights(boneIndex, weight);
			}
		}

		// 최종 결과 계산
		// * 정점마다 저장된 BoneWeight를 순회하며 비율을 맞추고 해당 정점에 저장합니다.
		for (uint32 v = 0; v < tempVertexBoneWeights.size(); v++)
		{
			tempVertexBoneWeights[v].Normalize();
			asBlendWeight blendWeight = tempVertexBoneWeights[v].GetBlendWeights();

			mesh->vertices[v].blendIndices = blendWeight.indices;
			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
		
	}
}

uint32 Converter::GetBoneIndex(const string& name)
{
	// 모든 bone 정보를 순회하며 일치하는 이름이 존재하면 해당 인덱스를 반환합니다.
	for (shared_ptr<asBone>& bone : _bones)
	{
		if (bone->name == name)
		{
			return bone->index;
		}
	}

	assert(false);
	return 0;
}

string Converter::WriteTexture(string saveFolder, string file)
{
	// 파일 이름을 불러옵니다. 
	string fileName = filesystem::path(file).filename().string();
	// 폴더 이름을 불러옵니다.
	string folderName = filesystem::path(saveFolder).filename().string();

	// 텍스처가 모델 파일 자체에 포함되어 있는 경우 파일의 경로에 따라 Embedded Texture를 가져옵니다.
	const aiTexture* srcTexture = _scene->GetEmbeddedTexture(file.c_str());
	if (srcTexture)
	{
		string pathStr = saveFolder + fileName;

		if (srcTexture->mHeight == 0)
		{
			shared_ptr<FileUtils> file = make_shared<FileUtils>();
			file->Open(Utils::ToWString(pathStr), FileMode::Write);
			file->Write(srcTexture->pcData, srcTexture->mWidth);
		}
		else
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = srcTexture->mWidth;
			desc.Height = srcTexture->mHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			D3D11_SUBRESOURCE_DATA subResource = { 0 };
			subResource.pSysMem = srcTexture->pcData;

			ComPtr<ID3D11Texture2D> texture;
			HRESULT hr = DEVICE->CreateTexture2D(&desc, &subResource, texture.GetAddressOf());
			CHECK(hr);

			DirectX::ScratchImage img;
			::CaptureTexture(DEVICE.Get(), DC.Get(), texture.Get(), img);

			// Save To File
			hr = DirectX::SaveToDDSFile(*img.GetImages(), DirectX::DDS_FLAGS_NONE, Utils::ToWString(fileName).c_str());
			CHECK(hr);
		}
	}
	// 일반적으로 텍스처가 내장되어있지 않은 경우입니다.
	// * 원본 파일 경로에 접근해 다른 경로로 이동시키는 것이 목적입니다.
	else
	{
		// src 경로
		string originStr = (filesystem::path(_assetPath) / folderName / file).string();
		Utils::Replace(originStr, "\\", "/");

		// dest 경로
		string pathStr = (filesystem::path(saveFolder) / fileName).string();
		Utils::Replace(pathStr, "\\", "/");

		// src 경로에 위치한 파일을 dest 경로에 복사합니다.
		::CopyFileA(originStr.c_str(), pathStr.c_str(), false);
	}

	return fileName;
}