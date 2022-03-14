#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include "SFML/Graphics.hpp"
#include "AnimationBase.hpp"
#include "AnimationDirectional.hpp"
#include "TextureManager.hpp"
#include "Direction.hpp"
#include <fstream>

class SpriteSheet
{
public:
	SpriteSheet(TextureManager* textureManager);
	~SpriteSheet();

	void CropSprite(const sf::IntRect& rect);

	sf::Vector2i GetSpriteSize()const;
	sf::Vector2f GetSpritePosition()const;
	void SetSpriteSize(const sf::Vector2i& size);
	void SetSpritePosition(const sf::Vector2f& pos);

	void SetDirection(const Direction& direction);
	Direction GetDirection()const;

	bool LoadSheet(const std::string& file);
	void ReleaseSheet();

	AnimationBase* GetCurrentAnimation();
	bool SetAnimation(const std::string& name, const bool& play = false, const bool& loop = false);

	void Update(const float& dt);
	void Draw(sf::RenderWindow* window);

private:
	std::string texture;
	sf::Sprite sprite;
	sf::Vector2i spriteSize;
	sf::Vector2f spriteScale;
	Direction direction;
	std::string animationType;
	std::unordered_map<std::string, AnimationBase*> animations;
	AnimationBase* animationCurrent;
	TextureManager* textureManager;
};

#endif