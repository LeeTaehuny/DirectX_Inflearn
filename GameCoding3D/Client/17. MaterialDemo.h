#pragma once
// 하나의 실행 단위로 생각합니다.
#include "IExecute.h"
#include "Geometry.h"

class MaterialDemo : public IExecute
{
public:
	void Init() override;
	void Update() override;
	void Render() override;

	/** Shader */
	// 셰이더를 저장하기 위한 변수를 선언합니다.
	shared_ptr<Shader> _shader;

	/** Object */
	// 오브젝트1를 저장하기 위한 변수를 선언합니다.
	shared_ptr<GameObject> _obj;
	// 오브젝트2를 저장하기 위한 변수를 선언합니다.
	shared_ptr<GameObject> _obj2;

	/** Camera */
	// 카메라 컴포넌트를 소유할 게임 오브젝트를 저장하기 위한 변수를 선언합니다.
	shared_ptr<GameObject> _camera;
};