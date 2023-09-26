#include "pch.h"
#include "Game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hwnd)
{
	// 멤버 변수 초기화
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateSRV();
}

void Game::Update()
{
}

void Game::Render()
{
	// 렌더를 위한 준비 작업
	RenderBegin();

	// TODO : 렌더
	// IA - VS - RS - PS - OM
	{
		// IA
		{
			// * stride : Vertex 구조체의 크기
			uint32 stride = sizeof(Vertex);
			// * offset : 보간 수치
			uint32 offset = 0;

			// 디바이스 컨텍스트를 이용해 IA에 정점 버퍼를 연결시켜줍니다.
			_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
			// 디바이스 컨텍스트를 이용해 IA에 인덱스 버퍼를 연결시켜줍니다.
			_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			// 디바이스 컨텍스트를 이용해 IA에 InputLayout 정보를 연결시켜줍니다.
			_deviceContext->IASetInputLayout(_inputLayout.Get());
			// 삼각형(대부분 모든 사물은 삼각형으로 표현)을 그리는 과정에서 전달한 정점들을 어떤 순서로 이어 붙일 것인지에 대한 정보를 지정합니다. (topology)
			_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		// VS
		{
			// 디바이스 컨텍스트를 이용해 VS에 만든 셰이더를 연결시켜줍니다.
			_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		}

		// RS
		{

		}

		// PS
		{
			// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더를 연결시켜줍니다.
			_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
			// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더 리소스 뷰를 연결시켜줍니다.
			_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
			_deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());
		}

		// OM
		{

		}

		// 디바이스 컨텍스트를 이용해 해당 물체를 그려달라고 요청합니다.
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	// 최종 렌더 정보를 제출
	RenderEnd();
}

void Game::RenderBegin()
{
	// OutMerge단계에 RenderTarget을 설정합니다.
	// * RenderTarget : _renderTargetView (후면 버퍼)
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	
	// RenderTarget을 초기 색상으로 초기화합니다.
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	// 파이프라인에서 RS의 설정입니다.
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Game::RenderEnd()
{
	// 그려진 버퍼를 전면 버퍼에 제출(출력)합니다.
	HRESULT hr = _swapChain->Present(1, 0);
	CHECK(hr);
}

void Game::CreateDeviceAndSwapChain()
{
	// swapChainDesc를 생성합니다.
	DXGI_SWAP_CHAIN_DESC desc;
	// desc의 내용을 0으로 초기화합니다.
	ZeroMemory(&desc, sizeof(desc));
	// desc의 내용을 다시 채워줍니다.
	{
		// 창의 크기만큼 버퍼의 크기를 설정 (픽셀 단위로 정보를 채우는 형식으로 동작)
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;

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

void Game::CreateRenderTargetView()
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

void Game::SetViewport()
{
	// 뷰포트에 대한 정보를 채워줍니다.
	_viewport.TopLeftX = 0.0f;
	_viewport.TopLeftY = 0.0f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.0f;
	_viewport.MaxDepth = 1.0f;
}

void Game::CreateGeometry()
{
	// 1 3
	// 0 2

	// Vertex Data
	{
		// 사이즈 조정
		_vertices.resize(4);

		// 위치 정보와 uv 좌표를 설정합니다.
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.0f);
		_vertices[0].uv = Vec2(0.0f, 1.0f);
		//_vertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);		

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.0f);
		_vertices[1].uv = Vec2(0.0f, 0.0f);
		//_vertices[1].color = Color(1.0f, 0.0f, 0.0f, 1.0f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.0f);
		_vertices[2].uv = Vec2(1.0f, 1.0f);
		//_vertices[2].color = Color(1.0f, 0.0f, 0.0f, 1.0f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.0f);
		_vertices[3].uv = Vec2(1.0f, 0.0f);
		//_vertices[3].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	}

	// VertexBuffer
	{
		// 버퍼 생성에 사용될 DESC를 생성합니다.
		D3D11_BUFFER_DESC desc;
		// 0으로 초기화해줍니다.
		ZeroMemory(&desc, sizeof(desc));
		// * D3D11_USAGE_IMMUTABLE : GPU만 읽을 수 있는 방식으로 사용하겠다고 설정
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// * D3D11_BIND_VERTEX_BUFFER : VertexBuffer를 바인딩하는 용도로 사용하겠다고 설정
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		// * desc의 바이트 크기를 설정합니다. (데이터 유형의 크기 * 수)
		desc.ByteWidth = (uint32)sizeof(Vertex) * _vertices.size();

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		// 첫 번째 데이터의 시작 주소를 저장합니다.
		data.pSysMem = _vertices.data();

		// 버퍼를 생성해줍니다. (_vertexBuffer에 결과물을 저장합니다.)
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		CHECK(hr);
	}

	// index
	{
		// 인덱스 컨테이너에 정보를 저장합니다.
		// * 저장하는 방향은 일정해야 합니다.
		_indices = { 0, 1, 2, 2, 1, 3 };
	}

	// indexBuffer
	{
		// 버퍼 생성에 사용될 DESC를 생성합니다.
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		// * D3D11_BIND_INDEX_BUFFER : IndexBuffer를 바인딩하는 용도로 사용하겠다고 설정
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)sizeof(uint32) * _indices.size();

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		// 첫 번째 데이터의 시작 주소를 저장합니다.
		data.pSysMem = _indices.data();

		// 버퍼를 생성해줍니다. (_indexBuffer에 결과물을 저장합니다.)
		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		CHECK(hr);
	}
}

void Game::CreateInputLayout()
{
	// 입력 요소에 대한 정보를 생성합니다.
	// * Vertex 구조체의 내부 요소들에 대해 묘사합니다.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// layout의 개수를 저장합니다.
	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	// 입력 버퍼 데이터를 설명하는 입력 레이아웃 개체를 만듭니다.
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// 만든 셰이더 파일로부터 _vsBlob에 임시 데이터를 불러와 로드합니다.
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// VS를 생성합니다.
	HRESULT hr = _device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	// 만든 셰이더 파일로부터 _psBlob에 임시 데이터를 불러와 로드합니다.
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// PS를 생성합니다.
	HRESULT hr = _device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;

	DirectX::TexMetadata md2;
	DirectX::ScratchImage img2;

	// 이미지를 불러옵니다. (DirectXTex 라이브러리 사용)
	HRESULT hr = ::LoadFromWICFile(L"test.png", WIC_FLAGS_NONE, &md, img);
	CHECK(hr);

	hr = ::LoadFromWICFile(L"test2.png", WIC_FLAGS_NONE, &md2, img2);
	CHECK(hr);

	// SRV를 생성합니다.
	hr = ::CreateShaderResourceView(_device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	hr = ::CreateShaderResourceView(_device.Get(), img2.GetImages(), img2.GetImageCount(), md2, _shaderResourceView2.GetAddressOf());
	CHECK(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	// 비트 플래그 설정 (Debug 모드, 최적화 건너 뛰기)
	const uint32 complieFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// HLSL(High Level Shader Language) 코드를 지정된 대상에 대한 바이트코드로 컴파일합니다.
	HRESULT hr = ::D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		complieFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);

	CHECK(hr);
}
