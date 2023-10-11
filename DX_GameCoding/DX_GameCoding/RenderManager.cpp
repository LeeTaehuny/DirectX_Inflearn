#include "pch.h"
#include "RenderManager.h"
#include "Pipeline.h"
#include "MeshRenderer.h"
#include "Graphics.h"
#include "Camera.h"
#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Animator.h"

RenderManager::RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
	: _device(device), _deviceContext(deviceContext)
{
}

RenderManager::~RenderManager()
{
}

void RenderManager::Init()
{
	// 파이프라인을 생성합니다.
	_pipeline = make_shared <Pipeline>(_deviceContext);

	// TransformData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_transformBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
	// ConstantBuffer 생성
	_transformBuffer->Create();

	// CameraData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_cameraBuffer = make_shared<ConstantBuffer<CameraData>>(_device, _deviceContext);
	// ConstantBuffer 생성
	_cameraBuffer->Create();

	// AnimationData 타입의 정보를 사용하는 ConstantBuffer 객체를 생성합니다.
	_animationBuffer = make_shared<ConstantBuffer<AnimationData>>(_device, _deviceContext);
	// ConstantBuffer 생성
	_animationBuffer->Create();

	// RasterizerState 객체를 생성합니다.
	_rasterizerState = make_shared<RasterizerState>(_device);
	// RasterizerState 생성
	_rasterizerState->Create();
	// BlendState 객체를 생성합니다.
	_blendState = make_shared<BlendState>(_device);
	// BlendState 생성
	_blendState->Create();
	// SamplerState 객체를 생성합니다.
	_samplerState = make_shared<SamplerState>(_device);
	// SamplerState 생성
	_samplerState->Create();
}

void RenderManager::Update(shared_ptr<Graphics> graphics)
{
	// Render 시작
	graphics->RenderBegin();

	{
		// 카메라 데이터 복사
		PushCamaraData();
		// 씬에 존재하는 물체들 중 렌더링이 필요한 물체들을 가져옵니다.
		GatherRenderableObjects();
		// 가져온 물체들을 렌더링 해줍니다.
		RenderObject();
	}

	// Render 종료
	graphics->RenderEnd();
}

void RenderManager::PushCamaraData()
{
	// 카메라 데이터를 적용시키기
	_cameraData.matView = Camera::S_MatView;
	_cameraData.matProjection = Camera::S_MatProjection;

	// 카메라 버퍼에 복사하기
	_cameraBuffer->CopyData(_cameraData);
}

void RenderManager::PushTransformData()
{
	// 트랜스폼 버퍼에 복사하기
	_transformBuffer->CopyData(_transformData);
}

void RenderManager::PushAnimationData()
{
	// 애니메이션 버퍼에 복사하기
	_animationBuffer->CopyData(_animationData);
}

void RenderManager::GatherRenderableObjects()
{
	// 기존에 가지고 있던 물체들을 초기화합니다.
	_renderObject.clear();

	// gameObject들을 가져옵니다.
	auto& gameObjects = SCENE->GetActiveScene()->GetGameObjects();
	for (const shared_ptr<GameObject>& gameObject : gameObjects)
	{
		// 메시 렌더러를 받아옵니다.
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();

		// 만약 메시 렌더러가 존재한다면?
		if (meshRenderer)
		{
			// 화면에 그릴 것들을 모아둔 벡터 컨테이너에 저장합니다.
			_renderObject.push_back(gameObject);
		}
	}
}

void RenderManager::RenderObject()
{
	for (const shared_ptr<GameObject>& gameObject : _renderObject)
	{
		// 메시 렌더러를 받아옵니다.
		shared_ptr<MeshRenderer> meshRenderer = gameObject->GetMeshRenderer();
		if (meshRenderer == nullptr) return;

		// 위치 정보를 가져옵니다.
		shared_ptr<Transform> transform = gameObject->GetTransform();
		if (transform == nullptr) return;

		// SRT
		// * 이미 물체마다 월드 변환 매트릭스를 구했습니다.
		// * transformData에 정보를 채워줍니다.
		_transformData.matWorld = transform->GetWorldMatrix();
		PushTransformData();

		// Animation
		// * 애니메이션이 존재하는지 확인합니다.
		shared_ptr<Animator> animator = gameObject->GetAnimator();
		// * 만약 애니메이션이 존재한다면?
		if (animator)
		{
			// * animationData에 정보를 채워줍니다.
			const Keyframe& keyframe = animator->GetCurrnetKeyframe();
			_animationData.spriteOffset = keyframe.offset;
			_animationData.spriteSize = keyframe.size;
			_animationData.textureSize = animator->GetCurrentAnimation()->GetTextureSize();
			_animationData.useAnimation = 1.0f;

			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, animator->GetCurrentAnimation()->GetTexture());
		}
		// * 만약 애니메이션이 없다면?
		else
		{
			// 데이터를 초기값으로 밀어줍니다.
			_animationData.spriteOffset = Vec2(0.0f, 0.0f);
			_animationData.spriteSize = Vec2(0.0f, 0.0f);
			_animationData.textureSize = Vec2(0.0f, 0.0f);
			_animationData.useAnimation = 0.0f;

			PushAnimationData();

			_pipeline->SetConstantBuffer(2, SS_VertexShader, _animationBuffer);
			_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		}


		PipelineInfo info;
		info.inputLayout = meshRenderer->GetInputLayout();
		info.vertexShader = meshRenderer->GetVertexShader();
		info.pixelShader = meshRenderer->GetPixelShader();
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;

		_pipeline->UpdatePipeline(info);

		_pipeline->SetVertexBuffer(meshRenderer->GetMesh()->GetVertexBuffer());
		_pipeline->SetIndexBuffer(meshRenderer->GetMesh()->GetIndexBuffer());

		_pipeline->SetConstantBuffer(0, SS_VertexShader, _cameraBuffer);
		_pipeline->SetConstantBuffer(1, SS_VertexShader, _transformBuffer);

		//_pipeline->SetTexture(0, SS_PixelShader, meshRenderer->GetTexture());
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

		_pipeline->DrawIndexed(meshRenderer->GetMesh()->GetIndexBuffer()->GetCount(), 0, 0);
	}
}
