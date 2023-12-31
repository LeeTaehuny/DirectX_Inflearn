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
	// ���� ������(V, P)�� ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushGlobalData(const Matrix& view, const Matrix& projection);
	// ��ġ ������(W)�� ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushTransformData(const TransformDesc& desc);
	// �� ������ ���̴��� Push�ϱ� ���� �Լ����� �����մϴ�.
	void PushLightData(const LightDesc& desc);
	void PushMaterialData(const MaterialDesc& desc);
	// Bone ������ ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushBoneData(const BoneDesc& desc);
	// Keyframe ������ ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushKeyframeData(const KeyframeDesc& desc);
	// Tween ������ ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushTweenData(const InstancedTweedDesc& desc);
	// Snow ������ ���̴��� Push�ϱ� ���� �Լ��� �����մϴ�.
	void PushSnowData(const SnowBillboardDesc& desc);

private:
	// �������� �����Ӹ��� �� ���� �����ϴ� �������� �����ϱ� ���� ������ �����մϴ�.
	GlobalDesc _globalDesc;
	// ���� ������ GPU�� �Ѱ��ֱ� ���� ��� ���۸� �����մϴ�.
	shared_ptr<ConstantBuffer<GlobalDesc>> _globalBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _globalEffectBuffer;

	// W�� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
	TransformDesc _transformDesc;
	shared_ptr<ConstantBuffer<TransformDesc>> _transformBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _transformEffectBuffer;

	// Light�� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
	LightDesc _lightDesc;
	shared_ptr<ConstantBuffer<LightDesc>> _lightBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _lightEffectBuffer;

	MaterialDesc _materialDesc;
	shared_ptr<ConstantBuffer<MaterialDesc>> _materialBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _materialEffectBuffer;

	// Bone�� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
	BoneDesc _boneDesc;
	shared_ptr<ConstantBuffer<BoneDesc>> _boneBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _boneEffectBuffer;

	// Keyframe�� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
	KeyframeDesc _keyframeDesc;
	shared_ptr<ConstantBuffer<KeyframeDesc>> _keyframeBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _keyframeEffectBuffer;

	// Tween �ִϸ��̼� ����� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
	InstancedTweedDesc _tweenDesc;
	shared_ptr<ConstantBuffer<InstancedTweedDesc>> _tweenBuffer;
	ComPtr<ID3DX11EffectConstantBuffer> _tweenEffectBuffer;

	// Snow�� ���õ� ���� ���� ���� �����ϵ��� ������ �������ݴϴ�.
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