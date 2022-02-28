#include "StateManager.hpp"

StateManager::StateManager(SharedContext* sharedContext) :
	sharedContext(sharedContext)
{
	RegisterState<IntroState>(StateType::Intro);
	RegisterState<MenuState>(StateType::MainMenu);
	/*RegisterState<PauseState>(StateType::Paused);
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

	if (states.size() > 1 && states.back().second->IsTransparent())
	{
		auto itr = states.end();
		while (itr != states.begin())
		{
			if (itr != states.end() && !itr->second->IsTransparent())
				break;

			--itr;
		}

		for (; itr != states.end(); itr++)
		{
			itr->second->Draw();
		}
	}
	else
	{
		states.back().second->Draw();
	}
}

void StateManager::Update(const sf::Time& time)
{
	if (states.empty())
		return;
	
	if (states.size() > 1 && states.back().second->IsTranscendent())
	{
		auto itr = states.end();

		while (itr != states.begin())
		{
			if (itr != states.end() && !itr->second->IsTranscendent())
				break;

			itr--;
		}

		for (; itr != states.end(); itr++)
		{
			itr->second->Update(time);
		}
	}
	else
	{
		states.back().second->Update(time);
	}
}

SharedContext* StateManager::GetSharedContext()
{
	return sharedContext;
}

bool StateManager::HasState(const StateType& type)
{
	for (auto itr = states.begin(); itr != states.end(); itr++)
	{
		if (itr->first == type)
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
	sharedContext->eventManager->SetCurrentState(type);
	
	for (auto itr = states.begin(); itr != states.end(); itr++)
	{
		if (itr->first == type)
		{
			states.back().second->Deactivate();
			StateType tempType = itr->first;
			BaseState* tempState = itr->second;

			states.erase(itr);
			states.emplace_back(tempType, tempState);
			tempState->Activate();
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