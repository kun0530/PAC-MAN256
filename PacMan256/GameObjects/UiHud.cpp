#include "pch.h"
#include "UiHud.h"
// #include "Player.h"

UiHud::UiHud(const std::string& name) : GameObject(name)
{
}

void UiHud::SetResolution(const sf::Vector2f resolution)
{
}

void UiHud::SetScore(int score)
{
	textScore.SetString(std::to_string(score));
}

void UiHud::SetHighScore(int highScore)
{
	textHiScore.SetString(std::to_string(highScore));
}

//void UiHud::SetChain(int chain)
//{
//	textChain.SetString(std::to_string(chain));
//	textChain.SetPosition(player->GetPosition() + sf::Vector2f(0.f, -30.f));
//	textChain.SetActive(true);
//	textChainTimer = 0.f;
//}

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
	/*textChain.Init();*/
	textMessage.Init();
	textFps.Init();

	sf::Font& font = RES_MGR_FONT.Get("fonts/Arial.ttf");

	float textSize = 60.f;
	textScore.Set(font, "", textSize, sf::Color::White);
	textHiScore.Set(font, "", textSize, sf::Color::White);
	/*textChain.Set(font, "", 20.f, sf::Color::White);
	textChain.SetActive(false);*/
	textMessage.Set(font, "", textSize, sf::Color::White);
	textMessage.SetActive(false);
	textFps.Set(font, "", textSize, sf::Color::White);
	textFps.SetActive(false);

	/*gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpSize);*/

	textScore.SetOrigin(Origins::TL);
	textHiScore.SetOrigin(Origins::TR);
	/*textChain.SetOrigin(Origins::BC);*/
	// Utils::SetOrigin(gaugeHp, Origins::BL);
	textMessage.SetOrigin(Origins::MC);
	textFps.SetOrigin(Origins::TR);

	// Top
	float topY = 25.f;
	textScore.SetPosition({ referenceResolution.x / 2.f, topY });
	textHiScore.SetPosition({ referenceResolution.x - 150.f, topY });
	textFps.SetPosition({ referenceResolution.x - 150.f, topY + 100.f });

	textMessage.SetPosition({ referenceResolution.x / 2.f, referenceResolution.y / 2.f });

	// Bottom
	float BottomY = referenceResolution.y - 100.f;
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	/*player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	textChainTimer = 0.f;*/
}

void UiHud::Update(float dt)
{
	//textChain.SetPosition(player->GetPosition() + sf::Vector2f(0.f, -30.f));
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
	//textChain.Draw(window);
	textMessage.Draw(window);
	// window.draw(gaugeHp);

	if (SCENE_MGR.GetDeveloperMode())
	{
		SetFps(FRAMEWORK.GetFps());
		textFps.Draw(window);
	}
}
