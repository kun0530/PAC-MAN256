#include "pch.h"
#include "SceneTitle.h"

SceneTitle::SceneTitle(SceneIds id) : Scene(id)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{


	Scene::Init();
}

void SceneTitle::Release()
{
	Scene::Release();
}

void SceneTitle::Enter()
{


	Scene::Enter();
}

void SceneTitle::Exit()
{
	Scene::Exit();
}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		SCENE_MGR.ChangeScene(SceneIds::SCENE_GAME);
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
