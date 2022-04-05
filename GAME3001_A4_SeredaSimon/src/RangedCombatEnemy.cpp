#include "RangedCombatEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Util.h"

#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "PatrolAction.h"

RangedCombatEnemy::RangedCombatEnemy()
{
	TextureManager::Instance().load("../Assets/textures/bullet.png", "ranged_enemy");

	const auto size = TextureManager::Instance().getTextureSize("ranged_enemy");
	setWidth(size.x);
	setHeight(size.y);
	
	getRigidBody()->bounds = glm::vec2(getWidth(), getHeight());
	getRigidBody()->velocity = glm::vec2(0, 0);
	getRigidBody()->acceleration = glm::vec2(0, 0);
	getRigidBody()->isColliding = false;

	setMaxHealth(10);
	setHealth(getMaxHealth());

	// starting motion properties
	setCurrentHeading(0.0f);// current facing angle
	setCurrentDirection(glm::vec2(1.0f, 0.0f)); // facing right
	m_maxSpeed = 10.0f; // a maximum number of pixels moved per frame
	m_turnRate = 5.0f; // a maximum number of degrees to turn each time-step
	m_accelerationRate = 2.0f; // a maximum number of pixels to add to the velocity each frame
	
	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));
}

RangedCombatEnemy::~RangedCombatEnemy() 
= default;

void RangedCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance().draw("ranged_enemy", x, y, getCurrentHeading(), 255, isCentered());

	drawHealthBar();

	// draw LOS
	if (debug)
	{
		Util::DrawLine(getTransform()->position, getMiddleLOSEndPoint(), getLOSColour());
	}
}

void RangedCombatEnemy::update()
{
	// Determine which action to perform
	m_tree->makeDecision();
}

void RangedCombatEnemy::clean()
{
}