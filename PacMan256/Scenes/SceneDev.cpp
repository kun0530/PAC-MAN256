#include "pch.h"
#include "SceneDev.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "TileMap.h"

SceneDev::SceneDev(SceneIds id) : Scene(id)
{
}

SceneDev::~SceneDev()
{
}

void SceneDev::Init()
{
	tileMap = new TileMap("Background");
	tileMap->sortLayer = -1;
	AddGo(tileMap);

	Scene::Init();
}

void SceneDev::Release()
{
	Scene::Release();
}

void SceneDev::Enter()
{
	Scene::Enter();

	tileMap->SetPosition({ 0.f, 0.f });
	tileMap->SetOrigin(Origins::MC);
}

void SceneDev::Exit()
{
	Scene::Exit();
}

void SceneDev::Update(float dt)
{
	Scene::Update(dt);
}

void SceneDev::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
