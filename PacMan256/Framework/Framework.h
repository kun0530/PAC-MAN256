#pragma once
#include "Singleton.h"

// 1. 초기화 / 메인루프 / 정리
// 2. 시간 관련 기능 / 윈도우 정보
// 3. ...Mgr 

class Framework : public Singleton<Framework>
{
	friend Singleton<Framework>;

protected:
	Framework() = default;
	virtual ~Framework() = default;

	sf::RenderWindow window;
	sf::Vector2i windowSize;
	float fixedUpdateTime = 1.f / 50.f;

	sf::Clock clock;
	float timeScale = 1.f;

	sf::Time realTime; // 게임 시작부터 경과 시간
	sf::Time time; // 게임 시작부터 경과 시간 (timeScale 적용된...)

	sf::Time realDeltaTime;
	sf::Time deltaTime;

	sf::Time fixedDeltaTime;

	float fpsTimer = 0.f;
	int fpsCount = 0;
	int fps = 0;

public:
	sf::RenderWindow& GetWindow() { return window; }	// !!
	const sf::Vector2i& GetWindowSize() const { return windowSize; }

	float GetRealTime() const { return realTime.asSeconds(); }
	float GetTime() const { return time.asSeconds(); }
	float GetRealDT() const { return realDeltaTime.asSeconds(); }
	float GetDT() const { return deltaTime.asSeconds(); }

	float GetTimeScale() const { return timeScale; }
	void SetTimeScale(float scale) { timeScale = scale; }

	int GetFps() const { return fps; }

	virtual void Init(int width, int height, const std::string& name = "Game");
	virtual void Do();
	virtual void Release();

};

#define FRAMEWORK (Singleton<Framework>::Instance())

