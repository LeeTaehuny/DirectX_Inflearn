#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

Component::Component(ComponentType type) : _type(type)
{
}

Component::~Component()
{
}

shared_ptr<GameObject> Component::GetGameObject()
{
	// weak_ptr.lock() -> shared_ptr
	return _gameObject.lock();
}

shared_ptr<Transform> Component::GetTransform()
{
	return _gameObject.lock()->GetTransform();
}
