#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"
#include "Ghost.h"
#include "CookieItem.h"

#include "TextGo.h"

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

	ghost = new Ghost("Ghost");
	ghost->sortLayer = 1;
	AddGo(ghost);

	uiScore = new TextGo("UI Score");
	AddGo(uiScore, Ui);

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

	uiScore->Set(font, std::to_string(score), 50, sf::Color::White);
	uiScore->SetOutline(sf::Color::Black, 3.f);
	uiScore->SetOrigin(Origins::TC);
	uiScore->SetPosition({ FRAMEWORK.GetWindowSize().x / 2.f, 30.f });

	// 각 타일의 그리드 인덱스 확인용
	for (int i = 0; i < tileMap->GetCellCount().x; i++)
	{
		for (int j = 0; j < tileMap->GetCellCount().y; j++)
		{
			TextGo* text = new TextGo("Position");
			text->Set(font, "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
				15, sf::Color::Black);
			//text->SetOutline(sf::Color::Black, 3.f);
			text->SetPosition(tileMap->GetGridPosition(i, j));
			text->SetOrigin(Origins::MC);
			text->sortLayer = 2;
			text->SetActive(false);
			posTexts.push_back(text);
			AddGo(text);
		}
	}

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

	FindGoAll("Ghost", ghostList, Layers::World);

	// 그리드 좌표 확인용
	if (SCENE_MGR.GetDeveloperMode())
	{
		for (auto text : posTexts)
		{
			text->SetActive(true);
		}
	}
	else
	{
		for (auto text : posTexts)
		{
			text->SetActive(false);
		}
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::AddScore(const int score)
{
	this->score += score * scoreScale;
	uiScore->SetString(std::to_string(this->score));
	uiScore->SetOrigin(Origins::TC);
}
