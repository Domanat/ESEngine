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
	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;
	eventManager->AddCallback(StateType::Game, "Key_Escape", &GameState::MainMenu, this);
	eventManager->AddCallback(StateType::Game, "Key_P", &GameState::Pause, this);

	sf::Vector2u size = stateManager->GetSharedContext()->window->GetWindowSize();
	view.setSize(size.x, size.y);
	view.setCenter(size.x / 2, size.y/2);
	view.zoom(0.6f);
	stateManager->GetSharedContext()->window->GetRenderWindow()->setView(view);

	gameMap = new Map(stateManager->GetSharedContext(), this);
	gameMap->LoadMap("media\\Maps\\map1.map");
}

void GameState::OnDestroy()
{
	EventManager* eventManager = stateManager->GetSharedContext()->eventManager;
	eventManager->RemoveCallback(StateType::Game, "Key_Escape");
	eventManager->RemoveCallback(StateType::Game, "Key_P");

	delete gameMap;
	gameMap = nullptr;
}

void GameState::Update(const sf::Time& time)
{
	SharedContext* context = stateManager->GetSharedContext();
	EntityBase* player = context->entityManager->Find("Player");
	if (!player)
	{
		std::cout << "Respawning player" << std::endl;
		context->entityManager->Add(EntityType::Player, "Player");
		player = context->entityManager->Find("Player");
		player->SetPosition(gameMap->GetPlayerStart());
	}
	else
	{
		view.setCenter(player->GetPosition());
		context->window->GetRenderWindow()->setView(view);
	}

	sf::FloatRect viewSpace = context->window->GetViewSpace();
	if (viewSpace.left <= 0) {
		view.setCenter(viewSpace.width / 2, view.getCenter().y);
		context->window->GetRenderWindow()->setView(view);
	}
	else if (viewSpace.left + viewSpace.width > (gameMap->GetMapSize().x + 1) * Sheet::TileSize) {
		view.setCenter(((gameMap->GetMapSize().x + 1) * Sheet::TileSize) - (viewSpace.width / 2), view.getCenter().y);
		context->window->GetRenderWindow()->setView(view);
	}

	gameMap->Update(time.asSeconds());
	stateManager->GetSharedContext()->entityManager->Update(time.asSeconds());
}

void GameState::Draw()
{
	gameMap->Draw();
	stateManager->GetSharedContext()->entityManager->Draw();
}

void GameState::MainMenu(EventDetails* details)
{
	stateManager->SwitchTo(StateType::MainMenu);
}

void GameState::Pause(EventDetails* details)
{
	stateManager->SwitchTo(StateType::Paused);
}

void GameState::Activate() {}
void GameState::Deactivate() {}