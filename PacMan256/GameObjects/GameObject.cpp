#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const std::string& name)
	: name(name)
{
}

GameObject::~GameObject()
{
}

void GameObject::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = { 0.f, 0.f };
}

void GameObject::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void GameObject::Init()
{
}

void GameObject::Release()
{
}

void GameObject::Reset()
{
}

void GameObject::Update(float dt)
{
}

void GameObject::LateUpdate(float dt)
{
}

void GameObject::FixedUpdate(float dt)
{
}

void GameObject::Draw(sf::RenderWindow& window)
{
}
