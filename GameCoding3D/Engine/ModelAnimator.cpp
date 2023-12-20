#include "pch.h"
#include "ModelAnimator.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Material.h"
#include "ModelAnimation.h"
#include "Camera.h"
#include "Light.h"

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader)
	: Super(ComponentType::Animator), _shader(shader)
{
	// TEST
	_tweenDesc.next.animIndex = rand() % 3;
	_tweenDesc.tweenSumTime += rand() % 100;
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::Update() // Tween
{
	
}

void ModelAnimator::UpdateTweenData()
{
	// 셰이더에 Push하기 위한 정보 변수를 선언합니다.
	TweenDesc& desc = _tweenDesc;

	// Anim Update - test (Tween)
	
	// 데이터를 추가합니다.
	// * 누적 시간
	desc.curr.sumTime += DT;

	// * 현재 애니메이션
	{
		shared_ptr<ModelAnimation> currentAnim = _model->GetAnimationByIndex(desc.curr.animIndex);

		if (currentAnim)
		{
			// 시간의 흐름에 따라 프레임을 변경합니다.
			float timePerFrame = 1 / (currentAnim->frameRate * desc.curr.speed);
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션이 예약 되어 있다면?
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += DT;
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			// 애니메이션 교체 성공
			desc.curr = desc.next;
			desc.ClearNextAnim();
		}
		else
		{
			// 애니메이션 교체 중
			shared_ptr<ModelAnimation> nextAnim = _model->GetAnimationByIndex(desc.next.animIndex);
			desc.next.sumTime += DT;

			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);

			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;

				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
			}

			desc.next.ratio = desc.next.sumTime / timePerFrame;
		}
	}


	
}

void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		// 셰이더 연결
		material->SetShader(_shader);
	}
}

void ModelAnimator::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_model == nullptr)
		return;

	// Shader에 전달하기 위한 텍스처가 없다면 생성합니다.
	if (_texture == nullptr)
	{
		CreateTexture();
	}

	// GlobalData
	_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightobj = SCENE->GetCurrentScene()->GetLight();

	if (lightobj)
		_shader->PushLightData(lightobj->GetLight()->GetLightDesc());

	// 셰이더에 srv 전달
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());

	// Bone들의 정보를 설정합니다.
	// * 저장하기 위한 변수 선언
	BoneDesc boneDesc;
	// * 저장할 Bone의 숫자 받아오기
	const uint32 boneCount = _model->GetBoneCount();
	// * 숫자만큼 순회하며 Bone 정보 삽입
	for (uint32 i = 0; i < boneCount; i++)
	{
		// 해당 인덱스에 맞는 Bone를 찾아옵니다.
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		// Bone 정보에 해당 bone의 Transform 정보를 저장합니다.
		boneDesc.transforms[i] = bone->transform;
	}

	// 저장한 Bone들의 정보를 셰이더에 Push합니다.
	_shader->PushBoneData(boneDesc);

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// 해당 메쉬의 BoneIndex를 셰이더에 전달합니다.
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		buffer->PushData();

		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}

InstanceID ModelAnimator::GetInstanceID()
{
	// InstanceID는 model와 shader의 주소값(uint64)으로 설정했습니다.
	// * 형식에 맞게 반환해줍니다.
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}

