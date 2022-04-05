#include "Agent.h"

#include <iostream>

#include "Util.h"
#include "CollisionManager.h"

Agent::Agent()
{
	for (unsigned int i = 0; i < 3; i++)
	{
		m_lineColor[i] = glm::vec4(0, 1, 0, 1);
		m_collisionWhiskers[i] = false;
	}
	/*m_lineColor[0] = glm::vec4(0, 1, 0, 1);
	m_lineColor[1] = glm::vec4(0, 1, 0, 1);
	m_lineColor[2] = glm::vec4(0, 1, 0, 1);

	m_collisionWhiskers[0] = false;
	m_collisionWhiskers[1] = false;
	m_collisionWhiskers[2] = false;*/
}

Agent::~Agent()
= default;

glm::vec2 Agent::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Agent::getCurrentDirection() const
{
	return m_currentDirection;
}

float Agent::getLOSDistance() const
{
	return m_LOSDistance;
}

bool Agent::hasLOS() const
{
	return m_hasLOS;
}

float Agent::getCurrentHeading() const
{
	return m_currentHeading;
}

glm::vec4 Agent::getLOSColour() const
{
	return m_LOSColour;
}

glm::vec2 Agent::getLeftLOSEndPoint() const
{
	return m_leftLOSEndPoint;
}

glm::vec2 Agent::getMiddleLOSEndPoint() const
{
	return m_middleLOSEndPoint;
}

glm::vec2 Agent::getRightLOSEndPoint() const
{
	return m_rightLOSEndPoint;
}

bool* Agent::getCollisionWhiskers()
{
	return m_collisionWhiskers; // Returns entire array.
}

glm::vec4 Agent::getLineColor(int index)
{
	return m_lineColor[index];
}

float Agent::getWhiskerAngle() const
{
	return m_whiskerAngle;
}

void Agent::setTargetPosition(const glm::vec2 new_position)
{
	m_targetPosition = new_position;
}

void Agent::setCurrentDirection(const glm::vec2 new_direction)
{
	m_currentDirection = new_direction;
}

void Agent::setLOSDistance(const float distance)
{
	m_LOSDistance = distance;
}

void Agent::setHasLOS(const bool state)
{
	m_hasLOS = state;
	m_LOSColour = (m_hasLOS) ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
}

void Agent::setCurrentHeading(const float heading)
{
	m_currentHeading = heading;
	m_changeDirection();
}

void Agent::setLOSColour(const glm::vec4 colour)
{
	m_LOSColour = colour;
}

void Agent::setLeftLOSEndPoint(glm::vec2 point)
{
	m_leftLOSEndPoint = point;
}

void Agent::setMiddleLOSEndPoint(glm::vec2 point)
{
	m_middleLOSEndPoint = point;
}

void Agent::setRightLOSEndPoint(glm::vec2 point)
{
	m_rightLOSEndPoint = point;
}

void Agent::setLineColor(int index, glm::vec4 color)
{
	m_lineColor[index] = color;
}

void Agent::setWhiskerAngle(float a)
{
	m_whiskerAngle = a;
}

void Agent::updateWhiskers(float a)
{
	setWhiskerAngle(a);
	setMiddleLOSEndPoint(getTransform()->position + getCurrentDirection() * getLOSDistance());

	float x = sin((getCurrentHeading() - m_whiskerAngle + 90) * Util::Deg2Rad);
	float y = cos((getCurrentHeading() - m_whiskerAngle + 90) * Util::Deg2Rad);
	setLeftLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);
	// You can make the 0.75 a field in ImGui as the whisker length.

	x = sin((getCurrentHeading() + m_whiskerAngle + 90) * Util::Deg2Rad);
	y = cos((getCurrentHeading() + m_whiskerAngle + 90) * Util::Deg2Rad);
	setRightLOSEndPoint(getTransform()->position + glm::vec2(x, -y) * getLOSDistance() * 0.75f);
}

bool Agent::checkAgentLOSToTarget(Agent * agent, DisplayObject * target_obstacle, std::vector<Obstacle*>&obstacles)
{
	bool hasLOS = false;

	auto targetDirection = glm::vec2(target_obstacle->getTransform()->position.x + 24, target_obstacle->getTransform()->position.y + 32) - agent->getTransform()->position;
	auto normalizedDirection = Util::normalize(targetDirection);
	setMiddleLOSEndPoint(getTransform()->position + normalizedDirection * getLOSDistance());

	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_obstacle);
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto obstacle : obstacles)
		{
			if (obstacle->getType() == NONE) continue; // Added Lab 7.
			auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, obstacle);
			if (AgentToObjectDistance > AgentToTargetDistance) continue;

			contactList.push_back(obstacle);
		}
		hasLOS = CollisionManager::LOSCheck(agent, getMiddleLOSEndPoint(), contactList, target_obstacle);
	}
	agent->setHasLOS(hasLOS);
	return hasLOS;
}

bool Agent::checkAgentLOSToTarget(Agent* agent, SDL_Rect* target_object, std::vector<Obstacle*>& obstacles)
{
	bool hasLOS = false;

	auto targetDirection = glm::vec2(target_object->x + target_object->w / 2, target_object->y + target_object->h / 2) - agent->getTransform()->position;
	auto normalizedDirection = Util::normalize(targetDirection);
	setMiddleLOSEndPoint(getTransform()->position + normalizedDirection * getLOSDistance());

	// if ship to target distance is less than or equal to LOS Distance
	auto AgentToTargetDistance = Util::getClosestEdge(agent->getTransform()->position, target_object);
	if (AgentToTargetDistance <= agent->getLOSDistance())
	{
		std::vector<DisplayObject*> contactList;
		for (auto obstacle : obstacles)
		{
			if (obstacle->getType() == NONE) continue; // Added Lab 7.
			auto AgentToObjectDistance = Util::getClosestEdge(agent->getTransform()->position, obstacle);
			if (AgentToObjectDistance > AgentToTargetDistance) continue;

			contactList.push_back(obstacle);
		}
		hasLOS = CollisionManager::LOSCheck(agent, getMiddleLOSEndPoint(), contactList, target_object);
	}
	agent->setHasLOS(hasLOS);
	return hasLOS;
}

void Agent::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);
}

SpriteSheet* Agent::getSpriteSheet()
{
	return m_pSpriteSheet;
}

Animation& Agent::getAnimation(const std::string& name)
{
	return m_pAnimations[name];
}

void Agent::setSpriteSheet(SpriteSheet* sprite_sheet)
{
	m_pSpriteSheet = sprite_sheet;
}

void Agent::setAnimation(const Animation& animation)
{
	if (!m_animationExists(animation.name))
	{
		m_pAnimations[animation.name] = animation;
	}
}

bool Agent::m_animationExists(const std::string& id)
{
	return m_pAnimations.find(id) != m_pAnimations.end();
}