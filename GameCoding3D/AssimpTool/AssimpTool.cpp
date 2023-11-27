﻿#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{

	{
		// 여러 포맷의 파일들을 불러오기 위한 변환기(converter)를 생성합니다.
		shared_ptr<Converter> converter = make_shared<Converter>();
		// 변환기(converter)를 사용해 House Asset을 불러옵니다. (FBX -> Memory)
		converter->ReadAssetFile(L"House/House.fbx");

		// Memory -> CustomData(File)
		// * 읽은 정보로부터 필요한 데이터를 추출해 저장합니다.
		converter->ExportMaterialData(L"House/House");
		converter->ExportModelData(L"House/House");

		// CustomData - > Memory
	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
