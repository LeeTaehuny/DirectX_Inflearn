#pragma once

// 여러 물체들이 공용으로 사용할 정보들을 열거형으로 선언합니다.
// * 대부분의 물체를 렌더링하기 위해 필요한 정보들
struct PipelineInfo
{
	shared_ptr<InputLayout>		inputLayout;
	shared_ptr<VertexShader>	vertexShader;
	shared_ptr<PixelShader>		pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<BlendState>		blendState;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

// Rendering Pipeline을 관리하는 클래스입니다.
class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

	// 공용으로 사용되는 파이프라인을 업데이트하기 위한 함수를 선언합니다.
	// * PipelineInfo를 전달받아 받은 내용대로 업데이트 합니다.
	// * 사용이 될 지 안될 지 모르는 것들은 빼고 공용적으로 사용되는 것들만 업데이트 해줍니다.
	void UpdatePipeline(PipelineInfo info);

	// 공용적이지 않은 옵션들을 설정하기 위한 함수들입니다.
	// * vertexBuffer를 렌더링 파이프라인에 연결하기 위한 함수를 선언합니다.		(IA)
	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	// * indexBuffer를 렌더링 파이프라인에 연결하기 위한 함수를 선언합니다.		(IA)

	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);
	// * constantBuffer를 렌더링 파이프라인에 연결하기 위한 함수를 적용니다.		(VS)
	//   -> 상수 버퍼는 입력 데이터에 따라 달라져야 하므로 템플릿 문법을 적용해줍니다.
	//   -> 상수 버퍼를 여러 개 사용한다고 가정했을 때 슬롯 여러개가 필요하므로 슬롯 번호도 받아줍니다.
	//   -> 상수 버퍼는 VS, PS 단계에서 사용이 가능하므로 구분을 위한 매개변수를 추가합니다.
	//		(Shader 클래스의 ShaderScope 열거형 참조)
	template<typename T>
	void SetConstantBuffer(uint32 slot, uint32 scope, shared_ptr<ConstantBuffer<T>> buffer)
	{
		// ShaderScope는 비트 플래그입니다. &연산자를 통해 비트가 켜져있는지 확인 가능합니다.

		// * VertexShader 용도로 사용할 것이라면?
		if (scope & SS_VertexShader)
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
		
		// * PixelShader 용도로 사용할 것이라면?
		if (scope & SS_PixelShader)
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
	}

	// * 텍스처(ShaderResourceView)를 렌더링 파이프라인에 연결하기 위한 함수를 선언합니다. (PS)
	void SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture);
	// * Sampling 규칙을 렌더링 파이프라인에 연결하기 위한 함수를 선언합니다.
	void SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState);

	// 일반 정점만을 사용한 경우의 물체를 화면에 그려주는 함수를 선언합니다.
	void Draw(uint32 vertexCount, uint32 startVertexLocation);
	// 인덱스 버퍼를 적용한 경우의 물체를 화면에 그려주는 함수를 선언합니다.
	void DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation);

private:
	// 렌더링 파이프라인에 리소스들을 연결시켜주기 위해 DeviceContext를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11DeviceContext> _deviceContext;
};

