#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device) : _device(device)
{

}

RasterizerState::~RasterizerState()
{

}

void RasterizerState::Create()
{
	// RasterizerState 생성에 사용될 정보를 생성합니다.
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// 그림을 채우는 형태에 대한 설정 (SOLID : 가득 채우기, WIREFRAME : 삼각형 정보만 표시)
	desc.FillMode = D3D11_FILL_SOLID;
	// 컬링모드 설정 (BACK : 그림의 후면으로 인식하면 출력 X, FRONT : 그림의 전면으로 인식하면 출력 X, NONE : 컬링 X)
	// * 컬링 : 그리지 않게 하는 설정 (Skip)
	desc.CullMode = D3D11_CULL_BACK;
	// 앞 방향을 반시계방향으로 표현할 것인가에 대한 정보 (falase : 시계방향이 앞방향(인덱스 버퍼))
	desc.FrontCounterClockwise = false;

	// RasterizerState를 생성합니다.
	HRESULT hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}
