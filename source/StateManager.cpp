#include "StateManager.hpp"

StateManager::StateManager(Window* window) :
	window(window)
{
	RegisterState<IntroState>(StateType::Intro);
	/*RegisterState<PauseState>(StateType::Paused);
	RegisterState<GameState>(StateType::Game);
	RegisterState<LostState>(StateType::GameOver);
	RegisterState<WinState>(StateType::Win);*/

}

StateManager::~StateManager()
{
	for (auto& itr : states)
	{
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Draw()
{
	if (states.empty())
		return;

	for (auto itr = states.end(); itr != states.begin(); itr--)
	{
		itr->second->Draw();
	}
}

void StateManager::Update(const sf::Time& time)
{
	if (states.empty())
		return;

	for (auto itr = states.end(); itr != states.begin(); itr--)
	{
		itr->second->Update(time);
	}
}

sf::RenderWindow* StateManager::GetRenderWindow()
{
	return window->GetRenderWindow();
}

EventManager* StateManager::GetEventManager()
{
	return window->GetEventManager();
}

bool StateManager::HasState(const StateType& type)
{
	for (auto itr : states)
	{
		if (itr.first == type)
		{
			auto removed = std::find(toRemove.begin(), toRemove.end(), type);

			if (removed == toRemove.end())
			{
				return true;
			}

			return false;
		}
	}

	return false;
}

void StateManager::Remove(const StateType& type)
{
	toRemove.push_back(type);
}

void StateManager::ProcessRequests()
{
	while (toRemove.begin() != toRemove.end())
	{
		RemoveState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& type)
{
	//EventManager->setCurrentState(type);
	for (auto itr = states.begin(); itr != states.end(); itr++)
	{
		if (itr->first == type)
		{
			states.back().second->Deactivate();
			StateType tempType = itr->first;
			BaseState* tmpState = itr->second;

			states.erase(itr);
			states.emplace_back(tempType, tmpState);

			tmpState->Activate();
			return;
		}
	}

	if (!states.empty())
	{
		states.back().second->Deactivate();
	}

	CreateState(type);
	states.back().second->Activate();
}

void StateManager::CreateState(const StateType& type)
{
	auto newState = statesFactory.find(type);

	if (newState == statesFactory.end())
		return;
	
	BaseState* state = newState->second();
	states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type)
{
	for (auto itr = states.begin(); itr != states.end(); itr++)
	{
		if (itr->first == type)
		{
			itr->second->OnDestroy();
			delete itr->second;
			states.erase(itr);
			return;
		}
	}
}