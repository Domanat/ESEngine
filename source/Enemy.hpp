#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"


class Enemy : public Character
{
public:
	Enemy(EntityManager* manager);
	~Enemy();

	void OnEntityCollision(EntityBase* collider, bool attack);
	void Update(float dt);

private:
	sf::Vector2f destination;
	bool hasDestination;
};

#endif