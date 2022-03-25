#ifndef GAME_HPP
#define GAME_HPP

#include "StateManager.hpp"
#include "SharedContext.hpp"
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

	SharedContext sharedContext;
	Window window;
	StateManager stateManager;
	EntityManager entityManager;
	sf::Clock clock;
	sf::Time elapsed;
	sf::Vector2i speed;
};
#endif