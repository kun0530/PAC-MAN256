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
	startTile = new TileMap("Background");
	startTile->LoadFromFile("Tables/Start_Path.csv");
	startTile->sortLayer = -1;
	currentTileMap = startTile;
	AddGo(startTile);

	for (int i = 1; i <= tileMapNum; i++)
	{
		TileMap* tileMap = new TileMap();
		tileMap->LoadFromFile("Tables/Path" + std::to_string(i) + ".csv");
		tileMap->sortLayer = -1;
		tileMap->SetActive(false);
		AddGo(tileMap);
		tileMaps.push_back(tileMap);
	}

	nextTileMap = tileMaps[Utils::RandomRange(0, tileMapNum)];
	nextTileMap->SetActive(true);
	/*if (tempTileMaps.empty() == true)
	{
		for (int i = 0; i < 3; i++)
		{
			tempTileMaps.push_back(tileMaps[i]);
		}
	}
	nextTileMap = tempTileMaps.front();
	tempTileMaps.pop_front();*/

	// Enter
	startTile->SetPosition({ 0.f, 0.f });
	startTile->SetOrigin(Origins::MC);

	sf::Vector2f pos = startTile->GetPosition();
	pos.y -= (startTile->GetGlobalBounds().height + nextTileMap->GetGlobalBounds().height) / 2.f;
	nextTileMap->SetPosition(pos);
	nextTileMap->SetOrigin(Origins::MC);
	

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
	currentTileMapId = 0;

	textChain->Set(font, "", 20.f, sf::Color::White);
	textChain->SetOrigin(Origins::BC);

	// 각 타일의 그리드 인덱스 확인용
	for (int i = 0; i < startTile->GetCellCount().x; i++)
	{
		for (int j = 0; j < startTile->GetCellCount().y; j++)
		{
			TextGo* text = new TextGo("Position");
			text->Set(font, "(" + std::to_string(i) + ", " + std::to_string(j) + ")",
				15, sf::Color::Black);
			// text->SetOutline(sf::Color::Black, 3.f);
			text->SetPosition(startTile->GetGridPosition(i, j));
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

TileMap* SceneGame::ChangeTileMap(bool isGoUp)
{
	if (player->GetCurrentTileMapId() == currentTileMapId)
	{
		if (isGoUp)
		{
			// 이전 맵 초기화
			if (currentTileMapId >= 1)
			{
				// tileMaps.push_back(prevTileMap);
			}
			if (currentTileMapId != 0) // 시작맵 제외
			{
				prevTileMap->SetActive(false);
			}



			prevTileMap = currentTileMap;
			currentTileMap = nextTileMap;
			++currentTileMapId;
			player->SetCurrentTileMapId(currentTileMapId);



			// TO-DO: 다음 타일맵 세팅
			/*if (tempTileMaps.empty() == true)
			{
				std::random_shuffle(tileMaps.begin(), tileMaps.end());
				for (int i = 0; i < 3; i++)
				{
					tempTileMaps.push_back(tileMaps[i]);
				}
			}
			nextTileMap = tempTileMaps.front();
			tempTileMaps.pop_front();*/

			do
			{
				nextTileMap = tileMaps[Utils::RandomRange(0, tileMapNum)];
			} while (nextTileMap == prevTileMap || nextTileMap == currentTileMap);

			sf::Vector2f pos = currentTileMap->GetPosition();
			pos.y -= (currentTileMap->GetGlobalBounds().height + nextTileMap->GetGlobalBounds().height) / 2.f;
			nextTileMap->SetPosition(pos);
			nextTileMap->SetOrigin(Origins::MC);
			nextTileMap->SetActive(true);
			nextTileMap->Reset();

			return currentTileMap;
		}
		else
		{
			player->SetCurrentTileMapId(currentTileMapId - 1);

			return prevTileMap;
		}
	}
	else if (isGoUp)
	{
		player->SetCurrentTileMapId(currentTileMapId);

		return currentTileMap;
	}
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
