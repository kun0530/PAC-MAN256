#include "pch.h"
#include "TileMap.h"

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
		{ 50.f, 0 },
		{ 50.f, 50.f },
		{ 0, 50.f }
	};

	for (int i = 0; i < count.y; i++)
	{
		for (int j = 0; j < count.x; j++)
		{
			// 하드 코딩: 좀비 게임에서만 적용 가능
			int texIndex = Utils::RandomRange(0, 3);
			if (i == 0 || i == count.y - 1 || j == 0 || j == count.x - 1) // 외곽
			{
				texIndex = 3;
			}

			int quadIndex = i * count.x + j; // 2차원 인덱스를 1차원 인덱스로 변환
			sf::Vector2f quadPos(size.x * j, size.y * i);

			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = (quadIndex * 4) + k;
				va[vertexIndex].position = quadPos + posOffsets[k];
				va[vertexIndex].texCoords = texCoord0[k];
				va[vertexIndex].texCoords.y += texIndex * 50.f;
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
	SetSpriteSheetId("graphics/background_sheet.png");
	Set({ 10, 10 }, { 50.f, 50.f });

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
