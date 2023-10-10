#pragma once

// 어떠한 리소스를 사용할 때 이 용도가 어디서 사용될 것인지를 구분하기 위한 열거형을 선언합니다.
enum ShaderScope
{
	SS_None = 0,				// default

	// 비트 플래그 용도로 사용할 것이라는 힌트를 주기 위해 << 연산자 사용
	SS_VertexShader = (1 << 0),	// VertexShader에서 사용 (1)
	SS_PixelShader	= (1 << 1),	// PixelShader에서 사용	(2)
	SS_Both			= SS_VertexShader | SS_PixelShader, // 둘 다 사용 (3)
};

// Shader 공용 클래스
class ShaderBase
{
public:
	ShaderBase(ComPtr<ID3D11Device> device);
	virtual ~ShaderBase();

	// 셰이더를 생성하기 위한 순수 가상함수를 선언합니다.
	virtual void Create(const wstring& path, const string& name, const string& version) = 0;

	// 임의 길이 데이터를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3DBlob> GetBlob() { return _blob; }

protected:
	// 셰이더는 파일을 로드하는 방식으로 만들어줘야 합니다.
	// * 공용으로 셰이더를 파일로부터 로드하기 위한 함수를 선언합니다.
	// * path : 경로
	// * name : 이름
	// * version : 셰이더 버전
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	// Shader 파일의 이름과 경로를 저장하기 위한 변수를 선언합니다.
	wstring _path;
	string _name;

	// Shader를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;
	// 임의 길이 데이터를 반환하는 데 사용할 변수를 선언합니다.
	ComPtr<ID3DBlob> _blob;
};

// VertexShader
class VertexShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	VertexShader(ComPtr<ID3D11Device> device);
	virtual ~VertexShader() override;

	// 셰이더를 생성하기 위한 가상함수를 재정의합니다.
	virtual void Create(const wstring& path, const string& name, const string& version);

	// VertexShader를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

protected:
	// VS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
};


// PixelShader
class PixelShader : public ShaderBase
{
	using Super = ShaderBase;
public:
	PixelShader(ComPtr<ID3D11Device> device);
	virtual ~PixelShader() override;

	// 셰이더를 생성하기 위한 가상함수를 재정의합니다.
	virtual void Create(const wstring& path, const string& name, const string& version);

	// PixelShader를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }

protected:
	// PS를 로드해 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
};