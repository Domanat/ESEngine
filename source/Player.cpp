#include "Player.hpp"
#include "EntityManager.hpp"
#include "StateManager.hpp"

Player::Player(EntityManager* manager) :
	Character(manager)
{
	Load("Player.char");
	type = EntityType::Player;

	EventManager* events = entityManager->GetSharedContext()->eventManager;
	events->AddCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_MoveRight", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Jump", &Player::React, this);
	events->AddCallback<Player>(StateType::Game, "Player_Attack", &Player::React, this);
}

Player::~Player()
{
	EventManager* eventManager = entityManager->GetSharedContext()->eventManager;
	eventManager->RemoveCallback(StateType::Game, "Player_MoveLeft");
	eventManager->RemoveCallback(StateType::Game, "Player_MoveRight");
	eventManager->RemoveCallback(StateType::Game, "Player_Jump");
	eventManager->RemoveCallback(StateType::Game, "Player_Attack");
}

void Player::OnEntityCollision(EntityBase* collider, bool attack)
{
	if (state == EntityState::Dying)
		return;

	if (attack)
	{
		if (state != EntityState::Attacking)
			return;
		if (!spriteSheet.GetCurrentAnimation()->IsInAction())
			return;

		if (collider->GetType() != EntityType::Enemy &&
			collider->GetType() != EntityType::Player)
			return;

		Character* opponent = (Character*)collider;
		opponent->GetHurt(1);
		
		if (position.x > opponent->GetPosition().x)
			opponent->AddVelocity(-32, 0);
		else
			opponent->AddVelocity(32, 0);
	}
	else
	{
		//other behavior
	}
}

void Player::React(EventDetails* details) {
	if (details->m_name == "Player_MoveLeft") {
		Character::Move(Direction::Left);
	}
	else if (details->m_name == "Player_MoveRight") {
		Character::Move(Direction::Right);
	}
	else if (details->m_name == "Player_Jump") {
		Character::Jump();
	}
	else if (details->m_name == "Player_Attack") {
		Character::Attack();
	}
}