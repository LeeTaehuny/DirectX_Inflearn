#pragma once
#include "Pass.h"
#include "Technique.h"
#include "BindShaderDesc.h"

struct ShaderDesc
{
	ComPtr<ID3DBlob> blob;
	ComPtr<ID3DX11Effect> effect;
};

class Shader
{
public:
	friend struct Pass;

public:
	Shader(wstring file);
	~Shader();

	wstring GetFile() { return _file; }
	ComPtr<ID3DX11Effect> Effect() { return _shaderDesc.effect; }

	void Draw(UINT technique, UINT pass, UINT vertexCount, UINT startVertexLocation = 0);
	void DrawIndexed(UINT technique, UINT pass, UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawInstanced(UINT technique, UINT pass, UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation = 0, UINT startInstanceLocation = 0);
	void DrawIndexedInstanced(UINT technique, UINT pass, UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);

	void Dispatch(UINT technique, UINT pass, UINT x, UINT y, UINT z);

	ComPtr<ID3DX11EffectVariable> GetVariable(string name);
	ComPtr<ID3DX11EffectScalarVariable> GetScalar(string name);
	ComPtr<ID3DX11EffectVectorVariable> GetVector(string name);
	ComPtr<ID3DX11EffectMatrixVariable> GetMatrix(string name);
	ComPtr<ID3DX11EffectStringVariable> GetString(string name);
	ComPtr<ID3DX11EffectShaderResourceVariable> GetSRV(string name);
	ComPtr<ID3DX11EffectRenderTargetViewVariable> GetRTV(string name);
	ComPtr<ID3DX11EffectDepthStencilViewVariable> GetDSV(string name);
	ComPtr<ID3DX11EffectUnorderedAccessViewVariable> GetUAV(string name);
	ComPtr<ID3DX11EffectConstantBuffer> GetConstantBuffer(string name);
	ComPtr<ID3DX11EffectShaderVariable> GetShader(string name);
	ComPtr<ID3DX11EffectBlendVariable> GetBlend(string name);
	ComPtr<ID3DX11EffectDepthStencilVariable> GetDepthStencil(string name);
	ComPtr<ID3DX11EffectRasterizerVariable> GetRasterizer(string name);
	ComPtr<ID3DX11EffectSamplerVariable> GetSampler(string name);

private:
	void CreateEffect();
	ComPtr<ID3D11InputLayout> CreateInputLayout(ComPtr<ID3DBlob> fxBlob, D3DX11_EFFECT_SHADER_DESC* effectVsDesc, vector<D3D11_SIGNATURE_PARAMETER_DESC>& params);

private:
	wstring _file;
	ShaderDesc _shaderDesc;
	D3DX11_EFFECT_DESC _effectDesc;
	shared_ptr<StateBlock> _initialStateBlock;
	vector<Technique> _techniques;

public:
	// 전역 데이터(V, P)를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushGlobalData(const Matrix& view, const Matrix& projection);
	// 위치 데이터(W)를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushTransformData(const TransformDesc& desc);
	// 빛 정보를 셰이더에 Push하기 위한 함수들을 선언합니다.
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);
	// Bone 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushBoneData(const BoneDesc& desc);
	// Keyframe 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushKeyframeData(const KeyframeDesc& desc);
	// Tween 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushTweenData(const InstancedTweedDesc& desc);
	// Snow 정보를 셰이더에 Push하기 위한 함수를 선언합니다.
	void PushSnowData(const SnowBillboardDesc& desc);

private:
	// 전역으로 프레임마다 한 번만 세팅하는 정보들을 저장하기 위한 변수를 선언합니다.
	GlobalDesc _globalDesc;
	// 전역 정보를 GPU에 넘겨주기 위한 상수 버퍼를 선언합니다.
	shared_ptr<ConstantBuffer<GlobalDesc>> _globalBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _globalEffectBuffer;

	// W와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	TransformDesc _transformDesc;
	shared_ptr<ConstantBuffer<TransformDesc>> _transformBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _transformEffectBuffer;

	// Light와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	LightDesc _lightDesc;
	shared_ptr<ConstantBuffer<LightDesc>> _lightBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _lightEffectBuffer;

	MaterialDesc _materialDesc;
	shared_ptr<ConstantBuffer<MaterialDesc>> _materialBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _materialEffectBuffer;

	// Bone와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	BoneDesc _boneDesc;
	shared_ptr<ConstantBuffer<BoneDesc>> _boneBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _boneEffectBuffer;

	// Keyframe과 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	KeyframeDesc _keyframeDesc;
	shared_ptr<ConstantBuffer<KeyframeDesc>> _keyframeBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _keyframeEffectBuffer;

	// Tween 애니메이션 재생과 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	InstancedTweedDesc _tweenDesc;
	shared_ptr<ConstantBuffer<InstancedTweedDesc>> _tweenBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _tweenEffectBuffer;

	// Snow와 관련된 정보 또한 설정 가능하도록 변수를 선언해줍니다.
	SnowBillboardDesc _snowDesc;
	shared_ptr<ConstantBuffer<SnowBillboardDesc>> _snowBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _snowEffectBuffer;

};

class ShaderManager
{
public:
	static ShaderDesc GetEffect(wstring fileName);

private:
	static unordered_map<wstring, ShaderDesc> shaders;
};