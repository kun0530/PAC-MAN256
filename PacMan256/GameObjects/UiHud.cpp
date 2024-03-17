#include "pch.h"
#include "UiHud.h"
#include "SceneGame.h"
#include "Player.h"

UiHud::UiHud(const std::string& name) : GameObject(name)
{
}

void UiHud::SetResolution(const sf::Vector2f resolution)
{
}

void UiHud::SetItemTimer(float timer, float duration)
{
	float value;
	if (duration == 0.f)
		value = 0.f;
	else
		value = timer / duration;
	itemTimer.setSize({ itemTimerSize.x * value, itemTimerSize.y });
}

void UiHud::SetScore(int score)
{
	textScore.SetString(std::to_string(score));
}

void UiHud::SetMultiplier(int multiplier)
{
	textMultiplier.SetString(formatMultiplier + std::to_string(multiplier));
}

void UiHud::SetMultiplierActive(bool active)
{
	if (active)
		textMultiplier.SetActive(true);
	else
		textMultiplier.SetActive(false);
}

void UiHud::SetHighScore(int highScore)
{
	textHiScore.SetString(std::to_string(highScore));
}

void UiHud::SetGameOver(const bool isOver)
{
	if (!isGameOver && isOver)
	{
		gameOverPosition = sceneGame->ScreenToUi((sf::Vector2i)(referenceResolution / 2.f));
		int outOfScreenPosX = referenceResolution.x / 2.f + 110.f;

		textGame.SetPosition(gameOverPosition - sf::Vector2f(outOfScreenPosX, textGameOverInterval));
		textOver.SetPosition(gameOverPosition + sf::Vector2f(outOfScreenPosX, textGameOverInterval));

		gameOverTimer = 0.f;
		gameOverMoveTime = Utils::Magnitude(gameOverPosition - textGame.GetPosition()) / gameOverSpeed;
	}

	isGameOver = isOver;
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
	textMultiplier.Init();
	textHiScore.Init();
	textGame.Init();
	textOver.Init();
	/*textChain.Init();*/
	textMessage.Init();
	textFps.Init();

	sf::Font& font = RES_MGR_FONT.Get("fonts/editundo.ttf");

	float textSize = 50.f;
	textScore.Set(font, "", textSize, sf::Color::White);
	textHiScore.Set(font, "", textSize, sf::Color::White);
	textMultiplier.Set(font, "", textSize, sf::Color::White);
	textMultiplier.SetActive(false);
	textGame.Set(font, "Game", 80.f, sf::Color::White);
	textOver.Set(font, "Over", 80.f, sf::Color::White);
	/*textChain.Set(font, "", 20.f, sf::Color::White);
	textChain.SetActive(false);*/
	textMessage.Set(font, "", textSize, sf::Color::White);
	textMessage.SetActive(false);
	textFps.Set(font, "", textSize, sf::Color::White);
	textFps.SetActive(false);

	itemTimer.setFillColor(sf::Color::White);
	itemTimer.setSize(itemTimerSize);

	Utils::SetOrigin(itemTimer, Origins::TC);
	textScore.SetOrigin(Origins::TC);
	textMultiplier.SetOrigin(Origins::TC);
	textHiScore.SetOrigin(Origins::TR);
	textGame.SetOrigin(Origins::BC);
	textOver.SetOrigin(Origins::TC);
	/*textChain.SetOrigin(Origins::BC);*/
	// Utils::SetOrigin(gaugeHp, Origins::BL);
	textMessage.SetOrigin(Origins::MC);
	textFps.SetOrigin(Origins::TR);

	// Top
	float topY = 25.f;
	itemTimer.setPosition({ referenceResolution.x / 2.f, topY });
	textScore.SetPosition({ referenceResolution.x / 2.f, topY + 10.f });
	textMultiplier.SetPosition({ referenceResolution.x / 2.f, topY + 60.f });
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
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));

	isGameOver = false;

	// textChainTimer = 0.f;
}

void UiHud::Update(float dt)
{
	//textChain.SetPosition(player->GetPosition() + sf::Vector2f(0.f, -30.f));
	if (player->GetUsingItem() != ItemType::NONE)
	{
		isUsingItem = true;
		SetItemTimer(player->GetItemDuration() - player->GetItemTimer(), player->GetItemDuration());
	}
	else
	{
		isUsingItem = false;
	}
	// SetMessage(std::to_string(player->GetItemDuration()) + ", " + std::to_string(player->GetItemTimer()));

	if (isGameOver)
	{
		if (gameOverTimer > gameOverMoveTime)
		{
			textGame.SetPosition(gameOverPosition - sf::Vector2f(0.f, textGameOverInterval));
			textOver.SetPosition(gameOverPosition + sf::Vector2f(0.f, textGameOverInterval));
		}
		else
		{
			float realDT = FRAMEWORK.GetRealDT();
			gameOverTimer += realDT;
			textGame.Translate(sf::Vector2f{ 1.f, 0.f } * gameOverSpeed * realDT);
			textOver.Translate(sf::Vector2f{ -1.f, 0.f } * gameOverSpeed * realDT);
		}
	}
}

void UiHud::LateUpdate(float dt)
{
}

void UiHud::FixedUpdate(float dt)
{
}

void UiHud::Draw(sf::RenderWindow& window)
{
	if (isUsingItem)
		window.draw(itemTimer);
	textScore.Draw(window);
	if (textMultiplier.GetActive())
		textMultiplier.Draw(window);
	textHiScore.Draw(window);
	//textChain.Draw(window);
	textMessage.Draw(window);
	// window.draw(gaugeHp);

	if (isGameOver)
	{
		textGame.Draw(window);
		textOver.Draw(window);
	}

	if (SCENE_MGR.GetDeveloperMode())
	{
		SetFps(FRAMEWORK.GetFps());
		textFps.Draw(window);
	}
}
