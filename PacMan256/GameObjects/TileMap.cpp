#include "pch.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "Item.h"
#include "rapidcsv.h"

TileMap::TileMap(const std::string& name) : GameObject(name)
{
}

sf::FloatRect TileMap::GetLocalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	bounds.left = origin.x;
	bounds.top = origin.y;
	return bounds;
}

sf::FloatRect TileMap::GetGlobalBounds()
{
	sf::FloatRect bounds = va.getBounds();
	return transform.transformRect(bounds);
}

const sf::Vector2f& TileMap::GetGridPosition(int x, int y) const
{
	if (y < 0)
		return transform.transformPoint(va[x * 4].position) + sf::Vector2f(cellSize.x / 2.f, -cellSize.y / 2.f);

	return transform.transformPoint(va[(y * cellCount.x + x) * 4].position) + cellSize / 2.f;
}

Item* TileMap::GetItem(sf::Vector2i index) const
{
	// 범위 밖의 grid index
	if (index.x < 0 || index.x > cellCount.x || index.y < 0 || index.y > cellCount.y)
	{
		return nullptr;
	}

	return tiles[index.y * cellCount.x + index.x]->item;
}

//void TileMap::SetItemType(const sf::Vector2i index, const ItemType type)
//{
//	tiles[index.y * cellCount.x + index.x]->itemType = type;
//}

bool TileMap::IsBlocked(int x, int y) const
{
	if (y < 0)
	{
		if (x <= 1 || x >= cellCount.x - 2)
			return true;
		else
			return false;
	}

	if (y >= cellCount.y)
		return true;

	if (x < 0 || x >= cellCount.x)
	{
		if (!IsBlocked(0, y) && !IsBlocked(cellCount.x - 1, y))
			return false;
		else
			return true;
	}

	return paths[y * cellCount.x + x] == 0;
}

//bool TileMap::IsCorner(int x, int y) const
//{
//	// 1. 유효한 좌표인지 확인
//	// 2. 통로 셀인지 확인
//
//	if (!IsBlocked(x + 1, y) && !IsBlocked(x - 1, y))
//		return false;
//	if (!IsBlocked(x, y + 1) && !IsBlocked(x, y - 1))
//		return false;
//
//	return true;
//}

int TileMap::IsFork(int x, int y, std::vector<sf::Vector2f>& directions) const
{
	int pathCount = 0;

	if (!IsBlocked(x + 1, y))
	{
		pathCount += 1;
		directions.push_back({ 1.f, 0.f });
	}
	if (!IsBlocked(x - 1, y))
	{
		pathCount += 1;
		directions.push_back({ -1.f, 0.f });
	}
	if (!IsBlocked(x, y + 1))
	{
		pathCount += 1;
		directions.push_back({ 0.f, 1.f });
	}
	if (!IsBlocked(x, y - 1))
	{
		pathCount += 1;
		directions.push_back({ 0.f, -1.f });
	}

	return pathCount;
}

const int TileMap::CountOpenedCell(sf::Vector2i gridIndex) const
{
	int count = 0;
	for (int i = - 1; i <= 1; i++)
	{
		for (int j = - 1; j <= 1; j++)
		{
			if (!IsBlocked(gridIndex.x + j, gridIndex.y + i))
			{
				count += 1;
			}
		}
	}

	return count;
}

