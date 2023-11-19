#pragma once
// 하나의 실행 단위로 생각합니다.
#include "IExecute.h"
#include "Geometry.h"

class MeshDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	/** Object */
	// 오브젝트를 저장하기 위한 변수를 선언합니다.
	shared_ptr<GameObject> _obj;

	/** Camera */
	// 카메라 컴포넌트를 소유할 게임 오브젝트를 저장하기 위한 변수를 선언합니다.
	shared_ptr<GameObject> _camera;
};