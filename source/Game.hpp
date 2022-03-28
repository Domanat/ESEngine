#ifndef GAME_HPP
#define GAME_HPP

#include "Window.hpp"
#include "EventManager.hpp"
#include "StateManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include <iostream>

class Game 
{
public:
	Game();
	~Game();

	void Update();
	void Render();
	void LateUpdate();

	sf::Time GetElapsed();

	Window* GetWindow();
	void RestartClock();

private:

	sf::Clock clock;
	sf::Time elapsed;
	SharedContext sharedContext;
	Window window;
	EntityManager entityManager;
	TextureManager textureManager;
	StateManager stateManager;
};
#endif