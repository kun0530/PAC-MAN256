#include "pch.h"
#include "SceneDev.h"

SceneDev::SceneDev(SceneIds id) : Scene(id)
{
}

SceneDev::~SceneDev()
{
}

void SceneDev::Init()
{
	Scene::Init();
}

void SceneDev::Release()
{
	Scene::Release();
}

void SceneDev::Enter()
{
	Scene::Enter();
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
