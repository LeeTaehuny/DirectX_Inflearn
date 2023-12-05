#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	// Mesh
	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");
	}

	// Idle
	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Idle.fbx");
		converter->ExportAnimationData(L"Kachujin/Idle");
	}
	// Run
	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Run.fbx");
		converter->ExportAnimationData(L"Kachujin/Run");
	}
	// Slash
	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Slash.fbx");
		converter->ExportAnimationData(L"Kachujin/Slash");
	}

}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
