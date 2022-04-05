#include "BaseEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Util.h"

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

const DecisionTree* BaseEnemy::getTree()
{
	return m_tree;
}

void BaseEnemy::m_move()
{
	Seek();

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

void BaseEnemy::m_buildTree()
{
	// Create and add root node.
	m_tree->setDeathNode(new DeathCondition());
	m_tree->getTree().push_back(m_tree->getDeathNode());

	m_tree->setTakeDamageNode(new TakeDamageCondition());
	m_tree->addNode(m_tree->getDeathNode(), m_tree->getTakeDamageNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getTakeDamageNode());

	m_tree->setIdleNode(new IdleCondition());
	m_tree->addNode(m_tree->getTakeDamageNode(), m_tree->getIdleNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getIdleNode());

	m_tree->setLOSNode(new LOSCondition());
	m_tree->addNode(m_tree->getIdleNode(), m_tree->getLOSNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getLOSNode());

	m_tree->setRadiusNode(new RadiusCondition());
	m_tree->addNode(m_tree->getLOSNode(), m_tree->getRadiusNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getRadiusNode());

	m_tree->setCloseCombatNode(new CloseCombatCondition());
	m_tree->addNode(m_tree->getLOSNode(), m_tree->getCloseCombatNode(), RIGHT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getCloseCombatNode());

	TreeNode* deathNode = m_tree->addNode(m_tree->getDeathNode(), new DeathAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(deathNode)->setAgent(this);
	m_tree->getTree().push_back(deathNode);

	TreeNode* takeDamageNode = m_tree->addNode(m_tree->getTakeDamageNode(), new TakeDamageAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(takeDamageNode)->setAgent(this);
	m_tree->getTree().push_back(takeDamageNode);

	TreeNode* idleNode = m_tree->addNode(m_tree->getIdleNode(), new IdleAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(idleNode)->setAgent(this);
	m_tree->getTree().push_back(idleNode);

	TreeNode* patrolNode = m_tree->addNode(m_tree->getRadiusNode(), new PatrolAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(patrolNode)->setAgent(this);
	m_tree->getTree().push_back(patrolNode);

	TreeNode* moveToLOSNode = m_tree->addNode(m_tree->getRadiusNode(), new MoveToLOSAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToLOSNode)->setAgent(this);
	m_tree->getTree().push_back(moveToLOSNode);

	TreeNode* moveToPlayerNode = m_tree->addNode(m_tree->getCloseCombatNode(), new MoveToPlayerAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToPlayerNode)->setAgent(this);
	m_tree->getTree().push_back(moveToPlayerNode);

	TreeNode* attackNode = m_tree->addNode(m_tree->getCloseCombatNode(), new AttackAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(attackNode)->setAgent(this);
	m_tree->getTree().push_back(attackNode);
}
