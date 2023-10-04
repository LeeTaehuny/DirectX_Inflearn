#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> deviceContext) : _deviceContext(deviceContext)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	// IA
	{
		_deviceContext->IASetInputLayout(info.inputLayout->GetComPtr().Get());
		_deviceContext->IASetPrimitiveTopology(info.topology);
	}

	// VS
	if (info.vertexShader)
	{
		_deviceContext->VSSetShader(info.vertexShader->GetComPtr().Get(), nullptr, 0);
	}

	// RS
	if (info.rasterizerState)
	{
		_deviceContext->RSSetState(info.rasterizerState->GetComPtr().Get());
	}

	// PS
	if (info.pixelShader)
	{
		_deviceContext->PSSetShader(info.pixelShader->GetComPtr().Get(), nullptr, 0);
	}

	// OM
	if (info.blendState)
	{
		_deviceContext->OMSetBlendState(info.blendState->GetComPtr().Get(), info.blendState->GetBlendFactor(), info.blendState->GetSampleMask());
	}
}

void Pipeline::SetVertexBuffer(shared_ptr<VertexBuffer> buffer)
{
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();

	// 디바이스 컨텍스트를 이용해 IA에 정점 버퍼를 연결시켜줍니다.
	_deviceContext->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<IndexBuffer> buffer)
{
	// 디바이스 컨텍스트를 이용해 IA에 인덱스 버퍼를 연결시켜줍니다.
	_deviceContext->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture)
{
	// ShaderScope는 비트 플래그입니다. &연산자를 통해 비트가 켜져있는지 확인 가능합니다.

	// * VertexShader 용도로 사용할 것이라면?
	if (scope & SS_VertexShader)
	{
		// 디바이스 컨텍스트를 이용해 VS에 만든 셰이더 리소스 뷰를 연결시켜줍니다.
		_deviceContext->VSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
	}
	// * PixelShader 용도로 사용할 것이라면?
	if (scope & SS_PixelShader)
	{
		// 디바이스 컨텍스트를 이용해 PS에 만든 셰이더 리소스 뷰를 연결시켜줍니다.
		_deviceContext->PSSetShaderResources(slot, 1, texture->GetComPtr().GetAddressOf());
	}
}

void Pipeline::SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState)
{
	// ShaderScope는 비트 플래그입니다. &연산자를 통해 비트가 켜져있는지 확인 가능합니다.

	// * VertexShader 용도로 사용할 것이라면?
	if (scope & SS_VertexShader)
	{
		// 디바이스 컨텍스트를 이용해 생성한 Sampling 규칙을 적용시켜줍니다.
		_deviceContext->VSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
	}
	// * PixelShader 용도로 사용할 것이라면?
	if (scope & SS_PixelShader)
	{
		// 디바이스 컨텍스트를 이용해 생성한 Sampling 규칙을 적용시켜줍니다.
		_deviceContext->PSSetSamplers(slot, 1, samplerState->GetComPtr().GetAddressOf());
	}
}

void Pipeline::Draw(uint32 vertexCount, uint32 startVertexLocation)
{
	_deviceContext->Draw(vertexCount, startVertexLocation);
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
