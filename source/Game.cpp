#include "Game.hpp"

Game::Game() :
	window(sf::VideoMode(800, 600), "Test"),
	stateManager(&window)
{
	stateManager.SwitchTo(StateType::MainMenu);
}

Game::~Game() { }

void Game::Update()
{
	//window.update();
	stateManager.Update(elapsed);
}

void Game::Render()
{
	//window.BeginDraw();
	stateManager.Draw();
	//window.EndDraw();
}

void Game::LateUpdate()
{
	stateManager.ProcessRequests();
	RestartClock();
}

sf::RenderWindow* Game::GetWindow()
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