void ModelAnimator::CreateTexture()
{
	// 애니메이션이 존재하지 않는다면 반환합니다.
	if (_model->GetAnimationCount() == 0) return;

	// 애니메이션의 수만큼 벡터 컨테이너의 크기를 조정합니다.
	_animTransforms.resize(_model->GetAnimationCount());

	// 애니메이션의 수만큼 반복합니다.
	for (uint32 i = 0; i <_model->GetAnimationCount(); i++)
	{
		// 애니메이션 Transform 정보를 생성합니다.
		CreateAnimationTransform(i);
	}

	// Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		// SRT 행렬은 64byte이지만 텍스처 최대 크기 16바이트 -> 4개로 만들어주기 위해 * 4
		desc.Width = MAX_MODEL_TRANSFORMS * 4;		  
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = _model->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16byte 크기
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		// 데이터를 넣어주는 과정
		// -> 아주 큰 버퍼를 만들고 데이터를 채우기
		// -> 텍스처를 처음 만들 때 해당 버퍼를 건네주기
		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		// 최종 사이즈 만큼 메모리를 할당하여 들고 있게 만들기
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());

		// 파편화된 데이터를 조립하기
		// * 애니메이션의 숫자만큼 순회하며 해당 애니메이션의 프레임 정보를 저장합니다.
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}

		// 리소스 생성 (texture)
		vector<D3D11_SUBRESOURCE_DATA> subResources(_model->GetAnimationCount());

		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE->CreateTexture2D(&desc, subResources.data(), _texture.GetAddressOf());
		CHECK(hr);

		::free(mallocPtr);
	}

	// Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = _model->GetAnimationCount();

		HRESULT hr = DEVICE->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf());
		CHECK(hr);
	}
}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	// cache
	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	// 전달받은 인덱스를 토대로 작업할 모델의 애니메이션을 받아옵니다.
	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	/** 특정 애니메이션의 특정 bone의 특정 frame에 관련된 SRT 변환 행렬 구하기 */
	 
	// 해당 애니메이션의 프레임 수만큼 반복합니다.
	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		// 모델의 bone 수만큼 반복합니다.
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			// 인덱스에 해당하는 bone을 받아옵니다.
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);

			Matrix matAnimation;

			// 해당 bone 이름으로 키 프레임을 받아옵니다.
			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);
			// 만약 키 프레임이 존재한다면?
			if (frame != nullptr)
			{
				// 해당하는 프레임의 키 프레임 데이터를 가져옵니다.
				ModelKeyframeData& data = frame->transforms[f];
				
				// 키 프레임 데이터에서 SRT 행렬을 구해줍니다.
				Matrix S, R, T;

				S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				// 해당 SRT 행렬을 저장합니다.
				// * 아직 Global이 아닌 자신의 직속 부모의 Local로 넘어가는 변환 행렬
				matAnimation = S * R * T;
			}
			// 존재하지 않는다면?
			else
			{
				// 행렬을 항등 행렬로 만들어줍니다.
				matAnimation = Matrix::Identity;
			}
			
			// T포즈 상태의 로컬->월드로 가기 위한 변환 행렬을 받아옵니다.
			Matrix toRootMatrix = bone->transform;
			// T포즈 상태의 월드->로컬로 가기 위한 변환 행렬을 정의합니다.
			Matrix invGlobal = toRootMatrix.Invert();

			// 해당 bone의 Parent 수를 받아옵니다.
			int32 parentIndex = bone->parentIndex;

			// 애니메이션 상태의 변환 행렬 구하기
			Matrix matParent = Matrix::Identity;

			// 만약 부모가 존재한다면?
			if (parentIndex >= 0)
			{
				// 부모 인덱스에 해당하는 Transforms 정보를 저장합니다.
				matParent = tempAnimBoneTransforms[parentIndex];
			}

			// 해당 bone에 현재 애니메이션의 SRT와 부모 Transform 행렬(월드 변환 행렬)을 곱해 저장합니다.
			// -> 즉 0번 bone부터 시작하기 때문에 루트부터 내려가며 모든 월드 변환 행렬을 정확히 구할 수 있습니다.
			tempAnimBoneTransforms[b] = matAnimation * matParent;


			// 결론
			// * 원본 T포즈 형태의 정점들을 애니메이션에서 이동한 정점들로 변경하고 싶습니다.
			//   1. 원본 T포즈 상태의 bone Transform 정보들을 가지고 있으므로 일단 local로 변경합니다. (invGlobal)
			//   2. 해당 local로 변경한 bone Transfrom 정보들을 각 프레임에서 이동시키고 싶은 만큼 SRT를 적용해주고,
			//      다시 Global로 이동시켜줍니다. (invGlobal) * (matAnimation * matParent)
			//   3. 2에서 구한 행렬을 해당 애니메이션 Transform정보에 저장합니다.

			// * 즉, 이제부터는 어떠한 bone이 있으면 _animTransforms[index].transforms[f][b]에 저장된 행렬을 곱해서
			//   bone을 해당 위치로 이동시킬 수 있다는 의미입니다. (애니메이션)
			_animTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransforms[b];
		}
	}
}
