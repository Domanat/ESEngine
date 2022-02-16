#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <SFML/Graphics.hpp>

class GameState : public sf::Drawable
{
public:
	virtual void Init() = 0;
	virtual void HandleInput(sf::Event event) = 0;
	virtual void Draw() = 0;
	virtual void CleanUp() = 0;

	virtual void Pause() = 0;
	virtual void Resume() = 0;

};

#endif