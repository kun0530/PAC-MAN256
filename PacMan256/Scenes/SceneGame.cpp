#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "Player.h"
#include "Ghost.h"

#include "UiHud.h"
#include "TextGo.h"

SceneGame::SceneGame(SceneIds id) : Scene(id)
{
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	tileMap1 = new TileMap("Background");
	tileMap1->LoadFromFile("Tables/Test_Path.csv");
	tileMap1->sortLayer = -1;
	AddGo(tileMap1);

	tileMap2 = new TileMap("Background2");
	tileMap2->LoadFromFile("Tables/Start_Path.csv");
	tileMap2->sortLayer = -1;
	AddGo(tileMap2);

	// Enter
	tileMap1->SetPosition({ 0.f, 0.f });
	tileMap1->SetOrigin(Origins::MC);

	sf::Vector2f pos = tileMap1->GetPosition();
	pos.y -= (tileMap1->GetGlobalBounds().height + tileMap2->GetGlobalBounds().height) / 2.f;
	tileMap2->SetPosition(pos);
	tileMap2->SetOrigin(Origins::MC);
	//


	player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);

	ghost = new Ghost("Ghost");
	ghost->sortLayer = 1;
	AddGo(ghost);

	uiHud = new UiHud("UI HUD");
	AddGo(uiHud, Ui);

	textChain = new TextGo("UI Chain");
	textChain->sortLayer = 2;
	AddGo(textChain);

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	score = 0;
	chain = 0;

	textChain->Set(font, "", 20.f, sf::Color::White);
	textChain->SetOrigin(Origins::BC);

	// 각 타일의 그리드 인덱스 확인용
	for (int i = 0; i < tileMap2->GetCellCount().x; i++)
	{
		for (int j = 0; j < tileMap2->GetCellCount().y; j++)
		{
			TextGo* text = new TextGo("Position");
			text->Set(font, "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
				15, sf::Color::Black);
			// text->SetOutline(sf::Color::Black, 3.f);
			text->SetPosition(tileMap2->GetGridPosition(i, j));
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

	textChain->SetPosition(player->GetPosition() + sf::Vector2f(0.f, -30.f));

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
	this->score += score * scoreMultiplier;
	uiHud->SetScore(this->score);
}

void SceneGame::AddChain()
{
	chain += 1;
	textChain->SetString(std::to_string(chain));
}

void SceneGame::ResetChain()
{
	chain = 0;
	textChain->SetString(std::to_string(chain));
}

void SceneGame::ChangeGhostMode()
{
	for (auto go : ghostList)
	{
		if (!go->GetActive())
			continue;

		Ghost* ghost = dynamic_cast<Ghost*>(go);
		if (ghost != nullptr)
		{
			ghost->ChangeMode();
		}
	}
}
