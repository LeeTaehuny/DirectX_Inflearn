#pragma once
class SamplerState
{
public:
	SamplerState(ComPtr<ID3D11Device> device);
	~SamplerState();

	// SamplerState를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11SamplerState> GetComPtr() { return _samplerState; }

	// SamplerState를 생성하기 위한 함수를 선언합니다.
	void Create();

private:
	// SamplerState를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// Sampler를 생성하는데 쓰이는 설정들을 묶은 상태 집합을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11SamplerState> _samplerState = nullptr;
};

