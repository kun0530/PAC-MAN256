#include "pch.h"
#include "KillScreen.h"
#include "SceneGame.h"
#include "TileMap.h"
#include "SpriteGo.h"

KillScreen::KillScreen(const std::string& name) : GameObject(name)
{
}

void KillScreen::Init()
{
	GameObject::Init();

	blackScreen.setFillColor(sf::Color::Black);
	blackScreen.setSize((sf::Vector2f)FRAMEWORK.GetWindowSize() * 0.78125f);
	Utils::SetOrigin(blackScreen, Origins::TL);

	for (int i = 0; i <= 3; ++i)
	{
		for (int j = 0; j < 50; ++j)
		{
			SpriteGo* glitchMap = new SpriteGo("Glitch Map");
			glitchMap->SetTexture("graphics/glitch-map-" + std::to_string(i) + ".png");
			glitchMap->SetOrigin(Origins::TL);
			killScreenParts.push_back(glitchMap);
		}
	}
}

void KillScreen::Release()
{
	GameObject::Release();

	for (auto parts : killScreenParts)
	{
		if (parts != nullptr)
		{
			delete parts;
			parts = nullptr;
		}
	}
	killScreenParts.clear();
}

void KillScreen::Reset()
{
	GameObject::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	tileMapId = sceneGame->GetCurrentTileMapId();
	if (tileMapId == 0)
		tileMap = sceneGame->GetCurrentTileMap();
	else
	{
		tileMap = sceneGame->GetPrevTileMap();
		--tileMapId;
	}
	SetOrigin(Origins::TL);
	
	gridIndexY = tileMap->GetCellCount().y - 1;
	nextGridindexY = gridIndexY - 1;

	cellBLPos = { -tileMap->GetCellSize().x / 2.f, tileMap->GetCellSize().y / 2.f };
	currentPosition = tileMap->GetGridPosition(0, gridIndexY) + cellBLPos;
	nextPosition = tileMap->GetGridPosition(0, nextGridindexY) + cellBLPos;
	SetPosition(currentPosition);

	partsUpdateTimer = partsUpdateDuration;
	for (auto parts : killScreenParts)
	{
		parts->SetPosition(sf::Vector2f(0.f, FRAMEWORK.GetWindowSize().y));
	}

	moveTime = Utils::Magnitude(nextPosition - currentPosition) / speed;
	timer = 0.f;
}

void KillScreen::Update(float dt)
{
	GameObject::Update(dt);

	blackScreen.setPosition(position);

	for (auto parts : killScreenParts)
	{
		if (partsUpdateTimer >= partsUpdateDuration)
		{
			float randPosX = Utils::RandomRange(-100.f, blackScreen.getSize().x);
			float randPosY = Utils::RandomRange(-30.f, 100.f);
			parts->SetPosition(position + sf::Vector2f(randPosX, randPosY));
			partsUpdateTimer = 0.f;
		}
		else
		{
			partsUpdateTimer += dt;
			parts->Translate(direction * speed * dt);
		}
	}

	if (sceneGame->GetCurrentTileMapId() - tileMapId >= 2)
		Reset();

	if (timer > moveTime)
	{
		currentPosition = nextPosition;
		SetPosition(currentPosition);

		gridIndexY = nextGridindexY;
		nextGridindexY = gridIndexY - 1;
		if (nextGridindexY < 0)
		{
			tileMap = sceneGame->GetCurrentTileMap();
			tileMapId = sceneGame->GetCurrentTileMapId();
			gridIndexY = tileMap->GetCellCount().y;
			nextGridindexY = gridIndexY - 1;
		}
		nextPosition = tileMap->GetGridPosition(0, nextGridindexY) + cellBLPos;

		moveTime = Utils::Magnitude(nextPosition - currentPosition) / speed;
		timer = 0.f;
	}
	else
	{
		timer += dt;
	}

	Translate(direction * speed * dt);
}

void KillScreen::FixedUpdate(float dt)
{
	GameObject::FixedUpdate(dt);
}

void KillScreen::Draw(sf::RenderWindow& window)
{
	GameObject::Draw(window);

	window.draw(blackScreen);

	for (auto parts : killScreenParts)
	{
		parts->Draw(window);
	}
}
