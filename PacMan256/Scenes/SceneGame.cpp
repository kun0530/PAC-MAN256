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
	tileMap->SetPosition({ 0.f, 0.f });
	tileMap->SetOrigin(Origins::MC);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	worldView.setCenter({ 0.f, player->GetPosition().y });
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
