#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"
#include "EventManager.hpp"

class Player : public Character
{
	friend class EntityManager;
public:
	Player(EntityManager* entityManager);
	~Player();

	void OnEntityCollision(EntityBase* collider, bool attack);
	void React(EventDetails* details);

};

#endif 