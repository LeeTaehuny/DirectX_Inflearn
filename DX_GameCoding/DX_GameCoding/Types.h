#pragma once

// 자료형들을 편리하게 사용할 수 있도록 이름을 재정의하는 헤더입니다.
#include <windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"

using int8	= __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8	 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Color = DirectX::XMFLOAT4;

// 마이크로소프트에서 제공하는 SimpleMath 라이브러리 사용
// -> 주의할 점 : 오른손 좌표계로 만들어졌기 때문에 특정 함수들(forward, backword)을 사용할 때 주의
using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;