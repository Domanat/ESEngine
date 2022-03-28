#include "IntroState.hpp"
#include "StateManager.hpp"

IntroState::IntroState(StateManager* stateManager)
	: BaseState(stateManager) {}

IntroState::~IntroState() {}

void IntroState::OnCreate() 
{
	timePassed = 0.0f;

	sf::Vector2u windowSize = stateManager->GetSharedContext()->window->GetRenderWindow()->getSize();

	TextureManager* textureMgr = stateManager->GetSharedContext()->textureManager;
	textureMgr->RequireResource("Intro");
	introSprite.setTexture(*textureMgr->GetResource("Intro"));
	introSprite.setOrigin(textureMgr->GetResource("Intro")->getSize().x / 2.0f,
		textureMgr->GetResource("Intro")->getSize().y / 2.0f);

	introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 3.0f);

	font.loadFromFile(Utils::GetResourceDirectory() + "media\\Fonts\\arial.ttf");
	text.setFont(font);
	text.setString({ "Press SPACE to continue" });
	text.setCharacterSize(15);

	sf::FloatRect textRect = text.getLocalBounds();

	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	text.setPosition(introSprite.getPosition().x,
		introSprite.getPosition().y + textureMgr->GetResource("Intro")->getSize().y / 1.5f);

	EventManager* evMgr = stateManager->GetSharedContext()->eventManager;
	evMgr->AddCallback(StateType::Intro, "Intro_Continue", &IntroState::Continue, this);
}

void IntroState::OnDestroy() 
{
	TextureManager* textureMgr = stateManager->GetSharedContext()->textureManager;
	textureMgr->ReleaseResource("Intro");

	EventManager* evMgr = stateManager->GetSharedContext()->eventManager;
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
	sf::RenderWindow* window = stateManager->GetSharedContext()->window->GetRenderWindow();

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