#include "GameState.hpp"

GameState::GameState(StateManager* stateManager) :
	BaseState(stateManager)
{
}

GameState::~GameState()
{
}

void GameState::OnCreate()
{
	texture.loadFromFile("Mushroom.png");
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	increment = sf::Vector2f(400, 400);

	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;
	eventManager->AddCallback(StateType::Game, "Key_Escape", &GameState::MainMenu, this);
	eventManager->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);
}

void GameState::OnDestroy()
{
	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;
	eventManager->RemoveCallback(StateType::Game, "Key_Escape");
	eventManager->RemoveCallback(StateType::Game, "Key_P");
}

void GameState::Update(const sf::Time& time)
{
	sf::Vector2u windowSize = stateManager->GetSharedContext()->window->GetWindowSize();

	sf::Vector2u textureSize = texture.getSize();

	if ((sprite.getPosition().x > windowSize.x - textureSize.x && increment.x > 0) ||
		(sprite.getPosition().x < 0 && increment.x < 0))
	{
		increment.x = -increment.x;
	}

	if ((sprite.getPosition().y > windowSize.y - textureSize.y && increment.y > 0) ||
		(sprite.getPosition().y < 0 && increment.y < 0))
	{
		increment.y = -increment.y;
	}

	sprite.setPosition(sprite.getPosition().x + (increment.x * time.asSeconds()), 
						sprite.getPosition().y + (increment.y * time.asSeconds()));

}

void GameState::Draw()
{
	stateManager->GetSharedContext()->window->GetRenderWindow()->draw(sprite);
}

void GameState::MainMenu(EventDetails* details)
{
	stateManager->SwitchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details)
{
	//stateManager->SwitchTo(StateType::Pause);
}

void GameState::Activate() {}
void GameState::Deactivate() {}