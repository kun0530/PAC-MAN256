#include "pch.h"
#include "PowerCookieItem.h"
#include "Player.h"
#include "SceneGame.h"

PowerCookieItem::PowerCookieItem(const std::string& name) : Item(name)
{
}

void PowerCookieItem::Init()
{
	Item::Init();

	SetTexture("graphics/power_cookie.png");
	SetOrigin(Origins::MC);
}

void PowerCookieItem::Release()
{
	Item::Release();
}

void PowerCookieItem::Reset()
{
	Item::Reset();
}

void PowerCookieItem::Update(float dt)
{
	Item::Update(dt);
}

void PowerCookieItem::FixedUpdate(float dt)
{
	Item::FixedUpdate(dt);
}

void PowerCookieItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}
