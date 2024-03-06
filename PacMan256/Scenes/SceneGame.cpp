#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	tileMap = new TileMap("Background");
	tileMap->sortLayer = -1;
	AddGo(tileMap);

	player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	Scene::Enter();

	tileMap->SetPosition({ 0.f, 0.f });
	tileMap->SetOrigin(Origins::MC);

	player->SetPosition(tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y));
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	worldView.setCenter({ 0.f, player->GetPosition().y });

	if (InputMgr::GetKeyDown(sf::Keyboard::Up) &&
		!tileMap->IsBlocked(playerGridIndex.x, playerGridIndex.y - 1))
	{
		playerGridIndex.y--;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Down) &&
		!tileMap->IsBlocked(playerGridIndex.x, playerGridIndex.y + 1))
	{
		playerGridIndex.y++;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Right) &&
		!tileMap->IsBlocked(playerGridIndex.x + 1, playerGridIndex.y))
	{
		playerGridIndex.x++;
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Left) &&
		!tileMap->IsBlocked(playerGridIndex.x - 1, playerGridIndex.y))
	{
		playerGridIndex.x--;
	}
	player->SetPosition(tileMap->GetGridPosition(playerGridIndex.x, playerGridIndex.y));
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
