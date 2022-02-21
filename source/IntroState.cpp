#include "IntroState.hpp"
#include "StateManager.hpp"

IntroState::IntroState(StateManager* stateManager)
	: BaseState(stateManager) {}

IntroState::~IntroState() {}

void IntroState::OnCreate() 
{
	timePassed = 0.0f;

	//sf::Vector2u windowSize = stateManager->GetRenderWindow()->getSize();

	//Error here
	sf::Vector2u windowSize = stateManager->GetRenderWindow()->getSize();
	//----------------


	introTexture.loadFromFile("intro.png");
	introSprite.setTexture(introTexture);
	introSprite.setOrigin(introTexture.getSize().x / 2.0f,
		introTexture.getSize().y / 2.0f);

	introSprite.setPosition(windowSize.x / 2.0f, 0);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString({ "Press SPACE to continue" });
	text.setCharacterSize(15);

	sf::FloatRect textRect = text.getLocalBounds();

	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* evMgr = stateManager->GetEventManager();
	evMgr->AddCallback(StateType::Intro, "Intro_Continue", &IntroState::Continue, this);
}

void IntroState::OnDestroy() 
{
	EventManager* evMgr = stateManager->GetEventManager();
	evMgr->RemoveCallback(StateType::Intro, "Intro_Continue");
}

void IntroState::Update(const sf::Time& time) 
{
	if (timePassed < 5.0f) 
	{ // Less than five seconds.

		timePassed += time.asSeconds();
		introSprite.setPosition(
			introSprite.getPosition().x,
			introSprite.getPosition().y + (48 * time.asSeconds()));
	}
}

void IntroState::Draw() 
{
	sf::RenderWindow* window = stateManager->GetRenderWindow();

	window->draw(introSprite);

	if (timePassed >= 5.0f) 
	{
		window->draw(text);
	}
}

void IntroState::Continue(EventDetails* details) {
	if (timePassed >= 5.0f) 
	{
		stateManager->SwitchTo(StateType::MainMenu);
		stateManager->Remove(StateType::Intro);
	}
}

void IntroState::Activate() {}
void IntroState::Deactivate() {}