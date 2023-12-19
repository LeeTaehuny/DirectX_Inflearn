#pragma once

// Texture를 셰이더와 주고받기 위한 버퍼 클래스입니다.
// * Texture는 기본적으로 2D 이미지입니다.
// * 픽셀 단위로 정보들을 여러 스레드가 처리할 수 있도록 실습해보겠습니다.
class TextureBuffer
{
public:
	// InputData로 생성자에서 텍스쳐를 받아 저장합니다.
	TextureBuffer(ComPtr<ID3D11Texture2D> src);
	~TextureBuffer();

public:
	// 버퍼들(+ View)을 생성하기 위한 함수
	void CreateBuffer();

private:
	void CreateInput(ComPtr<ID3D11Texture2D> src);
	void CreateSRV();
	void CreateOutput();
	void CreateUAV();
	void CreateResult();

private:
	// 입력을 위한 리소스와 반환을 위한 리소스를 하나씩 만들어줍니다.
	ComPtr<ID3D11Texture2D> _input;
	ComPtr<ID3D11Texture2D> _output;

	// 리소스(Buffer)를 바로 이용하는 것이 아닌 리소스를 묘사하는 View를 만들어 이용합니다.
	// * 셰이더에 전달하기 위한 리소스 뷰
	ComPtr<ID3D11ShaderResourceView> _srv;
	// * 셰이더에서 받아오기 위한 엑세스 뷰
	ComPtr<ID3D11UnorderedAccessView> _uav;

// Getter
public:
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return _srv; }
	ComPtr<ID3D11UnorderedAccessView> GetUAV() { return _uav; }

	uint32 GetWidth() { return _width; }
	uint32 GetHeight() { return _height; }
	uint32 GetArraySize() { return _arraySize; }

	ComPtr<ID3D11Texture2D> GetOutput() { return (ID3D11Texture2D*)_output.Get(); }
	ComPtr<ID3D11ShaderResourceView> GetOutputSRV() { return _outputSRV; }


private:
	// 이미지와 관련된 정보를 저장하기 위한 변수들을 선언합니다.
	// * 가로 크기
	uint32 _width = 0;
	// * 세로 크기
	uint32 _height = 0;
	uint32 _arraySize = 0;
	DXGI_FORMAT _format;
	// * 최종적인 결과 이미지 (RawBuffer의 _result 역할)
	ComPtr<ID3D11ShaderResourceView> _outputSRV;
};

