#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd)
{
	_hwnd = hwnd;

	// 생성과 동시에 Device, DeviceContext, Viewport를 생성해줍니다.
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{
}

void Graphics::RenderBegin()
{
	// OutputMerger단계에 RenderTarget을 설정합니다.
	// * RenderTarget : _renderTargetView (후면 버퍼)
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);

	// RenderTarget을 초기 색상으로 초기화합니다.
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 파이프라인에서 RS의 설정입니다.
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	// 그려진 버퍼를 전면 버퍼에 제출(출력)합니다.
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	// swapChainDesc를 생성합니다.
	DXGI_SWAP_CHAIN_DESC desc;
	// desc의 내용을 0으로 초기화합니다.
	ZeroMemory(&desc, sizeof(desc));
	// desc의 내용을 다시 채워줍니다.
	{
		// 창의 크기만큼 버퍼의 크기를 설정 (픽셀 단위로 정보를 채우는 형식으로 동작)
		desc.BufferDesc.Width = GWinSizeX;
		desc.BufferDesc.Height = GWinSizeY;

		// 화면 주사율과 관련된 옵션
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		// 포맷 정보 설정
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// 멀티 샘플링 관련
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		// 버퍼의 사용 용도 설정 (최종적으로 화면에 그려주기 위한 용도)
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// 후면 버퍼의 숫자 설정
		desc.BufferCount = 1;
		// 출력할 윈도우 핸들 번호
		desc.OutputWindow = _hwnd;

		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}


	// Device와 SwapChain을 한 번에 생성합니다.
	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,		// 하드웨어의 그래픽 카드를 사용
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,							// swapChainDesc
		_swapChain.GetAddressOf(),		// &swapChain (만약 comptr 사용 X라면) - 결과물을 swapChain에 넣어줌
		_device.GetAddressOf(),			// &device    (만약 comptr 사용 X라면) - 결과물을 device에 넣어줌
		nullptr,
		_deviceContext.GetAddressOf()   // &deviceContext    (만약 comptr 사용 X라면) - 결과물을 deviceContext에 넣어줌
	);

	// _device.Get()          -> ID3D11Device* 반환 (디바이스 반환)
	// _device.GetAddressOf() -> ID3D11Device** 반환 (디바이스의 주소 반환)

	// 모든 정보가 제대로 생성되었는지 체크
	// * 실패하면 crash가 나도록 유도 (pch에서 assert 매크로 선언)
	CHECK(hr);
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	// 후면 버퍼를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	// swapChain에 저장된 후면 버퍼를 ID3D11Texture2D 타입으로 backBuffer에 넣어줍니다.
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());

	// 모든 정보가 제대로 생성되었는지 체크
	// * 실패하면 crash가 나도록 유도 (pch에서 assert 매크로 선언)
	CHECK(hr);

	// Device에서 뷰를 생성합니다.
	// * backBuffer에 저장되어 있는 후면 버퍼를 _renderTargetView의 주소에 저장합니다.
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);
}

void Graphics::SetViewport()
{
	// 뷰포트에 대한 정보를 채워줍니다.
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}
