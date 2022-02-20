#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP
#include "MenuState.hpp"
#include "BaseState.hpp"

#include <unordered_map>
#include <functional>

enum class StateType
{
	MainMenu = 1, Game, Paused, GameOver, Win
};


class StateManager
{
public:
	StateManager(sf::RenderWindow* window);
	~StateManager();

	void Update(const sf::Time& time);
	void Draw();

	// Delete all states which has to be deleted in the end
	void ProcessRequests();

	sf::RenderWindow* GetWindow();

	bool HasState(const StateType& type);

	void SwitchTo(const StateType& type);
	void Remove(const StateType& type);

private:

	void CreateState(const StateType& type);
	void RemoveState(const StateType& type);

	template<class T>
	void RegisterState(const StateType& type)
	{
		statesFactory[type] = [this]()->BaseState*
		{
			return new T(this);
		}
	}

	sf::RenderWindow* window;
	std::vector<std::pair<StateType, BaseState*>> states;
	// States to be removed
	std::vector<StateType> toRemove;
	//Automatically produce objects of different types derived from Base State
	std::unordered_map<StateType, std::function<BaseState* (void)>> statesFactory;
};

#endif