const int TileMap::GetTileId(sf::Vector2i gridIndex, int count) const
{
	// TO-DO: 시간 남으면 최적화
	int indexId = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			switch (count)
			{
			case 1:
				if (i != 0 && j != 0 && !IsBlocked(gridIndex.x + j, gridIndex.y + i))
				{
					indexId = (j + 2 * i + 5) / 2;
					return indexId;
				}
				break;
			case 2:
			case 3:
				if (j == 0 && !IsBlocked(gridIndex.x + j, gridIndex.y + i))
				{
					if (i == -1)
					{
						indexId = 5;
					}
					else
					{
						indexId = 6;
					}
					return indexId;
				}
				else if (i == 0 && !IsBlocked(gridIndex.x + j, gridIndex.y + i))
				{
					if (j == -1)
					{
						indexId = 7;
					}
					else
					{
						indexId = 8;
					}
					return indexId;
				}
				break;
			case 5:
				if (i != 0 && j != 0 && IsBlocked(gridIndex.x + j, gridIndex.y + i))
				{
					indexId = (j + 2 * i + 5) / 2 + 8;
					return indexId;
				}
				break;
			default:
				break;
			}
		}
	}
	return indexId;
}

void TileMap::Set(const sf::Vector2i& count, const sf::Vector2f& size)
{
	cellCount = count;
	cellSize = size;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count.x * count.y * 4);

	sf::Vector2f posOffsets[4] = {
		{ 0, 0 },
		{ size.x, 0 },
		{ size.x, size.y },
		{ 0, size.y }
	};

	sf::Vector2f texCoord0[4] = {
		{ 0, 0 },
		{ 32.f, 0 },
		{ 32.f, 32.f },
		{ 0, 32.f }
	};

	for (int i = 0; i < count.y; i++)
	{
		for (int j = 0; j < count.x; j++)
		{
			int quadIndex = i * count.x + j;
			int texIndexX = paths[quadIndex];
			int texIndexY = 0;
			if (texIndexX == 0)
			{
				texIndexY = GetTileId(sf::Vector2i(j, i), CountOpenedCell(sf::Vector2i(j, i)));
			}
			//&& i > 0 && i < count.y - 1 && j > 0 && j < count.x - 1

			sf::Vector2f quadPos(size.x * j, size.y * i);

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = (quadIndex * 4) + k;
				va[vertexIndex].position = quadPos + posOffsets[k];
				va[vertexIndex].texCoords = texCoord0[k];
				va[vertexIndex].texCoords.x += texIndexX * 32.f;
				va[vertexIndex].texCoords.y += texIndexY * 32.f;
			}
		}
	}
}

void TileMap::LoadFromFile(const std::string& filePath)
{
	rapidcsv::Document doc(filePath);
	id = filePath;

	int row = doc.GetRowCount();
	int col = doc.GetColumnCount();

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			auto cell = doc.GetCell<int>(j, i);
			paths.push_back(cell);
		}
	}

	SetSpriteSheetId("graphics/Background_Sheet4.png");
	Set({ col, row }, { 50.f, 50.f });
}

void TileMap::SetSpriteSheetId(const std::string& id)
{
	spriteSheetId = id;
	texture = &RES_MGR_TEXTURE.Get(spriteSheetId);
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity;

	float scaleX = isFlipX ? -scale.x : scale.x;
	float scaleY = isFlipY ? -scale.y : scale.y;
	transform.scale(scaleX, scaleY, position.x, position.y);
	transform.rotate(rotation, position.x, position.y);
	transform.translate(position - origin);
}

void TileMap::SetOrigin(Origins preset)
{
	if (preset == Origins::Custom)
		return;

	originPreset = preset;
	sf::Vector2f newOrigin(va.getBounds().width, va.getBounds().height);
	newOrigin.x *= ((int)originPreset % 3) * 0.5f;
	newOrigin.y *= ((int)originPreset / 3) * 0.5f;
	
	origin = newOrigin;

	UpdateTransform();
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;

	origin = newOrigin;

	UpdateTransform();
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	UpdateTransform();
}

void TileMap::Translate(const sf::Vector2f& delta)
{
	GameObject::Translate(delta);
	UpdateTransform();
}

void TileMap::SetRotation(const float r)
{
	GameObject::SetRotation(r);
	UpdateTransform();
}

void TileMap::SetScale(const sf::Vector2f& scale)
{
	GameObject::SetScale(scale);
	UpdateTransform();
}

