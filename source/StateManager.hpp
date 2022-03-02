#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP
#include "BaseState.hpp"
#include "IntroState.hpp"
#include "MenuState.hpp"
#include "GameState.hpp"
#include "PauseState.hpp"
#include "Window.hpp"
#include "SharedContext.hpp"

#include <unordered_map>
#include <functional>


enum class StateType
{
	Intro = 1, MainMenu, Game, Paused, GameOver, Win
};

class StateManager
{
public:
	StateManager(SharedContext* sharedContext);
	~StateManager();

	void Update(const sf::Time& time);
	void Draw();

	// Delete all states which has to be deleted in the end
	void ProcessRequests();

	SharedContext* GetSharedContext();

	bool HasState(const StateType& type);

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type);

private:

	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template<class T>
	void RegisterState(const StateType& type)
	{
		statesFactory[type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	SharedContext* sharedContext;
	std::vector<std::pair<StateType, BaseState*>> states;
	// States to be removed
	std::vector<StateType> toRemove;
	//Automatically produce objects of different types derived from Base State
	std::unordered_map<StateType, std::function<BaseState* (void)>> statesFactory;
};

#endif