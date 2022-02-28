#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "BaseState.hpp"
#include "StateManager.hpp"
#include "EventManager.hpp"

class GameState : public BaseState
{
public:
	GameState(StateManager* stateManager);
	~GameState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void MainMenu(EventDetails* details);
	void Pause(EventDetails* details);

private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f increment;
};

#endif