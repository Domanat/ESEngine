#ifndef BASE_STATE_HPP
#define BASE_STATE_HPP
#include <SFML/Graphics.hpp>

class StateManager;


class BaseState
{
public:
	BaseState(StateManager* stateManager) :
		stateManager(stateManager)
	{

	}

	virtual ~BaseState() {}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	// Set state to the top on the stack
	virtual void Activate() = 0;
	// Remove state from the top of the stack
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& value) { isTransparent = value; }
	bool IsTransparent()const { return isTransparent; }
	void SetTranscendent(const bool& value) { isTranscendent = value; }
	bool IsTranscendent()const { return isTranscendent; }

	StateManager* GetStateManager() { return stateManager; }

protected:
	StateManager* stateManager;

	bool isTransparent;
	bool isTranscendent;
};

#endif