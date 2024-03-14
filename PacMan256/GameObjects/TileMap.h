#pragma once
#include "GameObject.h"

class Item;

enum class MapStatus
{
	NONE,
	PREV,
	CURRENT,
	NEXT
};

class TileMap : public GameObject
{

	struct Tile
	{
		int x = 0;
		int y = 0;
		int type = 0;
		Item* item = nullptr;
		// ItemType itemType = ItemType::NONE;
	};

protected:
	std::string id;
	MapStatus mapStatus = MapStatus::NONE;

	sf::VertexArray va;
	std::string spriteSheetId;
	sf::Texture* texture;

	sf::Vector2i cellCount; // 셀의 가로줄, 세로줄 개수
	sf::Vector2f cellSize; // 단위 셀의 가로, 세로 길이

	sf::Transform transform;

	std::vector<int> paths;
	std::vector<Tile*> tiles;
public:

	TileMap(const std::string& name = "");

	virtual sf::FloatRect GetLocalBounds() override;
	virtual sf::FloatRect GetGlobalBounds() override;

	const sf::Vector2i& GetCellCount() const { return cellCount; }
	const sf::Vector2f& GetCellSize() const { return cellSize; }

	const MapStatus GetMapStatus() const { return mapStatus; }
	void SetMapStatus(const MapStatus status) { mapStatus = status; }

	const sf::Vector2f& GetGridPosition(int x, int y) const;
	Item* GetItem(sf::Vector2i index) const;
	// void SetItemType(const sf::Vector2i index, const ItemType type);

	bool IsBlocked(int x, int y) const;
	int IsFork(int x, int y, std::vector<sf::Vector2f>& dirList) const;
	const int CountOpenedCell(sf::Vector2i gridIndex) const;
	const int GetTileId(sf::Vector2i gridIndex, int count) const;

	void LoadFromFile(const std::string& filePath);
	void Set(const sf::Vector2i& count, const sf::Vector2f& size);
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

	void SetActive(bool active) override;

	void Init() override;
	void Release() override;

	void Reset() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

