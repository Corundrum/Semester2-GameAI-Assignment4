#include "BaseEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Util.h"
#include "PlayScene.h"

#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"
#include "IdleAction.h"
#include "DeathAction.h"
#include "TakeDamageAction.h"

BaseEnemy::BaseEnemy()
{
}

BaseEnemy::~BaseEnemy()
= default;

float BaseEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

float BaseEnemy::getTurnRate() const
{
	return m_turnRate;
}

float BaseEnemy::getAccelerationRate() const
{
	return m_accelerationRate;
}

glm::vec2 BaseEnemy::getDesiredVelocity() const
{
	return m_desiredVelocity;
}

void BaseEnemy::setMaxSpeed(const float speed)
{
	m_maxSpeed = speed;
}

void BaseEnemy::setTurnRate(const float angle)
{
	m_turnRate = angle;
}

void BaseEnemy::setAccelerationRate(const float rate)
{
	m_accelerationRate = rate;
}

void BaseEnemy::setDesiredVelocity(const glm::vec2 target_position)
{
	m_desiredVelocity = Util::normalize(target_position - getTransform()->position);
}

void BaseEnemy::drawHealthBar()
{
	float health_percent = (float)getHealth() / getMaxHealth();

	Util::DrawFilledRect(glm::vec2(getTransform()->position.x - getWidth() / 2 + 7, getTransform()->position.y - 14), 48, 8, glm::vec4(0, 0, 0, 1));
	if (getHealth() > 0)
	{
		Util::DrawFilledRect(glm::vec2(getTransform()->position.x - getWidth() / 2 + 7, getTransform()->position.y - 14), 48 * health_percent, 8, glm::vec4(1, 0.2, 0.2, 1));
	}
}

void BaseEnemy::Seek()
{
	// Find next waypoint:
	if (Util::distance(m_patrol[m_waypoint], getTransform()->position) < 10)
	{
		//if moved to last waypoint go back to beginning
		if (++m_waypoint == m_patrol.size())
		{
			m_waypoint = 0;
		}
		setTargetPosition(m_patrol[m_waypoint]);
	}

	setDesiredVelocity(getTargetPosition());
	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();
	LookWhereYoureGoing(steering_direction);
	getRigidBody()->acceleration = getCurrentDirection() * getAccelerationRate();
}

void BaseEnemy::Flee()
{
	// Find next waypoint:
	if (Util::distance(m_patrol[m_waypoint], getTransform()->position) < 10)
	{
		//if moved to last waypoint go back to beginning
		if (++m_waypoint == m_patrol.size())
		{
			m_waypoint = 0;
		}
		setTargetPosition(m_patrol[m_waypoint]);
	}

	setDesiredVelocity(getTargetPosition());
	const glm::vec2 steering_direction = getDesiredVelocity() - getCurrentDirection();
	LookWhereYoureGoing(steering_direction);
	getRigidBody()->acceleration = -getCurrentDirection() * getAccelerationRate();
}

void BaseEnemy::LookWhereYoureGoing(const glm::vec2 target_direction)
{
	float target_rotation = Util::signedAngle(getCurrentDirection(), target_direction) - 90;

	const float turn_sensitivity = 3.0f;

	if (getCollisionWhiskers()[0])
	{
		setCurrentHeading(getCurrentHeading() + getTurnRate());
	}
	else if (getCollisionWhiskers()[2])
	{
		setCurrentHeading(getCurrentHeading() - getTurnRate());
	}
	else if (abs(target_rotation) > turn_sensitivity)
	{
		if (target_rotation > 0.0f)
		{
			setCurrentHeading(getCurrentHeading() + getTurnRate());
		}
		else if (target_rotation < 0.0f)
		{
			setCurrentHeading(getCurrentHeading() - getTurnRate());
		}
	}

	updateWhiskers(getWhiskerAngle());
}

PathNode* BaseEnemy::findNextNode()
{
	std::vector<PathNode*> closest_points;
	PathNode* next_tile = nullptr;

	for (unsigned i = 0; i < PlayScene::m_pGrid.size(); i++)
	{
		auto distance = Util::distance(this->getTransform()->position, PlayScene::m_pGrid[i]->getTransform()->position);
		if (distance <= 60 && distance > 20)
		{
			closest_points.push_back(PlayScene::m_pGrid[i]);
		}
	}

	for (unsigned i = 0; i < closest_points.size(); i++)
	{
		if (next_tile == nullptr)
		{
			next_tile = closest_points[i];
		}
		else
		{
			if (Util::distance(closest_points[i]->getTransform()->position, Player::s_pPlayerObj->getTransform()->position) < Util::distance(next_tile->getTransform()->position, Player::s_pPlayerObj->getTransform()->position))
			{
				next_tile = closest_points[i];
			}
		}
	}

	return next_tile;
}

const DecisionTree* BaseEnemy::getTree()
{
	return m_tree;
}

void BaseEnemy::m_move(bool flee)
{
	if (flee)
	{
		Flee();
	}
	else
	{
		Seek();
	}

	//                                   final Position     position term    velocity term     acceleration term
	// kinematic equation for motion --> Pf            =      Pi     +     Vi*(time)    +   (0.5)*Ai*(time * time)

	const float dt = TheGame::Instance().getDeltaTime();

	// compute the position term
	const glm::vec2 initial_position = getTransform()->position;

	// compute the velocity term
	const glm::vec2 velocity_term = getRigidBody()->velocity * dt;

	// compute the acceleration term
	const glm::vec2 acceleration_term = getRigidBody()->acceleration * 0.5f;// *dt;


	// compute the new position
	glm::vec2 final_position = initial_position + velocity_term + acceleration_term;

	getTransform()->position = final_position;

	// add our acceleration to velocity
	getRigidBody()->velocity += getRigidBody()->acceleration;

	// clamp our velocity at max speed
	getRigidBody()->velocity = Util::clamp(getRigidBody()->velocity, getMaxSpeed());
}


