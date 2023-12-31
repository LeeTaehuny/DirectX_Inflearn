﻿#pragma once
#include "AsTypes.h"

// Assimp를 가져와 로드하는 기능의 클래스입니다.
class Converter
{
public:
	Converter();
	~Converter();

public:
	// 애셋을 읽기 위한 함수를 선언합니다. (FBX -> Memory)
	void ReadAssetFile(wstring file);

	// _scene에 저장된 정보를 가져와 별도의 ModelData파일로 만들어주기 위한 함수를 선언합니다.
	void ExportModelData(wstring savePath);
	// _scene에 저장된 정보를 가져와 별도의 Material파일로 만들어주기 위한 함수를 선언합니다.
	void ExportMaterialData(wstring savePath);
	// _scene에 저장된 정보를 가져와 애니메이션 데이터를 추출하기 위한 함수를 선언합니다.
	void ExportAnimationData(wstring savePath, uint32 index = 0);

private:
	// 읽어온 정보 중 Model을 읽어오기 위한 함수를 선언합니다.
	void ReadModelData(aiNode* node, int32 index, int32 parent);
	// 읽어온 정보 중 Mesh를 읽어오기 위한 함수를 선언합니다.
	void ReadMeshData(aiNode* node, int32 bone);

	// Model 파일을 저장하기 위한 함수를 선언합니다.
	void WriteModelFile(wstring finalPath);
	// Texture 파일을 Material Data에 저장하기 위한 함수를 선언합니다.
	string WriteTexture(string saveFolder, string file);

private:
	// 읽어온 정보 중 Material을 읽어오기 위한 함수를 선언합니다.
	void ReadMaterialData();
	// Material 파일을 저장하기 위한 함수를 선언합니다.
	void WriteMaterialData(wstring finalPath);

private:
	// 읽어온 정보 중 애니메이션 데이터를 우리만의 자료형으로 변환시키기 위한 함수를 선언합니다.
	shared_ptr<asAnimation> ReadAnimationData(aiAnimation* srcAnimation);
	// 애니메이션 노드를 파싱하기 위한 함수를 선언합니다.
	shared_ptr<asAnimationNode> ParseAnimationNode(shared_ptr<asAnimation> animation, aiNodeAnim* srcNode);
	// 키 프레임 데이터를 읽어오기 위한 함수를 선언합니다.
	void ReadKeyframeData(shared_ptr<asAnimation> animation, aiNode* srcNode, map<string, shared_ptr<asAnimationNode>>& cache);
	// 읽어온 키 프레임 데이터를 저장하기 위한 함수를 선언합니다.
	void WriteAnimationData(shared_ptr<asAnimation> animation, wstring finalPath);

private:
	// 읽어온 정보 중 Skin과 관련된 정보를 읽어오기 위한 함수를 선언합니다.
	void ReadSkinData();
	// 이름을 전달받아 몇 번째 Bone인지 인덱스를 반환하기 위한 함수를 선언합니다.
	uint32 GetBoneIndex(const string& name);

private:
	// 필요한 것을 로드하기 위한 상대 경로들을 저장합니다.
	// * 애셋
	wstring _assetPath = L"../Resources/Assets/";
	// * 모델
	wstring _modelPath = L"../Resources/Models/";
	// * 텍스처(머터리얼)
	wstring _texturePath = L"../Resources/Textures/";

private:
	// Assimp를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Assimp::Importer> _importer;
	// 읽어들인 3D모델의 전반적인 정보를 저장하기 위한 구변수를 선언합니다.
	const aiScene* _scene;

private:
	// Bone 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<asBone>> _bones;
	// 메쉬 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<asMesh>> _meshes;
	// 머터리얼 정보를 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<shared_ptr<asMaterial>> _materials;
};

