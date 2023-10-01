#pragma once

// 공용적으로 화면에 그려주기 위한 정보들을 담고 있는 클래스입니다.
// * Device, DeviceContext, SwapChain, RenderTargetView, Viewport
class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();

	// 렌더를 위한 준비 작업용 함수를 선언합니다.
	void	RenderBegin();
	// 최종 렌더 정보를 제출용 함수를 선언합니다.
	void	RenderEnd();

	// Device, DeviceContext를 반환하는 함수를 정의합니다.
	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }

private:
	// Device와 SwapChain을 생성하기 위한 함수를 선언합니다.
	void	CreateDeviceAndSwapChain();
	// 후면 버퍼를 묘사하는 View를 만들기 위한 함수를 선언합니다.
	void	CreateRenderTargetView();
	// viewPort를 설정하기 위한 함수를 선언합니다.
	// * 화면크기가 변할 때 1번만 세팅하면 됩니다.
	void	SetViewport();

private:
	// 윈도우 핸들 번호를 저장하기 위한 변수를 선언합니다.
	HWND	_hwnd;

	// 윈도우 창 크기를 저장하기 위한 변수를 선언합니다.
	uint32	_width = GWinSizeX;
	uint32	_height = GWinSizeY;
	
private:
	// Device & SwapChain
	// * 장치와 장치 문맥을 저장할 ComPtr 객체를 생성합니다.
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;

	// * SwapChain : 버퍼 공간에 장면을 미리 준비한 후 화면에 전환시키면서 깜빡임 현상을 해결
	// * IDXGISwapChain : DirectX에서 SwapChain을 사용하기 위한 인터페이스
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// RTV (Render Target View)
	// * 후면 버퍼를 묘사하는 View를 저장할 변수를 선언합니다.
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	// * 뷰포트 (화면을 묘사하는 구조체) 타입의 변수를 선언합니다.
	D3D11_VIEWPORT _viewport = { 0 };
	// * 화면을 초기화할 색상을 저장하기 위한 변수를 선언합니다.
	float _clearColor[4] = { 0.f, 0.f, 0.f, 0.f };

};

