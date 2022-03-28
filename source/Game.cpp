#include "Game.hpp"

Game::Game() :
	window(sf::Vector2u(800, 600), "Window"),
	stateManager(&sharedContext),
	entityManager(&sharedContext, 10)
{
	std::cout << Utils::GetResourceDirectory() << std::endl;

	clock.restart();
	srand(time(NULL));

	sharedContext.window = &window;
	sharedContext.eventManager = window.GetEventManager();

	sharedContext.window = &window;
	sharedContext.eventManager = window.GetEventManager();
	sharedContext.textureManager = &textureManager;
	sharedContext.entityManager = &entityManager;
	
	stateManager.SwitchTo(StateType::Intro);
}

Game::~Game() { }

void Game::Update()
{
	window.Update();
	stateManager.Update(elapsed);
}

void Game::Render()
{
	window.BeginDraw();
	stateManager.Draw();
	window.EndDraw();
}

void Game::LateUpdate()
{
	stateManager.ProcessRequests();
	RestartClock();
}

Window* Game::GetWindow()
{
	return &window;
}

sf::Time Game::GetElapsed() 
{ 
	return clock.getElapsedTime(); 
}

void Game::RestartClock()
{ 
	elapsed = clock.restart(); 
}