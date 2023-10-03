#pragma once

// 자주 사용할 헤더들을 미리 컴파일하기 위한 헤더입니다.



#include "Types.h"
#include "Struct.h"
#include "Values.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
using namespace std;

// WIN
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>					// ComPtr 를 사용하기 위함
#include <DirectXMath.h>

// DX 추가 포함 디렉터리 설정 후 DirectXTex 헤더 불러오기
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace Microsoft::WRL;		// ComPtr 를 사용하기 위함

// 라이브러리를 사용하겠다고 설정하기
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// 다운받은 라이브러리도 사용하겠다고 설정하기
// * Debug 모드인 경우
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
// * Debug 모드가 아닌 경우
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif

// assertion 체크
#define CHECK(p) assert(SUCCEEDED(p));

// engine
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexData.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"