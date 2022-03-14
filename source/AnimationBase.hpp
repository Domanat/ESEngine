#ifndef ANIM_BASE_HPP
#define ANIM_BASE_HPP

#include <string>
#include <iostream>

class SpriteSheet;

class AnimationBase
{
	friend class SpriteSheet;

public:
	AnimationBase();

	virtual ~AnimationBase();

	void SetSpriteSheet(SpriteSheet* sheet);
	void SetFrame(int frame);
	void SetStartFrame(int frame);
	void SetEndFrame(int frame);
	void SetFrameRow(int row);
	void SetActionStart(int frame);
	void SetActionEnd(int frame);
	void SetFrameTime(float time);
	void SetLooping(bool loop);
	void SetName(const std::string& name);

	SpriteSheet* GetSpriteSheet();
	int GetFrame();
	int GetStartFrame();
	int GetEndFrame();
	int GetFrameRow();
	int GetActionStart();
	int GetActionEnd();
	float GetFrameTime();
	float GetElapsedTime();
	bool IsLooping();
	bool IsPlaying();
	bool IsInAction();
	std::string GetName();

	void Play();
	void Pause();
	void Stop();
	void Reset();

	virtual void Update(const float& dt);

	friend std::stringstream& operator>>(std::stringstream& stream, AnimationBase& a)
	{
		a.ReadIn(stream);
		return stream;
	}

protected:
	virtual void FrameStep() = 0;
	virtual void CropSprite() = 0;
	virtual void ReadIn(std::stringstream& stream) = 0;

	int frameCurrent;
	int frameStart;
	int frameEnd;
	int frameRow;
	int frameActionStart;
	int frameActionEnd;
	float frameTime;
	float elapsedTime;
	bool loop;
	bool playing;
	std::string name;
	SpriteSheet* spriteSheet;
};

#endif