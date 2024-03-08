#include "pch.h"
#include "Item.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "Player.h"

Item::Item(const std::string& name) : SpriteGo(name)
{
}

void Item::Init()
{
	SpriteGo::Init();

	SetOrigin(Origins::MC);
}

void Item::Release()
{
	SpriteGo::Release();
}

void Item::Reset()
{
	SpriteGo::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	SetPosition(tileMap->GetGridPosition(gridIndex.x, gridIndex.y));
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
