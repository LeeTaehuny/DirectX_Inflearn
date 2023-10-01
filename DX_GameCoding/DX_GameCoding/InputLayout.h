#pragma once

// VS 단계에서 정점들을 입력값으로 받을 때 사용하기 위한 정보를 생성하는 클래스입니다.
class InputLayout
{
public:
	InputLayout(ComPtr<ID3D11Device> device);
	~InputLayout();

	// InputLayout을 반환하기 위한 함수를 정의합니다.
	ComPtr<ID3D11InputLayout> GetComPtr() { return _inputLayout; }

	// InputLayout을 생성하기 위한 함수를 선언합니다.
	// * InputLayout은 사용되는 Vertex 정보에 따라 다르게 생성되야 합니다.
	// * layout[] 배열에 대한 정보를 벡터 컨테이너 형식으로 받기 위해 매개변수를 추가합니다.
	//   -> vertex의 정보가 몇 개인지 알 수 없기 때문에 배열 자체가 아닌 개수 정보를 알 수 있는 벡터 컨테이너로 전달받습니다.
	// * VertexShader를 컴파일한 결과를 매개변수로 받아줍니다.
	void Create(const vector<D3D11_INPUT_ELEMENT_DESC>& desc, ComPtr<ID3DBlob> blob);

private:
	// InputLayout을 생성하기 위해 디바이스 정보가 필요합니다.
	// * Device를 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11Device> _device;

	// 입력 레이아웃을 저장하기 위한 변수를 선언합니다.
	ComPtr<ID3D11InputLayout> _inputLayout;
};

