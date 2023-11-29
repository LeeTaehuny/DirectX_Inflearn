#pragma once

struct ModelBone;
struct ModelMesh;

// ModelMesh에서 생성한 구조체들을 최종적으로 사용하기 위한 클래스입니다.
// * this 포인터를 사용하기 위해 enable_shared_from_this<Model>를 상속받아줍니다.
class Model : public enable_shared_from_this<Model>
{
public:
	Model();
	~Model();
	
public:
	// 우리만의 포맷으로 저장한 정보들을 불러오기 위한 함수를 선언합니다.
	void ReadMaterial(wstring filename);
	void ReadModel(wstring filename);

public:
	uint32 GetMaterialCount() { return static_cast<uint32>(_materials.size()); }
	vector<shared_ptr<Material>>& GetMaterials() { return _materials; }
	shared_ptr<Material> GetMaterialByIndex(uint32 index) { return _materials[index]; }
	shared_ptr<Material> GetMaterialByName(const wstring& name);

	uint32 GetMeshCount() { return static_cast<uint32>(_meshes.size()); }
	vector<shared_ptr<ModelMesh>>& GetMeshes() { return _meshes; }
	shared_ptr<ModelMesh> GetMeshByIndex(uint32 index) { return _meshes[index]; }
	shared_ptr<ModelMesh> GetMeshByName(const wstring& name);

	uint32 GetBoneCount() { return static_cast<uint32>(_bones.size()); }
	vector<shared_ptr<ModelBone>>& GetBones() { return _bones; }
	shared_ptr<ModelBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _bones.size() ? nullptr : _bones[index]); }
	shared_ptr<ModelBone> GetBoneByName(const wstring& name);

private:
	// 생성한 머터리얼과 모델들에 대해 
	void BindCacheInfo();

private:
	// Model/Texture 경로를 저장하기 위한 변수를 선언합니다.
	wstring _modelPath = L"../Resources/Models/";
	wstring _texturePath = L"../Resources/Textures/";

private:
	// 필요한 정보들을 저장하기 위한 변수를 선언합니다.
	// * Root Bone
	shared_ptr<ModelBone> _root;
	// * 머터리얼
	vector<shared_ptr<Material>> _materials;
	// * bone들의 정보
	vector<shared_ptr<ModelBone>> _bones;
	// * mesh들의 정보
	vector<shared_ptr<ModelMesh>> _meshes;

};

