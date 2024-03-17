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

#include "DeathEffectGo.h"

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

	// 킬 스크린
	killScreen = new KillScreen("Kill Screen");
	killScreen->sortLayer = 5;
	AddGo(killScreen);

	
	// 플레이어
	player = new Player("Player");
	player->sortLayer = 4;
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
	isGameOver = false;

	std::ifstream file("Tables/High_Score.txt");
	std::string line;
	if (file.is_open())
	{
		if (getline(file, line))
		{
			highScore = std::stoi(line);
		}
	}
	else
	{
		std::cout << "The \"High_Score.txt\" file cannot be opened." << std::endl;
	}
	file.close();

	textChain->Set(font, "", 20.f, sf::Color::White);
	textChain->SetOrigin(Origins::BC);

	currentTileMap = startTile;
	startTile->SetPosition({ 0.f, 0.f });
	startTile->SetOrigin(Origins::MC);
	startTile->SetActive(true);

	std::string tileMapTexId = "graphics/Background_Sheet" + std::to_string(SCENE_MGR.GetThemeNum()) + ".png";
	startTile->SetSpriteSheetId(tileMapTexId);
	for (auto tileMap : tileMaps)
	{
		if (tileMap != nullptr)
		{
			tileMap->SetSpriteSheetId(tileMapTexId);
			tileMap->SetActive(false);
		}
	}

	prevTileMap = nullptr;

	nextTileMap = tileMaps[Utils::RandomRange(0, tileMapNum)];
	nextTileMap->SetMapStatus(MapStatus::NEXT);
	nextTileMap->SetActive(true);

	sf::Vector2f pos = startTile->GetPosition();
	pos.y -= (startTile->GetGlobalBounds().height + nextTileMap->GetGlobalBounds().height) / 2.f;
	nextTileMap->SetPosition(pos);
	nextTileMap->SetOrigin(Origins::MC);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	if (isGameOver && InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		SOUND_MGR.StopAll();
		SCENE_MGR.ChangeScene(SceneIds::SCENE_TITLE);
	}

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
	{
		if (player->IsAlive())
		{
			SOUND_MGR.PlaySfx("sounds/PM_DEATH_GLITCH.wav");
			player->OnDie();
		}
	}
	if (killScreen->GetPosition().y - player->GetPosition().y <= 500.f)
	{
		if (!isGlitchPlaying)
			SOUND_MGR.PlayBgm("sounds/GEN_GLITCH_LOOP.wav", true);
		isGlitchPlaying = true;
	}
	else
	{
		SOUND_MGR.StopBgm();
		isGlitchPlaying = false;
	}

	if (isGhostKill)
		ZoomCamera(dt);
	else
		worldView.setCenter({ 0.f, player->GetPosition().y });
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
			nextTileMap->ItemReset();

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
	if (chain >= 256)
	{
		CompleteChain256();
	}
}

void SceneGame::ResetChain()
{
	if (chain != 0)
		SOUND_MGR.PlaySfx("sounds/GEN_BREAK_PACDOT_CHAIN.wav");
	chain = 0;
	textChain->SetString("");
}

void SceneGame::CompleteChain256()
{
	SOUND_MGR.PlaySfx("sounds/GEN_SCORE_256_PACDOT.wav");
	for (auto go : ghostList)
	{
		float distance = Utils::Distance(player->GetPosition(), go->GetPosition());
		if (distance <= 1000.f)
		{
			Ghost* ghost = dynamic_cast<Ghost*>(go);
			ghost->OnDie(false);
		}
	}
	ResetChain();
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

void SceneGame::ZoomInOutCamera()
{
	if (isGhostKill)
	{
		isZoomIn = true;
		isZoomOut = false;
		cameraZoomInTimer = 0.f;
		cameraZoomOutTimer = 0.f;
	}
	else
	{
		isGhostKill = true;
		isZoomIn = true;
		isZoomOut = false;
	}
}

void SceneGame::MakeDeatEffect(sf::Vector2f pos, sf::Color color)
{
	DeathEffectGo* deathEffect = new DeathEffectGo("Death Effect");
	deathEffect->SetEffectColor(color);
	deathEffect->SetInitPosition(pos);
	deathEffect->Init();
	deathEffect->Reset();
	deathEffect->sortLayer = 3;
	AddGo(deathEffect);
}

void SceneGame::GameOver()
{
	uiHud->SetGameOver(true);
	MakeDeatEffect(player->GetPosition(), sf::Color::Yellow);
	isGameOver = true;
}

void SceneGame::ZoomCamera(float dt)
{
	sf::Vector2f windowSize = (sf::Vector2f)FRAMEWORK.GetWindowSize();
	sf::Vector2f worldViewSize = worldView.getSize();
	sf::Vector2f worldViewCenter = worldView.getCenter();

	float cameraClampX = 750.f - worldViewSize.x / 2.f;
	if (isZoomIn)
	{
		if (cameraZoomInTimer > cameraZoomInDuration)
		{
			isZoomIn = false;
			isZoomOut = true;
			cameraZoomInTimer = 0.f;

			worldView.setSize(windowSize * zoomInSize);
			worldViewCenter.x = Utils::Clamp(worldViewCenter.x, -cameraClampX, cameraClampX);
			worldView.setCenter(player->GetPosition());
		}
		else
		{
			cameraZoomInTimer += dt;
			worldViewSize = Utils::Lerp(worldViewSize, windowSize * zoomInSize, cameraZoomInTimer / cameraZoomInDuration);
			worldView.setSize(worldViewSize);

			worldViewCenter = Utils::Lerp(worldViewCenter, player->GetPosition(), cameraZoomInTimer / cameraZoomInDuration);
			worldViewCenter.x = Utils::Clamp(worldViewCenter.x, -cameraClampX, cameraClampX);
			worldView.setCenter(worldViewCenter);
		}
	}
	else if (isZoomOut)
	{
		if (cameraZoomOutTimer > cameraZoomOutDuration)
		{
			isZoomOut = false;
			isGhostKill = false;
			cameraZoomOutTimer = 0.f;

			worldView.setSize(windowSize * zoomOutSize);
			worldView.setCenter({ 0.f, player->GetPosition().y });
		}
		else
		{
			cameraZoomOutTimer += dt;
			worldViewSize = Utils::Lerp(worldViewSize, windowSize * zoomOutSize, cameraZoomOutTimer / cameraZoomOutDuration);
			worldView.setSize(worldViewSize);

			worldViewCenter = Utils::Lerp(worldViewCenter, { 0.f, player->GetPosition().y }, cameraZoomOutTimer / cameraZoomOutDuration);
			worldViewCenter.x = Utils::Clamp(worldViewCenter.x, -cameraClampX, cameraClampX);
			worldView.setCenter(worldViewCenter);
		}
	}
}
