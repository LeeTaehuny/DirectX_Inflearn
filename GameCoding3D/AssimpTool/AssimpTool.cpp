#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	// TOWER
	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();

		converter->ReadAssetFile(L"Kachujin/Mesh.fbx");
		converter->ExportMaterialData(L"Kachujin/Kachujin");
		converter->ExportModelData(L"Kachujin/Kachujin");

	}

}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
