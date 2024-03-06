#pragma once

enum class AnimationLoopTypes
{
	Single,
	Loop,
	PingPong
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect texCoord;

	AnimationFrame(const std::string& id, const sf::IntRect& coord)
		: textureId(id), texCoord(coord)
	{
	}

	const sf::Texture& GetTexture() const
	{
		return RES_MGR_TEXTURE.Get(textureId);
	}
};

struct AnimationClip
{
	std::string id;
	std::vector<AnimationFrame> frames;
	AnimationLoopTypes loopType = AnimationLoopTypes::Single;
	int fps = 30;

	int GetTotalFrame() const
	{
		return frames.size();
	}

	bool loadFromFile(const std::string& filePath);
};

struct AnimationEvent
{
	std::string clipId;
	int frame = 0;
	std::function<void()> action; //  함수 객체
};

class Animator
{
protected:
	// std::unordered_map<std::string, AnimationClip> clips; // 임시로 사용할 컨테이너
	std::queue<std::string> queue;
	std::list<AnimationEvent> eventList;

	float speed = 1.f; // animationClip의 fps의 배율: 1.f인 경우, 그대로

	bool isPlaying = false;
	int addFrame = 1;
	AnimationClip* currentClip = nullptr; // 현재 재생 중인 애니메이션
	int totalFrame = 0;
	int currentFrame = -1;
	float clipDuration = 0.f; // fps와 speed로 계산

	float accumTime = 0.f;

	sf::Sprite* target = nullptr; // 애니메이션 대상

public:
	// 생성자, 소멸자
	Animator();
	~Animator();



	// void AddClip(const AnimationClip& clip);
	void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
	void ClearEvent();



	// Get, Set 함수
	bool IsPlaying() const { return isPlaying; }
	const std::string& GetCurrentClipId() { return currentClip->id; }

	sf::Sprite* GetTarget() const { return target; }
	void SetTarget(sf::Sprite* t) { target = t; }

	float GetSpeed() const { return speed; }
	void SetSpeed(float s) { speed = s; }

	void SetFrame(const AnimationFrame& frame);



	void Update(float dt);

	void Play(const std::string& clipId, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);
	void Stop();
};

