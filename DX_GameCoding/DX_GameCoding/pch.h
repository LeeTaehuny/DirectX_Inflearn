#pragma once

// ���� ����� ������� �̸� �������ϱ� ���� ����Դϴ�.

#include "Types.h"
#include "Struct.h"
#include "Values.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

// WIN
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

// DX �߰� ���� ���͸� ���� �� DirectXTex ��� �ҷ�����
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

using namespace DirectX;
using namespace Microsoft::WRL;

// ���̺귯���� ����ϰڴٰ� �����ϱ�
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// �ٿ���� ���̺귯���� ����ϰڴٰ� �����ϱ�
// * Debug ����� ���
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_Debug.lib")
// * Debug ��尡 �ƴ� ���
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif
