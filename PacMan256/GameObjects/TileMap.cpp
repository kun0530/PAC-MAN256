#include "pch.h"
#include "TileMap.h"
#include "Item.h"

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
	return va[(y * cellCount.x + x) * 4].position - origin + sf::Vector2f(25.f, 25.f);
}

const std::pair<ItemType, Item*> TileMap::GetItem(sf::Vector2i index) const
{
	std::pair<ItemType, Item*> itemInfo = { ItemType::NONE, nullptr };

	// 범위 밖의 grid index
	if (index.x < 0 || index.x > cellCount.x || index.y < 0 || index.y > cellCount.y)
	{
		return itemInfo;
	}

	auto tile = startMap[index.y * cellCount.x + index.x];

	itemInfo.first = tile->itemType;
	if (itemInfo.first == ItemType::COOKIE || itemInfo.first == ItemType::NONE)
	{
		itemInfo.second = tile->cookie;
	}
	else
	{
		itemInfo.second = tile->specialItem;
	}

	return itemInfo;
}

void TileMap::SetItemType(const sf::Vector2i index, const ItemType type)
{
	startMap[index.y * cellCount.x + index.x]->itemType = type;
}

bool TileMap::IsCorner(int x, int y) const
{
	// 1. 유효한 좌표인지 확인
	// 2. 통로 셀인지 확인

	if (!IsBlocked(x + 1, y) && !IsBlocked(x - 1, y))
		return false;
	if (!IsBlocked(x, y + 1) && !IsBlocked(x, y - 1))
		return false;

	return true;
}

bool TileMap::IsFork(int x, int y, std::vector<sf::Vector2f>& directions) const
{
	// 1. 유효한 좌표인지 확인
	// 2. 통로 셀인지 확인
	
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

	if (pathCount < 3)
		return false;
	return true;
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

void TileMap::Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<int>& tiles)
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
			int quadIndex = i * count.x + j; // 2차원 인덱스를 1차원 인덱스로 변환
			int texIndexX = tiles[quadIndex];
			int texIndexY = 0;
			if (texIndexX == 0 && i > 0 && i < count.y - 1 && j > 0 && j < count.x - 1)
			{
				texIndexY = GetTileId(sf::Vector2i(j, i), CountOpenedCell(sf::Vector2i(j, i)));
			}

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

void TileMap::SetSpriteSheetId(const std::string& id)
{
	spriteSheetId = id;
	texture = &RES_MGR_TEXTURE.Get(spriteSheetId);
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity; // 기본 변환

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
	
	/*for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position -= origin;
	}*/
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;

	origin = newOrigin;

	UpdateTransform();

	/*for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position -= origin;
	}*/
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	GameObject::SetPosition(pos);
	UpdateTransform();

	/*sf::Vector2f delta = pos - position;
	for (int i = 0; i < va.getVertexCount(); i++)
	{
		va[i].position += delta;
	}
	position = pos;*/
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

void TileMap::Init()
{
	GameObject::Init();

	// 테스트 시작 맵
	startPath = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0,  0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	// 실제 적용할 시작 맵
	/*startPath = {
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
		0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 
		0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};*/

	SetSpriteSheetId("graphics/Background_Sheet.png");
	Set({ 30, 30 }, { 50.f, 50.f }, startPath);

	startMap.clear();
	for (int i = 0; i < cellCount.y; ++i)
	{
		for (int j = 0; j < cellCount.x; j++)
		{
			Tile* tile = new Tile;
			tile->y = i;
			tile->x = j;
			tile->type = startPath[i * cellCount.x + j];
			if (tile->type == 1)
			{
				Item* cookie = new Item;
				cookie->SetItemType(ItemType::COOKIE);
				cookie->SetTexture("graphics/cookie.png");
				cookie->SetGridIndex(j, i);
				cookie->Init();
				cookie->Reset();
				SCENE_MGR.GetCurrentScene()->AddGo(cookie);
				tile->cookie = cookie;
				tile->itemType = ItemType::COOKIE;

				int randNum = Utils::RandomRange(0, 100);
				if (randNum < 1)
				{
					Item* powerCookie = new Item;
					powerCookie->SetItemType(ItemType::POWER_COOKIE);
					powerCookie->SetTexture("graphics/power_cookie.png");
					powerCookie->SetGridIndex(j, i);
					powerCookie->Init();
					powerCookie->Reset();
					SCENE_MGR.GetCurrentScene()->AddGo(powerCookie);
					tile->cookie->SetActive(false);
					tile->specialItem = powerCookie;
					tile->itemType = ItemType::POWER_COOKIE;
				}
				else if (randNum < 3)
				{
					Item* fruit = new Item;
					fruit->SetItemType(ItemType::FRUIT);
					int fruitNum = Utils::RandomRange(2, 7);
					switch (fruitNum)
					{
					case 2:
						fruit->SetTexture("graphics/Fruit_Cherry.png");
						break;
					case 3:
						fruit->SetTexture("graphics/Fruit_Apple.png");
						break;
					case 4:
						fruit->SetTexture("graphics/Fruit_Strawberry.png");
						break;
					case 5:
						fruit->SetTexture("graphics/Fruit_Orange.png");
						break;
					case 6:
						fruit->SetTexture("graphics/Fruit_Melon.png");
						break;
					}
					fruit->SetValue(fruitNum);
					fruit->SetGridIndex(j, i);
					fruit->Init();
					fruit->Reset();
					SCENE_MGR.GetCurrentScene()->AddGo(fruit);
					tile->cookie->SetActive(false);
					tile->specialItem = fruit;
					tile->itemType = ItemType::FRUIT;
				}
			}

			startMap.push_back(tile);
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
