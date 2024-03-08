#include "pch.h"
#include "Player.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "Ghost.h"
#include "Item.h"

Player::Player(const std::string& name) : SpriteGo(name)
{
}

void Player::Init()
{
	SpriteGo::Init();

	SetTexture("graphics/Pacman_1.png");
	SetOrigin(Origins::MC);
	// SetPosition({ 0.f, 0.f });
	SetScale({ 0.5f, 0.5f });
}

void Player::Release()
{
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();

	tileMap = dynamic_cast<TileMap*>(SCENE_MGR.GetCurrentScene()->FindGo("Background"));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	gridIndex = { 13, 16 };
	isArrive = true;
	currentPos = tileMap->GetGridPosition(gridIndex.x, gridIndex.y);
	SetPosition(currentPos);

	timer = 0.f;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	sf::Vector2i inputDirection = InputMgr::GetAxisOne();
	if (inputDirection != sf::Vector2i(0, 0))
		inputDirections.push(inputDirection);

	if (isArrive)
	{
		while (!inputDirections.empty())
		{
			sf::Vector2i dir = inputDirections.front();
			inputDirections.pop();

			if (!tileMap->IsBlocked(gridIndex.x + dir.x, gridIndex.y + dir.y))
			{
				direction = (sf::Vector2f)dir;
			}
		}

		if (!tileMap->IsBlocked(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y))
		{
			nextPos = tileMap->GetGridPosition(gridIndex.x + (int)direction.x, gridIndex.y + (int)direction.y);
			moveTime = Utils::Magnitude(nextPos - currentPos) / speed;
			isArrive = false;
			timer = 0.f;
		}
	}
	else
	{
		if (timer > moveTime)
		{
			currentPos = nextPos;
			SetPosition(currentPos);

			gridIndex.x += (int)direction.x;
			gridIndex.y += (int)direction.y;

			if (EatItem())
			{
				sceneGame->AddChain();
			}
			else
			{
				sceneGame->ResetChain();
			}

			isArrive = true;
		}
		else
		{
			timer += dt;
			Translate(direction * speed * dt);
		}
	}

	// 아이템
	if (itemMode != ItemType::NONE)
	{
		itemTimer += dt;
		if (itemTimer > itemDuration)
		{
			SetItemMode(ItemType::NONE);
		}
	}
}

void Player::FixedUpdate(float dt)
{
	SpriteGo::FixedUpdate(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Player::SetItemMode(ItemType mode)
{
	itemMode = mode;

	// TO-DO: 나중에 데이터 테이블로 정리
	switch (itemMode)
	{
	case ItemType::POWER_COOKIE:
		itemDuration = 5.f;
		break;
	default:
		break;
	}

	std::list<GameObject*> goList = sceneGame->GetGhostList();
	for (auto go : goList)
	{
		if (!go->GetActive())
			continue;

		Ghost* ghost = dynamic_cast<Ghost*>(go);
		if (ghost != nullptr)
		{
			ghost->ChangeMode();
		}
	}
	itemTimer = 0.f;
}

bool Player::EatItem()
{
	auto& itemInfo = tileMap->GetItem(gridIndex);

	if (itemInfo.second == nullptr)
		return false;
	
	switch (itemInfo.first)
	{
	case ItemType::NONE:
		return false;
	case ItemType::COOKIE:
		sceneGame->AddScore(1);
		itemInfo.second->SetActive(false);
		tileMap->SetItemType(gridIndex, ItemType::NONE);
		break;
	case ItemType::POWER_COOKIE:
		sceneGame->AddScore(1);
		SetItemMode(ItemType::POWER_COOKIE);
		itemInfo.second->SetActive(false);
		tileMap->SetItemType(gridIndex, ItemType::NONE);
		break;
	}

	return true;
}

void Player::OnDie()
{
	// Game Over!!!
	FRAMEWORK.SetTimeScale(0.f);
	SetActive(false);
}

