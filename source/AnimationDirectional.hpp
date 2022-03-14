#ifndef ANIMATIONDIRECTIONAL
#define ANIMATIONDIRECTIONAL

#include "AnimationBase.hpp"
#include "Direction.hpp"

class AnimationDirectional : public AnimationBase
{
protected:
	void FrameStep();
	void CropSprite();
	void ReadIn(std::stringstream& stream);
};

#endif