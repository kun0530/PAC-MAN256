#pragma once
#include "Item.h"

class CookieItem : public Item
{
protected:

public:
	CookieItem(const std::string& name = "");
	~CookieItem() override = default;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};