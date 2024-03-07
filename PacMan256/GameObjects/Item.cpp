#include "pch.h"
#include "Item.h"

Item::Item(const std::string& name) : SpriteGo(name)
{
}

void Item::Init()
{
	SpriteGo::Init();
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();
}

void Item::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Item::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Item::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}
