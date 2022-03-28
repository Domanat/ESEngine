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

bool SpriteSheet::SetAnimation(const std::string& name, const bool& play, const bool& loop)
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

sf::Vector2i SpriteSheet::GetSpriteSize()const { return spriteSize; }
sf::Vector2f SpriteSheet::GetSpritePosition()const { return sprite.getPosition(); }
Direction SpriteSheet::GetDirection()const { return direction; }
AnimationBase* SpriteSheet::GetCurrentAnimation() { return animationCurrent; }

bool SpriteSheet::LoadSheet(const std::string& file)
{
	std::ifstream sheet;
	sheet.open(Utils::GetResourceDirectory() + file);

	if (sheet.is_open())
	{
		ReleaseSheet();
		std::string line;

		while (std::getline(sheet, line))
		{
			if (line[0] == '|')
				continue;

			std::stringstream keystream(line);
			std::string type;
			keystream >> type;

			if (type == "Texture")
			{
				if (texture != "")
				{
					std::cerr << "Duplicate texture entries in " << file << std::endl;
					continue;
				}

				std::string newTexture;
				keystream >> newTexture;

				if (!textureManager->RequireResource(newTexture))
				{
					std::cerr << "Could not set up the texture: " << newTexture << std::endl;
					continue;
				}

				texture = newTexture;
				sprite.setTexture(*textureManager->GetResource(texture));
			}
			else if (type == "Size")
			{
				keystream >> spriteSize.x >> spriteSize.y;
				SetSpriteSize(spriteSize);
			}
			else if (type == "Scale")
			{
				keystream >> spriteScale.x >> spriteScale.y;
				sprite.setScale(spriteScale);
			}
			else if (type == "AnimationType")
			{
				keystream >> animationType;
			}
			else if (type == "Animation")
			{
				std::string name;
				keystream >> name;

				if (animations.find(name) != animations.end())
				{
					std::cerr << "Duplicate animation " << name << " in " << file << std::endl;
					continue;
				}

				AnimationBase* animation = nullptr;

				if (animationType == "Directional")
					animation = new AnimationDirectional;
				else
				{
					std::cerr << "Unknown animation type: " << animationType << std::endl;
					continue;
				}

				keystream >> *animation;
				animation->SetSpriteSheet(this);
				animation->SetName(name);
				animation->Reset();
				animations.emplace(name, animation);

				if (animationCurrent)
					continue;

				animationCurrent = animation;
				animationCurrent->Play();
			}
		}

		sheet.close();
		return true;
	}

	std::cerr << "Failed loading spritesheet " << file << std::endl;
	return false;
}