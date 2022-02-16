#ifndef ENGINE_HPP
#define ENGINE_HPP

#include "SFML/Graphics.hpp"
#include "StateManager.hpp"
#include "Constants.hpp"

class Engine
{
public:
	Engine();

	void Init();

	void Draw();
	void Render();
private:

	sf::RenderWindow window;
	StateManager stateManager;
};

#endif 
