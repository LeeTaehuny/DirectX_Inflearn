#include "pch.h"
#include "Button.h"
#include "MeshRenderer.h"
#include "Material.h"

Button::Button() : Super(ComponentType::Button)
{
}

Button::~Button()
{
}

bool Button::Picked(POINT screenPos)
{
	return ::PtInRect(&_rect, screenPos);
}

void Button::Create(Vec2 screenPos, Vec2 size, shared_ptr<class Material> material)
{
	// 해당 컴포넌트를 들고 있는 원본 오브젝트를 불러옵니다.
	auto go = _gameObject.lock();

	// 스크린(화면)의 크기를 받아옵니다.
	float height = GRAPHICS->GetViewport().GetHeight();
	float width = GRAPHICS->GetViewport().GetWidth();

	// 현재는 중앙이 원점인 좌표계입니다.
	// * 좌측 상단을 원점으로 우하향 할 수록 커지는 좌표계로 변경해줍니다.
	float x = screenPos.x - width / 2;
	float y = height / 2 - screenPos.y;

	Vec3 position = Vec3(x, y, 0);

	// 버튼 오브젝트를 설정합니다.
	{
		go->GetOrAddTransform()->SetPosition(position);
		go->GetOrAddTransform()->SetScale(Vec3(size.x, size.y, 1));
		go->SetLayerIndex(Layer_UI);

		if (go->GetMeshRenderer() == nullptr)
		{
			go->AddComponent(make_shared<MeshRenderer>());
		}

		go->GetMeshRenderer()->SetMaterial(material);

		auto mesh = RESOURCES->Get<Mesh>(L"Quad");
		go->GetMeshRenderer()->SetMesh(mesh);
		go->GetMeshRenderer()->SetPass(0);
	}

	// Picking (Rect 영역 설정)
	_rect.left = screenPos.x - size.x / 2;
	_rect.right = screenPos.x + size.x / 2;
	_rect.top = screenPos.y - size.x / 2;
	_rect.bottom = screenPos.y + size.x / 2;
	
}

void Button::AddOnClickedEvent(std::function<void(void)> func)
{
	_onClicked = func;
}

void Button::InvokeOnClicked()
{
	if (_onClicked)
	{
		_onClicked();
	}
}
