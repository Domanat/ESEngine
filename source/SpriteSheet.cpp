#include "SpriteSheet.hpp"

SpriteSheet::SpriteSheet(TextureManager* textureManager) :
	textureManager(textureManager),
	animationCurrent(nullptr),
	spriteScale(1.f, 1.f),
	direction(Direction::Right())
{

}

SpriteSheet::~SpriteSheet()
{
	ReleaseSheet();
}

void SpriteSheet::ReleaseSheet()
{
	textureManager->ReleaseResource(texture);
	animationCurrent = nullptr;

	while (animations.begin() != animations.end())
	{
		delete animations.begin()->second;
		animations.erase(animations.begin());;
	}
}

void SpriteSheet::SetSpriteSize(const sf::Vector2i& size)
{
	spriteSize = size;
	sprite.setOrigin(spriteSize.x / 2, spriteSize.y / 2);
}

void SpriteSheet::SetSpritePosition(const sf::Vector2f& position)
{
	sprite.setPosition(position);
}

void SpriteSheet::SetDirection(const Direction& dir)
{
	if (dir == direction)
		return;

	direction = dir;
	animationCurrent->CropSprite();
}

void SpriteSheet::CropSprite(const sf::IntRect& rect)
{
	sprite.setTextureRect(rect);
}

bool SpriteSheet::SetAnimation(const std::string& name, const bool& play = false, const bool& loop = false)
{
	auto itr = animations.find(name);

	if (itr == animations.end())
		return false;

	if (itr->second == animationCurrent)
		return false;

	if (animationCurrent)
		animationCurrent->Stop();

	animationCurrent = itr->second;
	animationCurrent->SetLooping(loop);
	if (play)
		animationCurrent->Play();
	
	animationCurrent->CropSprite();
	return true;
}

void SpriteSheet::Update(const float& dt)
{
	animationCurrent->Update(dt);
}

void SpriteSheet::Draw(sf::RenderWindow* window)
{
	window->draw(sprite);
}