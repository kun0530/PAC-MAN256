#pragma once
#include "GameObject.h"

class Item;

class TileMap : public GameObject
{

	struct Tile
	{
		int x = 0;
		int y = 0;
		int type = 0;
		Item* cookie = nullptr;
		Item* specialItem = nullptr;
		ItemType itemType = ItemType::NONE;
	};

protected:
	sf::VertexArray va;
	std::string spriteSheetId;
	sf::Texture* texture;

	sf::Vector2i cellCount; // 셀의 가로줄, 세로줄 개수
	sf::Vector2f cellSize; // 단위 셀의 가로, 세로 길이

	sf::Transform transform;

	std::vector<int> startPath;
	std::vector<Tile*> startMap;
public:

	TileMap(const std::string& name = "");

	virtual sf::FloatRect GetLocalBounds() override;
	virtual sf::FloatRect GetGlobalBounds() override;

	const sf::Vector2i& GetCellCount() const { return cellCount; }
	const sf::Vector2f& GetCellSize() const { return cellSize; }

	const sf::Vector2f& GetGridPosition(int x, int y) const;
	const std::pair<ItemType, Item*>& GetItem(sf::Vector2i index) const;

	bool IsBlocked(int x, int y) const { return startPath[y * cellCount.x + x] == 0; }
	bool IsCorner(int x, int y) const;
	bool IsFork(int x, int y, std::vector<sf::Vector2f>& dirList) const;

	void Set(const sf::Vector2i& count, const sf::Vector2f& size, const std::vector<int>& tiles);
	void SetSpriteSheetId(const std::string& id);
	void UpdateTransform();

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void SetPosition(const sf::Vector2f& pos) override;
	void Translate(const sf::Vector2f& delta) override;
	void SetRotation(const float r) override;

	void SetScale(const sf::Vector2f& scale) override;

	void SetFlipX(bool flip) override;
	void SetFlipY(bool flip) override;

	void Init() override;
	void Release() override;

	void Reset() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

