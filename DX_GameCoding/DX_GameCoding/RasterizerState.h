#pragma once
class RasterizerState
{
public:
	RasterizerState(ComPtr<ID3D11Device> device);
	~RasterizerState();

	// RasterizerState를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11RasterizerState> GetComPtr() { return _rasterizerState; }

	// RasterizerState를 생성하기 위한 함수를 선언합니다.
	void Create();
	
private:
	// RasterizerState를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// 파이프라인의 RS 단계를 구성하는 데 쓰이는 설정들을 묶은 상태 집합을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11RasterizerState> _rasterizerState = nullptr;
};

