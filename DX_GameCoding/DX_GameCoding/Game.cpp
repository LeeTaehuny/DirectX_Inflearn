#include "pch.h"
#include "Game.h"
#include "Graphics.h"

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

	// Graphics 객체를 생성합니다.
	_graphics = make_shared<Graphics>(hwnd);


	CreateGeometry();
	CreateVS();
	CreateInputLayout();
	CreatePS();

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();
	CreateConstantBuffer();
}

void Game::Update()
{
	// 테스트를 위해 로컬 위치를 이동시켜봅니다.
	_localPosition.x += 0.001f;


	// SRT
	{
		// SRT 순서에 맞춰 행렬을 생성합니다.
		// * Scale
		Matrix matScale = Matrix::CreateScale(_localScale);

		// * Rotation 
		Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
		matRotation *= Matrix::CreateRotationY(_localRotation.y);
		matRotation *= Matrix::CreateRotationZ(_localRotation.z);

		// * Translation
		Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

		// 만들어진 SRT 정보를 조합해 월드 행렬을 만들어줍니다. 
		Matrix matWorld = matScale * matRotation * matTranslation;
		// World 행렬에 넣어줍니다.
		_transformData.matWorld = matWorld;
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	ZeroMemory(&subResource, sizeof(subResource));

	// _transformData를 매 프레임마다 _constantBuffer에 복사를 해주겠습니다.
	// * Map으로 데이터를 넣어줄 준비를 합니다.
	_graphics->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	// * 해당 위치에서 _transformData를 GPU에 복사해줍니다.
	::memcpy(subResource.pData, &_transformData, sizeof(_transformData));

	// * 사용이 끝났으므로 Unmap으로 Map을 해제합니다.
	_graphics->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0);
}

void Game::Render()
{
	// 렌더를 위한 준비 작업
	_graphics->RenderBegin();

	// TODO : 렌더
	// IA - VS - RS - PS - OM
	{
		// 임시용
		ComPtr<ID3D11DeviceContext> _deviceContext = _graphics->GetDeviceContext();

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
			// 디바이스 컨텍스트를 이용해 VS에 만든 상수 버퍼를 연결시켜줍니다.
			_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
		}

		// RS
		{
			// 코딩은 할 수 없지만 여러가지 설정은 가능한 단계
			// VS 단계에서 넘겨준 삼각형을 대상으로 삼각형 내부에 있는 모든 픽셀들을 판별하는 영역

			// 디바이스 컨텍스트를 이용해 생성한 RasterizerState(설정값)를 RS에 적용시켜줍니다.
			_deviceContext->RSSetState(_rasterizerState.Get());
		}

		// PS
		{
			// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더를 연결시켜줍니다.
			_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
			// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더 리소스 뷰를 연결시켜줍니다.
			_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
			_deviceContext->PSSetShaderResources(1, 1, _shaderResourceView2.GetAddressOf());

			// 디바이스 컨텍스트를 이용해 생성한 Sampling 규칙을 적용시켜줍니다.
			_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		}

		// OM
		{
			// 디바이스 컨텍스트를 이용해 생성한 Blending 규칙을 적용시켜줍니다.
			_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, -1);
		}

		// 디바이스 컨텍스트를 이용해 해당 물체를 그려달라고 요청합니다.
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
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
		HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
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
		HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
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
	_graphics->GetDevice()->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// 만든 셰이더 파일로부터 _vsBlob에 임시 데이터를 불러와 로드합니다.
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	// VS를 생성합니다.
	HRESULT hr = _graphics->GetDevice()->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreatePS()
{
	// 만든 셰이더 파일로부터 _psBlob에 임시 데이터를 불러와 로드합니다.
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	// PS를 생성합니다.
	HRESULT hr = _graphics->GetDevice()->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
	CHECK(hr);
}

void Game::CreateRasterizerState()
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
	HRESULT hr = _graphics->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	CHECK(hr);
}

void Game::CreateSamplerState()
{
	// SamplerState 생성에 사용될 정보를 생성합니다.
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	
	// 순서대로 R, G, B, A의 설정을 의미
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;

	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	// SamplerState를 생성합니다.
	HRESULT hr = _graphics->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
	CHECK(hr);
}	

void Game::CreateBlendState()
{
	// BlendState 생성에 사용될 정보를 생성합니다.
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	// 블렌딩과 관련된 옵션 (false : 블렌딩 X, true : 블렌딩 O - 반투명 등에 사용)
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// BlendState를 생성합니다.
	HRESULT hr = _graphics->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
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
	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView.GetAddressOf());
	CHECK(hr);

	hr = ::CreateShaderResourceView(_graphics->GetDevice().Get(), img2.GetImages(), img2.GetImageCount(), md2, _shaderResourceView2.GetAddressOf());
	CHECK(hr);
}

void Game::CreateConstantBuffer()
{
	// 버퍼 생성에 사용될 DESC를 생성합니다.
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	// D3D11_USAGE_DYNAMIC : CPU_Write + GPU_Read
	desc.Usage = D3D11_USAGE_DYNAMIC;
	// D3D11_BIND_CONSTANT_BUFFER : 상수 버퍼 용도로 사용하겠다는 설정
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	// D3D11_CPU_ACCESS_WRITE : CPU의 쓰기를 허용한다는 플래그
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 버퍼를 생성해줍니다. (_constantBuffer에 결과물을 저장합니다.)
	HRESULT hr = _graphics->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
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
