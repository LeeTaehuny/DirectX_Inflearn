#pragma once
#include <vector>

//class Graphics;
//class VertexBuffer;

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
	// 윈도우 핸들 번호를 저장하기 위한 변수를 선언합니다.
	HWND	_hwnd = 0;

	// 윈도우 창 크기를 저장하기 위한 변수를 선언합니다.
	// uint32	_width = 0;
	// uint32	_height = 0;

	// Graphics 타입의 Shared_ptr를 선언합니다.
	shared_ptr<Graphics> _graphics;
	// Pipeline 타입의 Shared_ptr를 선언합니다.
	shared_ptr<Pipeline> _pipeline;

private:
	// Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer> _vertexBuffer;
	shared_ptr<IndexBuffer> _indexBuffer;
	shared_ptr<InputLayout> _inputLayout;

	// VS
	shared_ptr<VertexShader> _vertexShader;

	// RS
	shared_ptr<RasterizerState> _rasterizerState;

	// PS
	shared_ptr<PixelShader> _pixelShader;

	// SRV
	shared_ptr<Texture> _texture1;

	// Sampler
	shared_ptr<SamplerState> _samplerState;

	// Blend
	shared_ptr<BlendState> _blendState;

private:
	// 위치, 회전, 크기를 가지는 구조체 타입의 변수를 선언합니다.
	TransformData _transformData;
	
	// constantBuffer
	shared_ptr<ConstantBuffer<TransformData>> _constantBuffer;

	// 로컬 좌표계 기준의 위치와 회전, 스케일을 저장하기 위한 변수를 선언합니다.
	Vec3 _localPosition = { 0.0f, 0.0f, 0.0f };
	Vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
	Vec3 _localScale = { 1.0f, 1.0f, 1.0f };
};

