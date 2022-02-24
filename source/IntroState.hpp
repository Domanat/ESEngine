#ifndef INTROSTATE_HPP
#define INTROSTATE_HPP

#include "BaseState.hpp"
#include "EventManager.hpp"

class IntroState : public BaseState 
{
public:
	IntroState(StateManager* stateManager);
	~IntroState();

	void OnCreate();
	void OnDestroy();

	void Activate();
	void Deactivate();

	void Update(const sf::Time& time);
	void Draw();

	void Continue(EventDetails* details);
private:
	sf::Texture introTexture;
	sf::Sprite introSprite;
	sf::Font font;
	sf::Text text;

	float timePassed;
};

#endif