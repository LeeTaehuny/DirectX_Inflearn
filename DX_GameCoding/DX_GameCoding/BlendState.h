#pragma once

class BlendState
{
public:
	BlendState(ComPtr<ID3D11Device> device);
	~BlendState();

	// BlendState를 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11BlendState> GetComPtr() { return _blendState; }

	// 섞는 비율을 반환하기 위한 함수를 정의합니다.
	const float* GetBlendFactor() { return &_blendFactor; }
	// 초기화할 색상을 반환하기 위한 함수를 정의합니다.
	uint32 GetSampleMask() { return _sampleMask; }

	// BlendState를 생성하기 위한 함수를 선언합니다.
	void Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc =
		{
			true,
			D3D11_BLEND_SRC_ALPHA,
			D3D11_BLEND_INV_SRC_ALPHA,
			D3D11_BLEND_OP_ADD,
			D3D11_BLEND_ONE,
			D3D11_BLEND_ZERO,
			D3D11_BLEND_OP_ADD,
			D3D11_COLOR_WRITE_ENABLE_ALL
		}, float factor = 0.0f);


private:
	// BlendState를 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// PS를 거쳐 생성된 단편이 렌더 타겟에 어떻게 적용될지를 결정하는 상태값들을 저장할 변수를 선언합니다.
	ComPtr<ID3D11BlendState> _blendState = nullptr;

	// 섞는 비율을 저장하기 위한 변수를 선언합니다.
	float _blendFactor = 0.0f;
	// 초기화 할 색상을 저장하기 위한 변수를 선언합니다.
	uint32 _sampleMask = 0xFFFFFFFF;
};

