#include "AnimationBase.hpp"
#include "SpriteSheet.hpp"

AnimationBase::AnimationBase() :
	frameCurrent(0), frameStart(0), frameEnd(0),
	frameRow(0), frameActionStart(-1), frameActionEnd(-1),
	loop(false), playing(false)
{

}

AnimationBase::~AnimationBase()
{

}

void AnimationBase::SetSpriteSheet(SpriteSheet* sheet)
{
	spriteSheet = sheet;
}

void AnimationBase::SetFrame(int frame)
{
	if ((frame >= frameStart && frame <= frameEnd) ||
		(frame >= frameEnd && frame <= frameStart))
	{
		frameCurrent = frame;
	}
}

bool AnimationBase::IsInAction()
{
	if (frameActionStart == -1 || frameActionEnd == -1)
		return true;

	return (frameCurrent >= frameActionStart && frameCurrent <= frameActionEnd);
}

void AnimationBase::Play()
{
	playing = true;
}

void AnimationBase::Pause()
{
	playing = false;
}

void AnimationBase::Stop()
{
	playing = false;
	Reset();
}

void AnimationBase::Reset()
{
	frameCurrent = frameStart;
	elapsedTime = 0;
	CropSprite();
}

void AnimationBase::Update(const float& dt)
{
	if (!playing)
		return;

	elapsedTime += dt;
	
	if (elapsedTime < frameTime)
		return;
	FrameStep();
	CropSprite();
	elapsedTime = 0;
}

void AnimationBase::SetSpriteSheet(SpriteSheet* l_sheet) { spriteSheet = l_sheet; }
void AnimationBase::SetStartFrame(int l_frame) { frameStart = l_frame; }
void AnimationBase::SetEndFrame(int l_frame) { frameEnd = l_frame; }
void AnimationBase::SetFrameRow(int l_row) { frameRow = l_row; }
void AnimationBase::SetActionStart(int l_frame) { frameActionStart = l_frame; }
void AnimationBase::SetActionEnd(int l_frame) { frameActionEnd = l_frame; }
void AnimationBase::SetFrameTime(float l_time) { frameTime = l_time; }
void AnimationBase::SetLooping(bool l_loop) { loop = l_loop; }
void AnimationBase::SetName(const std::string& l_name) { name = l_name; }

SpriteSheet* AnimationBase::GetSpriteSheet() { return spriteSheet; }
int AnimationBase::GetFrame() { return frameCurrent; }
int AnimationBase::GetStartFrame() { return frameStart; }
int AnimationBase::GetEndFrame() { return frameEnd; }
int AnimationBase::GetFrameRow() { return frameRow; }
int AnimationBase::GetActionStart() { return frameActionStart; }
int AnimationBase::GetActionEnd() { return frameActionEnd; }
float AnimationBase::GetFrameTime() { return frameTime; }
float AnimationBase::GetElapsedTime() { return elapsedTime; }
std::string AnimationBase::GetName() { return name; }
bool AnimationBase::IsLooping() { return loop; }
bool AnimationBase::IsPlaying() { return playing; }