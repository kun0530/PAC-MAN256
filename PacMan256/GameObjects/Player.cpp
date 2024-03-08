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

	// ¾ÆÀÌÅÛ
	if (usingItem != ItemType::NONE)
	{
		itemTimer += dt;
		if (itemTimer > itemDuration)
		{
			EndUsingItem();
			SetUsingItem(ItemType::NONE);
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
		break;
	case ItemType::POWER_COOKIE:
		sceneGame->AddScore(1);
		SetUsingItem(ItemType::POWER_COOKIE);
		break;
	case ItemType::FRUIT:
		if (sceneGame->GetScoreMultiplier() < itemInfo.second->GetValue())
			sceneGame->SetScoreMultiplier(itemInfo.second->GetValue());
		SetUsingItem(ItemType::FRUIT);
		break;
	}

	itemInfo.second->SetActive(false);
	tileMap->SetItemType(gridIndex, ItemType::NONE);

	return true;
}

void Player::SetUsingItem(ItemType item)
{
	usingItem = item;

	switch (usingItem)
	{
	case ItemType::POWER_COOKIE:
		itemDuration = 5.f;
		break;
	case ItemType::FRUIT:
		itemDuration = 10.f;
		break;
	default:
		break;
	}

	sceneGame->ChangeGhostMode();
	itemTimer = 0.f;
}

void Player::EndUsingItem()
{
	switch (usingItem)
	{
	case ItemType::NONE:
		break;
	case ItemType::COOKIE:
		break;
	case ItemType::POWER_COOKIE:
		break;
	case ItemType::FRUIT:
		sceneGame->SetScoreMultiplier(1);
		break;
	}

	sceneGame->ChangeGhostMode();
}

void Player::OnDie()
{
	// Game Over!!!
	FRAMEWORK.SetTimeScale(0.f);
	SetActive(false);
}