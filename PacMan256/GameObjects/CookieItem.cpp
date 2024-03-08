#include "pch.h"
#include "CookieItem.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Player.h"

CookieItem::CookieItem(const std::string& name) : Item(name)
{
}

void CookieItem::Init()
{
	Item::Init();

	SetTexture("graphics/cookie.png");
	SetOrigin(Origins::MC);
}

void CookieItem::Release()
{
	Item::Release();
}

void CookieItem::Reset()
{
	Item::Reset();
}

void CookieItem::Update(float dt)
{
	Item::Update(dt);
}

void CookieItem::FixedUpdate(float dt)
{
	Item::FixedUpdate(dt);
}

void CookieItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}
