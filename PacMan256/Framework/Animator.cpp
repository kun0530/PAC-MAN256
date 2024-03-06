#include "pch.h"
#include "Animator.h"
#include "rapidcsv.h"

Animator::Animator()
{
}

Animator::~Animator()
{
}

void Animator::AddEvent(const std::string& clipId, int frame, std::function<void()> action)
{
	eventList.push_back({ clipId, frame, action });
}

void Animator::ClearEvent()
{
	eventList.clear();
}

//void Animator::AddClip(const AnimationClip& clip)
//{
//	if (clips.find(clip.id) == clips.end()) // 중복을 허용하지 않는 삽입
//	{
//		// clips.insert({ clip.id, clip }); 아래와 동일한 문법
//		clips[clip.id] = clip;
//	}
//}

void Animator::SetFrame(const AnimationFrame& frame)
{
	target->setTexture(frame.GetTexture());
	target->setTextureRect(frame.texCoord);
}

void Animator::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt * speed;
	if (accumTime < clipDuration)
		return;
	
	accumTime = 0.f;
	currentFrame += addFrame;

	if (currentFrame == totalFrame || currentFrame == -1)
	{
		if (!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();

			Play(id, false);

			return;
		}

		switch (currentClip->loopType)
		{
		case AnimationLoopTypes::Single:
			currentFrame = totalFrame - 1;
			Stop();
			break;
		case AnimationLoopTypes::Loop:
			currentFrame = 0;
			break;
		case AnimationLoopTypes::PingPong:
			if (addFrame > 0)
			{
				addFrame = -1;
				currentFrame = totalFrame - 2;
				totalFrame = -1;
			}
			else
			{
				addFrame = 1;
				currentFrame = totalFrame + 2;
				totalFrame = currentClip->frames.size();
			}

			break;
		}
	}

	for (auto& event : eventList)
	{
		if (currentClip->id == event.clipId && currentFrame == event.frame)
		{
			if (event.action != nullptr)
			{
				event.action();
			}
		}
	}

	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(const std::string& clipId, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop();
		}
	}

	isPlaying = true;
	addFrame = 1;
	accumTime = 0.f;

	currentClip = &RES_MGR_ANI_CLIP.Get(clipId);
	currentFrame = 0;
	totalFrame = currentClip->GetTotalFrame();
	clipDuration = 1.f / currentClip->fps;
	SetFrame(currentClip->frames[0]);
}

void Animator::PlayQueue(const std::string& clipId)
{
	queue.push(clipId);
}

void Animator::Stop()
{
	isPlaying = false;
}

bool AnimationClip::loadFromFile(const std::string& filePath)
{
	rapidcsv::Document doc(filePath);

	// id = doc.GetCell<std::string>(0, 0);
	id = filePath;
	fps = doc.GetCell<int>(1, 0);
	loopType = (AnimationLoopTypes)doc.GetCell<int>(2, 0);

	for (int i = 3; i < doc.GetRowCount(); ++i)
	{
		auto row = doc.GetRow<std::string>(i);
		frames.push_back({ row[0], { std::stoi(row[1]), std::stoi(row[2]),
			std::stoi(row[3]), std::stoi(row[4]) } });
	}

	return true;
}
