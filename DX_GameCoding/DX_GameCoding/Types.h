#pragma once

// 자료형들을 편리하게 사용할 수 있도록 이름을 재정의하는 헤더입니다.

#include "DirectXMath.h"

using int8	= __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8	 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Vec2  = DirectX::XMFLOAT2;
using Vec3  = DirectX::XMFLOAT3;
using Vec4  = DirectX::XMFLOAT4;

using Color = DirectX::XMFLOAT4;