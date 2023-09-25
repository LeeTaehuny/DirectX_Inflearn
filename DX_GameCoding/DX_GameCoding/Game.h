#pragma once
#include <vector>

class Game
{
public:
	Game();
	~Game();

public:
	// 초기화 함수를 선언합니다.
	void	Init(HWND hwnd);
	// 업데이트 함수를 선언합니다.
	void	Update();
	// 렌더 함수를 선언합니다.
	void	Render();

private:
	// 렌더를 위한 준비 작업용 함수를 선언합니다.
	void	RenderBegin();
	// 최종 렌더 정보를 제출용 함수를 선언합니다.
	void	RenderEnd();

private:
	// Device와 SwapChain을 생성하기 위한 함수를 선언합니다.
	void	CreateDeviceAndSwapChain();
	// 후면 버퍼를 묘사하는 View를 만들기 위한 함수를 선언합니다.
	void	CreateRenderTargetView();
	// viewPort를 설정하기 위한 함수를 선언합니다.
	// * 화면크기가 변할 때 1번만 세팅하면 됩니다.
	void	SetViewport();

// 삼각형 띄우기
private:
	// 기하학적인 도형을 만들기 위한 함수를 선언합니다.
	void	CreateGeometry();

	// 생성한 기하학적인 도형이 어떻게 되어있는지 묘사해야 합니다.
	// * input layout을 생성하기 위한 함수를 선언합니다.
	void	CreateInputLayout();

	// VS를 생성하기 위한 함수를 선언합니다.
	void	CreateVS();
	// PS를 생성하기 위한 함수를 선언합니다.
	void	CreatePS();

	// 셰이더는 파일을 로드하는 방식으로 만들어줘야 합니다.
	// * 공용으로 셰이더를 파일로부터 로드하기 위한 함수를 선언합니다.
	// * path : 경로
	// * name : 이름
	// * version : 셰이더 버전
	// * blob : 로드한 셰이더 결과물을 저장할 변수
	void	LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	// 윈도우 핸들 번호를 저장하기 위한 변수를 선언합니다.
	HWND	_hwnd;

	// 윈도우 창 크기를 저장하기 위한 변수를 선언합니다.
	uint32	_width = 0;
	uint32	_height = 0;

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

private:
	// Geometry
	// * 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<Vertex> _vertices;

	// _vertices의 데이터들은 CPU의 메모리에 존재합니다.
	// GPU의 메모리(VRAM)으로 옮겨줄 필요가 있습니다.
	
	// * 정점 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;

	// * 입력 레이아웃을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	// * VS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	// * 임의 길이 데이터를 반환하는 데 사용할 변수를 선언합니다.
	ComPtr<ID3DBlob> _vsBlob;

	// PS
	// * PS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	// * 임의 길이 데이터를 반환하는 데 사용할 변수를 선언합니다.
	ComPtr<ID3DBlob> _psBlob;
};

