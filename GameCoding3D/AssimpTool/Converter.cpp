#include "pch.h"
#include "Converter.h"
#include <filesystem>
#include "Utils.h"
#include "tinyxml2.h"

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

}

void Converter::ReadMeshData(aiNode* node, int32 bone)
{
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

string Converter::WriteTexture(string saveFolder, string file)
{
	return "";
}
