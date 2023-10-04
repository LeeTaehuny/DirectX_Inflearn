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

	// RasterizerState 객체를 생성합니다.
	_rasterizerState = make_shared<RasterizerState>(_graphics->GetDevice());
	// SamplerState 객체를 생성합니다.
	_samplerState = make_shared<SamplerState>(_graphics->GetDevice());
	// BlendState 객체를 생성합니다.
	_blendState = make_shared<BlendState>(_graphics->GetDevice());

	// Pipeline 객체를 생성합니다.
	_pipeline = make_shared<Pipeline>(_graphics->GetDeviceContext());

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

	_rasterizerState->Create();
	_samplerState->Create();
	_blendState->Create();

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
		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;

		_pipeline->UpdatePipeline(info);

		// 임시용
		ComPtr<ID3D11DeviceContext> _deviceContext = _graphics->GetDeviceContext();

		// IA
		{
			_pipeline->SetVertexBuffer(_vertexBuffer);
			_pipeline->SetIndexBuffer(_indexBuffer);
		}

		// VS
		{
			_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		}

		// RS
		{
		}

		// PS
		{
			_pipeline->SetTexture(0, SS_PixelShader, _texture1);
			_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		}

		// OM
		{
		}

		_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	// 최종 렌더 정보를 제출
	_graphics->RenderEnd();
}