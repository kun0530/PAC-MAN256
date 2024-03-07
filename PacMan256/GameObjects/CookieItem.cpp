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

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));
}

void CookieItem::Update(float dt)
{
	Item::Update(dt);

	if (gridIndex == player->GetGridIndex())
	{
		sceneGame->AddScore(10);
		SetActive(false);
	}
}

void CookieItem::FixedUpdate(float dt)
{
	Item::FixedUpdate(dt);
}

void CookieItem::Draw(sf::RenderWindow& window)
{
	Item::Draw(window);
}