void TileMap::SetFlipX(bool flip)
{
	GameObject::SetFlipX(flip);
	UpdateTransform();
}

void TileMap::SetFlipY(bool flip)
{
	GameObject::SetFlipY(flip);
	UpdateTransform();
}

void TileMap::SetActive(bool active)
{
	GameObject::SetActive(active);

	if (!active)
	{
		for (auto tile : tiles)
		{
			if (tile->item != nullptr)
			{
				tile->item->SetActive(false);
			}
		}
	}
	else
	{
		ItemReset();
	}
}

void TileMap::Init()
{
	GameObject::Init();

	tiles.clear();
	for (int i = 0; i < cellCount.y; ++i)
	{
		for (int j = 0; j < cellCount.x; j++)
		{
			Tile* tile = new Tile;
			tile->y = i;
			tile->x = j;
			tile->type = paths[i * cellCount.x + j];
			if (tile->type == 1)
			{
				Item* cookie = new Item;
				cookie->SetItemType(ItemType::NONE);
				cookie->SetTexture("graphics/cookie.png");
				cookie->SetGridIndex(j, i);
				cookie->SetTileMap(this);
				cookie->Init();
				cookie->Reset();
				// SCENE_MGR.GetCurrentScene()->AddGo(cookie);
				tile->item = cookie;
				// cookie->SetActive(false);
				// tile->itemType = ItemType::NONE;
			}
			tiles.push_back(tile);
		}
	}
}

void TileMap::Release()
{
	GameObject::Release();
}

void TileMap::Reset()
{
	GameObject::Reset();

	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	for (auto tile : tiles)
	{
		if (tile->item != nullptr)
		{
			sceneGame->AddGo(tile->item);
		}
	}

	ItemReset();
}

void TileMap::ItemReset()
{
	if (!active)
	{
		for (auto tile : tiles)
		{
			if (tile->item == nullptr)
				continue;

			tile->item->SetActive(false);
		}
		return;
	}

	for (auto tile : tiles)
	{
		if (IsBlocked(tile->x, tile->y))
			continue;

		if (tile->item == nullptr)
			continue;

		int randNum = Utils::RandomRange(0, 100);
		if (randNum < 1)
		{
			tile->item->SetTexture("graphics/power_cookie.png", true);
			tile->item->SetItemType(ItemType::POWER_COOKIE);
			tile->item->Reset();
			// tile->itemType = ItemType::POWER_COOKIE;
		}
		else if (randNum < 2)
		{
			tile->item->SetItemType(ItemType::FRUIT);
			int fruitNum = Utils::RandomRange(2, 7);
			switch (fruitNum)
			{
			case 2:
				tile->item->SetTexture("graphics/Fruit_Cherry.png", true);
				break;
			case 3:
				tile->item->SetTexture("graphics/Fruit_Apple.png", true);
				break;
			case 4:
				tile->item->SetTexture("graphics/Fruit_Strawberry.png", true);
				break;
			case 5:
				tile->item->SetTexture("graphics/Fruit_Orange.png", true);
				break;
			case 6:
				tile->item->SetTexture("graphics/Fruit_Melon.png", true);
				break;
			}
			tile->item->SetValue(fruitNum);
			tile->item->Reset();
			// tile->itemType = ItemType::FRUIT;
		}
		else
		{
			tile->item->SetTexture("graphics/cookie.png", true);
			tile->item->SetItemType(ItemType::COOKIE);
			// tile->itemType = ItemType::COOKIE;
		}

		tile->item->SetOrigin(Origins::MC);
		tile->item->SetActive(true);
		tile->item->SetPosition(GetGridPosition(tile->x, tile->y));
	}
}

void TileMap::Update(float dt)
{
	GameObject::Update(dt);
}

void TileMap::Draw(sf::RenderWindow& window)
{
	sf::RenderStates state;
	state.texture = texture;
	state.transform = transform;

	window.draw(va, state);
}
