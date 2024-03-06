#include "pch.h"
#include "UiHud.h"

UiHud::UiHud(const std::string& name) : GameObject(name)
{
}

void UiHud::SetResolution(const sf::Vector2f resolution)
{
}

void UiHud::SetScore(int s)
{
	textScore.SetString(formatScore + std::to_string(s));
}

void UiHud::SetHiScore(int s)
{
	textHiScore.SetString(formatHighScore + std::to_string(s));
}

void UiHud::SetAmmo(int current,  int total)
{
	textAmmo.SetString(std::to_string(current) + "/" + std::to_string(total));
	// std::string text = formatAmmo.replace(formatAmmo.find("{0}"), 3, std::to_string(current));
}

void UiHud::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	gaugeHp.setSize({ gaugeHpSize.x * value, gaugeHpSize.y });
}

void UiHud::SetWave(int w)
{
	textWave.SetString(formatWave + std::to_string(w));
}

void UiHud::SetZombieCount(int count)
{
	textZombieCount.SetString(formatZombieCount + std::to_string(count));
}

void UiHud::SetMessage(const std::string& msg)
{
	textMessage.SetString(msg);
	textMessage.SetOrigin(Origins::MC);
}

void UiHud::SetMessageActive(bool active)
{
	if (active)
		textMessage.SetActive(true);
	else
		textMessage.SetActive(false);
}

void UiHud::SetFps(int fps)
{
	textFps.SetString(formatFps + std::to_string(fps));
}

void UiHud::Init()
{
	textScore.Init();
	textHiScore.Init();
	imgAmmoIcon.Init();
	textAmmo.Init();
	textWave.Init();
	textZombieCount.Init();
	textMessage.Init();
	textFps.Init();

	sf::Font& font = RES_MGR_FONT.Get("fonts/zombiecontrol.ttf");

	float textSize = 60.f;
	textScore.Set(font, "", textSize, sf::Color::White);
	textHiScore.Set(font, "", textSize, sf::Color::White);
	textAmmo.Set(font, "", textSize, sf::Color::White);
	textWave.Set(font, "", textSize, sf::Color::White);
	textZombieCount.Set(font, "", textSize, sf::Color::White);
	textMessage.Set(font, "", textSize, sf::Color::White);
	textMessage.SetActive(false);
	textFps.Set(font, "", textSize, sf::Color::White);
	textFps.SetActive(false);

	imgAmmoIcon.SetTexture("graphics/ammo_icon.png");
	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpSize);

	textScore.SetOrigin(Origins::TL);
	textHiScore.SetOrigin(Origins::TR);
	imgAmmoIcon.SetOrigin(Origins::BL);
	textAmmo.SetOrigin(Origins::BL);
	Utils::SetOrigin(gaugeHp, Origins::BL);
	textWave.SetOrigin(Origins::BR);
	textZombieCount.SetOrigin(Origins::BR);
	textMessage.SetOrigin(Origins::MC);
	textFps.SetOrigin(Origins::TR);

	// Top
	float topY = 100.f;
	textScore.SetPosition({ 150.f, topY });
	textHiScore.SetPosition({ referenceResolution.x - 150.f, topY });
	textFps.SetPosition({ referenceResolution.x - 150.f, topY + 100.f });

	textMessage.SetPosition({ referenceResolution.x / 2.f, referenceResolution.y / 2.f });

	// Bottom
	float BottomY = referenceResolution.y - 100.f;
	imgAmmoIcon.SetPosition({ 100.f, BottomY });
	textAmmo.SetPosition({ 300.f, BottomY });
	gaugeHp.setPosition({ 600.f, BottomY });

	textWave.SetPosition({ referenceResolution.x - 500.f, BottomY });
	textZombieCount.SetPosition({ referenceResolution.x - 150.f, BottomY });
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
}

void UiHud::Update(float dt)
{
}

void UiHud::LateUpdate(float dt)
{
}

void UiHud::FixedUpdate(float dt)
{
}

void UiHud::Draw(sf::RenderWindow& window)
{
	textScore.Draw(window);
	textHiScore.Draw(window);
	imgAmmoIcon.Draw(window);
	textAmmo.Draw(window);
	textWave.Draw(window);
	textZombieCount.Draw(window);
	textMessage.Draw(window);
	window.draw(gaugeHp);

	if (SCENE_MGR.GetDeveloperMode())
	{
		SetFps(FRAMEWORK.GetFps());
		textFps.Draw(window);
	}
}
