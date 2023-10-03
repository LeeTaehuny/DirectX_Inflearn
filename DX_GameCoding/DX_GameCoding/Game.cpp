#include "pch.h"
#include "Game.h"
//#include "Graphics.h"
//#include "VertexBuffer.h"

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

	// VertexBuffer 객체를 생성합니다.
	_vertexBuffer = make_shared<VertexBuffer>(_graphics->GetDevice());
	// IndexBuffer 객체를 생성합니다.
	_indexBuffer = make_shared<IndexBuffer>(_graphics->GetDevice());
	// InputLayout 객체를 생성합니다.
	_inputLayout = make_shared<InputLayout>(_graphics->GetDevice());

	// VertexTextureData 타입의 정보를 사용하는 Geometry 객체를 생성합니다.
	_geometry = make_shared<Geometry<VertexTextureData>>();

	// VS, PS 객체를 생성합니다.
	_vertexShader = make_shared<VertexShader>(_graphics->GetDevice());
	_pixelShader = make_shared<PixelShader>(_graphics->GetDevice());

	// TransformData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphics->GetDevice(), _graphics->GetDeviceContext());

	// Texture 객체를 생성합니다.
	_texture1 = make_shared<Texture>(_graphics->GetDevice());

	// Vertex Data
	GeometryHelper::CreateRectangle(_geometry);
	// VertexBuffer
	_vertexBuffer->Create(_geometry->GetVertices());
	// indexBuffer
	_indexBuffer->Create(_geometry->GetIndices());

	// VertexShader
	_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
	// InputLayout
	_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());
	// PixelShader
	_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

	CreateRasterizerState();
	CreateSamplerState();
	CreateBlendState();

	_texture1->Create(L"test.png");
	_constantBuffer->Create();
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

	_constantBuffer->CopyData(_transformData);
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
			uint32 stride = sizeof(VertexTextureData);
			// * offset : 보간 수치
			uint32 offset = 0;

			// 디바이스 컨텍스트를 이용해 IA에 정점 버퍼를 연결시켜줍니다.
			_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset);
			// 디바이스 컨텍스트를 이용해 IA에 인덱스 버퍼를 연결시켜줍니다.
			_deviceContext->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
			// 디바이스 컨텍스트를 이용해 IA에 InputLayout 정보를 연결시켜줍니다.
			_deviceContext->IASetInputLayout(_inputLayout->GetComPtr().Get());
			// 삼각형(대부분 모든 사물은 삼각형으로 표현)을 그리는 과정에서 전달한 정점들을 어떤 순서로 이어 붙일 것인지에 대한 정보를 지정합니다. (topology)
			_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		// VS
		{
			// 디바이스 컨텍스트를 이용해 VS에 만든 셰이더를 연결시켜줍니다.
			_deviceContext->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
			// 디바이스 컨텍스트를 이용해 VS에 만든 상수 버퍼를 연결시켜줍니다.
			_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());
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
			_deviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
			// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더 리소스 뷰를 연결시켜줍니다.
			_deviceContext->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());

			// 디바이스 컨텍스트를 이용해 생성한 Sampling 규칙을 적용시켜줍니다.
			_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());
		}

		// OM
		{
			// 디바이스 컨텍스트를 이용해 생성한 Blending 규칙을 적용시켜줍니다.
			_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, -1);
		}

		// 디바이스 컨텍스트를 이용해 해당 물체를 그려달라고 요청합니다.
		_deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
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