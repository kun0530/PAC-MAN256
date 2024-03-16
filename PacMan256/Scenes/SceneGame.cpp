#include "pch.h"
#include "SceneGame.h"

#include "TileMap.h"
#include "KillScreen.h"
#include "Player.h"

#include "Ghost.h"
#include "GhostBlinky.h"
#include "GhostPinky.h"
#include "GhostInky.h"
#include "GhostSpunky.h"
#include "GhostClyde.h"
#include "GhostGlitchy.h"

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
	currentTileMap->SetMapStatus(MapStatus::CURRENT);
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
	nextTileMap->SetMapStatus(MapStatus::NEXT);
	nextTileMap->SetActive(true);

	// Enter
	startTile->SetPosition({ 0.f, 0.f });
	startTile->SetOrigin(Origins::MC);

	sf::Vector2f pos = startTile->GetPosition();
	pos.y -= (startTile->GetGlobalBounds().height + nextTileMap->GetGlobalBounds().height) / 2.f;
	nextTileMap->SetPosition(pos);
	nextTileMap->SetOrigin(Origins::MC);



	// 킬 스크린
	killScreen = new KillScreen("Kill Screen");
	killScreen->sortLayer = 3;
	AddGo(killScreen);

	
	// 플레이어
	player = new Player("Player");
	player->sortLayer = 1;
	AddGo(player);
	


	// 고스트 테스트
	//Ghost* ghostBlinky = new GhostBlinky("Ghost");
	//ghostBlinky->sortLayer = 1;
	//AddGo(ghostBlinky);

	//Ghost* ghostPinky = new GhostPinky("Ghost");
	//ghostPinky->sortLayer = 1;
	//AddGo(ghostPinky);

	//Ghost* ghostInky = new GhostInky("Ghost");
	//ghostInky->sortLayer = 1;
	//AddGo(ghostInky);

	//Ghost* ghostSpunky = new GhostSpunky("Ghost");
	//ghostSpunky->sortLayer = 1;
	//AddGo(ghostSpunky);

	//Ghost* ghostClyde = new GhostClyde("Ghost");
	//ghostClyde->sortLayer = 1;
	//AddGo(ghostClyde);

	//Ghost* ghostGlitchy = new GhostGlitchy("Ghost");
	//ghostGlitchy->sortLayer = 1;
	//AddGo(ghostGlitchy);


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

	// 프루트
	if (scoreMultiplier != 1)
	{
		uiHud->SetMultiplierActive(true);
		uiHud->SetMultiplier(scoreMultiplier);
		if (fruitTimer > fruitRealDuration)
		{
			uiHud->SetMultiplierActive(false);
			fruitTimer = 0.f;
			fruitRealDuration = fruitDuration;
			scoreMultiplier = 1;
		}
		else
			fruitTimer += dt;
	}

	// 킬 스크린
	int killScreenMapId = killScreen->GetTileMapId();
	int killScreenIndexY = killScreen->GetGridIndexY();
	for (auto obj : ghostList)
	{
		Ghost* ghost = dynamic_cast<Ghost*>(obj);
		if (ghost->GetTileMapId() > killScreenMapId || ghost->GetGridIndex().y <= killScreenIndexY)
			continue;

		RemoveGo(ghost);
	}
	if (!(player->GetCurrentTileMapId() > killScreenMapId || player->GetGridIndex().y <= killScreenIndexY))
		player->OnDie();
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

int SceneGame::CountPathNum(int x, int y, TileMap* map, std::vector<sf::Vector2f>& directions) const
{
	directions.clear();

	if (x == 0 || x == map->GetCellCount().x - 1)
	{
		directions.push_back(sf::Vector2f(1.f, 0.f));
		directions.push_back(sf::Vector2f(-1.f, 0.f));
		return 2;
	}
	if (y == 0)
	{
		directions.push_back(sf::Vector2f(0.f, 1.f));
		directions.push_back(sf::Vector2f(0.f, -1.f));
		return 2;
	}

	int count = 0;
	if (y == map->GetCellCount().y - 1)
	{
		if (!map->IsBlocked(x - 1, y))
		{
			directions.push_back(sf::Vector2f(-1.f, 0.f));
			++count;
		}
		if (!map->IsBlocked(x + 1, y))
		{
			directions.push_back(sf::Vector2f(1.f, 0.f));
			++count;
		}
		if (!map->IsBlocked(x, y - 1))
		{
			directions.push_back(sf::Vector2f(0.f, -1.f));
			++count;
		}

		switch (map->GetMapStatus())
		{
		case MapStatus::NONE:
		case MapStatus::PREV:
			return false;
		case MapStatus::CURRENT:
			if (!prevTileMap->IsBlocked(x, 0))
			{
				directions.push_back(sf::Vector2f(0.f, 1.f));
				++count;
			}
			break;
		case MapStatus::NEXT:
			if (!currentTileMap->IsBlocked(x, 0))
			{
				directions.push_back(sf::Vector2f(0.f, 1.f));
				++count;
			}
			break;
		}

		return count;
	}

	return map->IsFork(x, y, directions);
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
				prevTileMap->SetMapStatus(MapStatus::NONE);
			}


			currentTileMap->SetMapStatus(MapStatus::PREV);
			nextTileMap->SetMapStatus(MapStatus::CURRENT);

			TileMap* tempTileMap = prevTileMap;
			prevTileMap = currentTileMap;
			currentTileMap = nextTileMap;
			++currentTileMapId;
			player->SetCurrentTileMapId(currentTileMapId);

			do
			{
				nextTileMap = tileMaps[Utils::RandomRange(0, tileMapNum)];
			} while (nextTileMap == prevTileMap || nextTileMap == currentTileMap || nextTileMap == tempTileMap);

			sf::Vector2f pos = currentTileMap->GetPosition();
			pos.y -= (currentTileMap->GetGlobalBounds().height + nextTileMap->GetGlobalBounds().height) / 2.f;
			nextTileMap->SetPosition(pos);
			nextTileMap->SetOrigin(Origins::MC);
			nextTileMap->SetMapStatus(MapStatus::NEXT);
			nextTileMap->SetActive(true);
			nextTileMap->Reset();

			CreateGhost(5);

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

void SceneGame::SetScoreMultiplier(int multiplier)
{
	if (scoreMultiplier == 1)
		fruitRealDuration = fruitDuration;
	else
		fruitRealDuration += fruitAddDuration;

	if (scoreMultiplier < multiplier)
		scoreMultiplier = multiplier;
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

void SceneGame::CreateGhost(int num)
{
	for (int i = 0; i < num; ++i)
	{
		Ghost* ghost = nullptr;
		int randNum = Utils::RandomRange(0, 6);
		switch (randNum)
		{
		case 0:
			ghost = new GhostBlinky("Ghost");
			break;
		case 1:
			ghost = new GhostPinky("Ghost");
			break;
		case 2:
			ghost = new GhostInky("Ghost");
			break;
		case 3:
			ghost = new GhostInky("Ghost");
			break;
		case 4:
			ghost = new GhostClyde("Ghost");
			break;
		case 5:
			ghost = new GhostGlitchy("Ghost");
			break;
		default:
			break;
		}
		if (ghost != nullptr)
		{
			ghost->sortLayer = 1;
			ghost->Init();
			ghost->Reset();
			AddGo(ghost);
		}
	}
}
