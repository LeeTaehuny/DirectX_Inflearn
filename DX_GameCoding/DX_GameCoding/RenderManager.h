#pragma once
#include "RenderHelper.h"

class Graphics;
class Pipeline;

// 화면에 그리기 위한 물체들을 들고 있는 역할을 하는 매니저 클래스입니다.
// * 조명, 여러 개의 카메라 등..
class RenderManager
{
public:
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
	~RenderManager();

	void Init();
	void Update(shared_ptr<Graphics> graphics);

private:
	// 데이터들을 버퍼로 넘겨주기 위한 함수들을 선언합니다.
	void PushCamaraData();
	void PushTransformData();

	// 렌더링이 가능한 오브젝트들을 가져오기 위한 함수를 선언합니다.
	void GatherRenderableObjects();
	// _renderObject들을 그려주기 위한 함수를 선얺바니다.
	void RenderObject();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

	shared_ptr<Pipeline> _pipeline;

private:
	// 버퍼들은 물체가 1000개여도 1개만 존재하면 됩니다.
	// -> 렌더 매니저에서 관리하도록 하겠습니다.
	
	// Camera
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	// SRT
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;

	// Animation

private:
	// RS
	shared_ptr<RasterizerState> _rasterizerState;
	// Sampler
	shared_ptr<SamplerState> _samplerState;
	// Blend
	shared_ptr<BlendState> _blendState;

	// 게임 오브젝트들을 저장할 벡터 컨테이너를 선언합니다. (그려주기 위한 오브젝트들)
	vector<shared_ptr<GameObject>> _renderObject;
};

