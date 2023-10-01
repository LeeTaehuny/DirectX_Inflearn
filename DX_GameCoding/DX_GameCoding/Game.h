#pragma once
#include <vector>

class Graphics;

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

	// RS 단계를 구성하는 데 쓰이는 설정들을 묶은 상태 집합을 생성하기 위한 함수를 선언합니다.
	void	CreateRasterizerState();
	// SamplerState를 생성하기 위한 함수를 선언합니다.
	void	CreateSamplerState();
	// BlendState를 생성하기 위한 함수를 선언합니다.
	void	CreateBlendState();

	// SRV(셰이더-리소스-뷰)를 생성하기 위한 함수를 선언합니다.
	// * SRV : 셰이더에 리소스로 사용할 수 있는 뷰
	void	CreateSRV();

	// 상수 버퍼를 생성하기 위한 함수를 선언합니다.
	void	CreateConstantBuffer();

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
	// uint32	_width = 0;
	// uint32	_height = 0;

	// Graphics 타입의 Shared_ptr를 선언합니다.
	shared_ptr<Graphics> _graphics;


private:
	// Geometry
	// * 정점들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<Vertex> _vertices;
	// * 정점 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;

	// * 인덱스들을 저장하기 위한 벡터 컨테이너를 선언합니다.
	vector<uint32> _indices;
	// * 인덱스 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	// * 입력 레이아웃을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	// * VS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	// * 임의 길이 데이터를 반환하는 데 사용할 변수를 선언합니다.
	ComPtr<ID3DBlob> _vsBlob;

	// RS
	// * 파이프라인의 RS 단계를 구성하는 데 쓰이는 설정들을 묶은 상태 집합을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;

	// PS
	// * PS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	// * 임의 길이 데이터를 반환하는 데 사용할 변수를 선언합니다.
	ComPtr<ID3DBlob> _psBlob;

	// SRV
	// * 셰이더 리소스 뷰를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView2 = nullptr;

	// Sampler
	// * Sampler를 생성하는데 쓰이는 설정들을 묶은 상태 집합을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;

	// Blend
	// * PS를 거쳐 생성된 단편이 렌더 타겟에 어떻게 적용될지를 결정하는 상태값들을 저장할 변수를 선언합니다.
	ComPtr<ID3D11BlendState> _blendState = nullptr;

private:
	// 위치, 회전, 크기를 가지는 구조체 타입의 변수를 선언합니다.
	TransformData _transformData;
	// * 상수 정보들을 저장하기 위한 버퍼를 선언합니다.
	ComPtr<ID3D11Buffer> _constantBuffer;

	// 로컬 좌표계 기준의 위치와 회전, 스케일을 저장하기 위한 변수를 선언합니다.
	Vec3 _localPosition = { 0.0f, 0.0f, 0.0f };
	Vec3 _localRotation = { 0.0f, 0.0f, 0.0f };
	Vec3 _localScale = { 1.0f, 1.0f, 1.0f };
};

