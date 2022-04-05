#include "CloseCombatEnemy.h"

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

#include "Sprite.h"
#include "Player.h"

CloseCombatEnemy::CloseCombatEnemy()
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/slugSpriteData.txt",
		"../Assets/sprites/slug.png",
		"slugSpriteSheet");

	SoundManager::Instance().load("../Assets/audio/squish.wav", "squish", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/death.wav", "death", SOUND_SFX);

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("slugSpriteSheet"));
	setWidth(48);
	setHeight(48);
	
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

	hitBox = { (int)getTransform()->position.x - getWidth() / 2, (int)(getTransform()->position.y + getHeight() * 0.25), getWidth(), getHeight() / 2 };

	setLOSDistance(400.0f); // 5 ppf x 80 feet
	setLOSColour(glm::vec4(1, 0, 0, 1));

	// Fill in action state and patrol code
	setActionState(NO_ACTION);

	//set patrol
	m_patrol.push_back(glm::vec2(760, 50)); 
	m_patrol.push_back(glm::vec2(760, 150));
	m_patrol.push_back(glm::vec2(300, 150)); 
	m_patrol.push_back(glm::vec2(300, 550));
	m_patrol.push_back(glm::vec2(600, 450));
	m_patrol.push_back(glm::vec2(300, 550));
	m_patrol.push_back(glm::vec2(300, 150));
	m_patrol.push_back(glm::vec2(760, 150));
	m_waypoint = 0;

	setTargetPosition(m_patrol[m_waypoint]);
	setType(AGENT);
	
	//create decision tree
	m_tree = new DecisionTree(this); // overloaded constructor
	m_buildTree();
	m_tree->display();

	m_buildAnimations();
}

CloseCombatEnemy::~CloseCombatEnemy() 
= default;

void CloseCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	
	switch (getActionState())
	{
	case DEATH:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("die"),
				x, y, 0.08f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("die"),
				x, y, 0.08f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case TAKE_DAMAGE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("take_damage"),
				x, y, 0.08f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("take_damage"),
				x, y, 0.08f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		if (getAnimation("take_damage").current_frame == 2)
		{
			getAnimation("take_damage").current_frame = 0;
			m_tree->getTakeDamageNode()->setTakingDamage(false);
		}
		break;
	case IDLE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("idle"),
				x, y, 0.02f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("idle"),
				x, y, 0.02f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case MOVE_TO_PLAYER:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case PATROL:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("patrol"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugSpriteSheet", getAnimation("patrol"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	default:
		break;
	}

	glm::vec4 blue = glm::vec4(0, 0, 1, 1);
	glm::vec4 green = glm::vec4(0, 1, 0, 1);
	glm::vec4 circleColour;

	// draw LOS
	if (isWithinRadius)
	{
		circleColour = green;
	}
	else
	{
		circleColour = blue;
	}
	if (debug)
	{
		Util::DrawCircle(getTransform()->position, 250, circleColour);
		Util::DrawLine(getTransform()->position, getMiddleLOSEndPoint(), getLOSColour());
		Util::DrawRect(glm::vec2(hitBox.x, hitBox.y), hitBox.w, hitBox.h, glm::vec4(1, 0, 0, 1));
	}


	drawHealthBar();
}

void CloseCombatEnemy::update()
{
	if (getActionState() != DEATH)
	{
		hitBox = { (int)getTransform()->position.x - 14, (int)getTransform()->position.y + 8, 44, 24 };

		if (EventManager::Instance().keyPressed(SDL_SCANCODE_K))
		{
			setHealth(getHealth() - 1);
		}
		if (EventManager::Instance().keyPressed(SDL_SCANCODE_P))
		{
			if (getActionState() != IDLE)
			{
				m_tree->getIdleNode()->setWantsToIdle(1);
			}
			else
			{
				m_tree->getIdleNode()->setWantsToIdle(0);
			}
		}

		if (getRigidBody()->velocity.x > 0)
		{
			if (!isFacingLeft)
			{
				isFacingLeft = true;
			}
		}
		else if (getRigidBody()->velocity.x < 0)
		{
			if (isFacingLeft)
			{
				isFacingLeft = false;
			}
		}

		if (Util::distance(getTransform()->position, glm::vec2(Player::s_pPlayerObj->getTransform()->position.x + 24, Player::s_pPlayerObj->getTransform()->position.y + 24)) < 250)
		{
			if (!isWithinRadius)
			{
				isWithinRadius = true;
			}
		}
		else
		{
			if (isWithinRadius)
			{
				isWithinRadius = false;
			}
		}

		if (getHealth() <= 0)
		{
			m_tree->getDeathNode()->setIsDead(true);
		}

		if (walkTimer >= 1.5 && (getActionState() == PATROL || getActionState() == MOVE_TO_PLAYER))
		{
			walkTimer = 0;
			SoundManager::Instance().playSound("squish");
		}

		// Determine which action to perform
		m_tree->makeDecision();
	}
	else
	{
		getRigidBody()->acceleration = glm::vec2(0, 0);
		getRigidBody()->velocity = glm::vec2(0, 0);
	}



	//Timers
	const float delta_time = TheGame::Instance().getDeltaTime();

	if (walkTimer < 1.5)
	{
		walkTimer += delta_time;
	}
}

void CloseCombatEnemy::clean()
{

}

void CloseCombatEnemy::idle()
{
	if (getActionState() != IDLE)
	{
		//initialize the action
		setActionState(IDLE);
	}
}

void CloseCombatEnemy::death()
{
	if (getActionState() != DEATH)
	{
		//initialize the action
		setActionState(DEATH);
		SoundManager::Instance().playSound("death");
		Player::s_pPlayerObj->addScore(10);
	}
}

void CloseCombatEnemy::takeDamage()
{
	if (getActionState() != TAKE_DAMAGE)
	{
		m_tree->getTakeDamageNode()->setTakingDamage(true);
		//initialize the action
		setHealth(getHealth() - 2);
		setActionState(TAKE_DAMAGE);
	}
}

void CloseCombatEnemy::patrol()
{
	if (getActionState() != PATROL)
	{
		setTargetPosition(m_patrol[m_waypoint]);
		//initialize the action
		setActionState(PATROL);
	}
	m_move();
}

void CloseCombatEnemy::moveToPlayer()
{
	if (getActionState() != MOVE_TO_PLAYER)
	{
		//initialize the action
		setActionState(MOVE_TO_PLAYER);
	}
	setTargetPosition(Player::s_pPlayerObj->getTransform()->position);
	m_move();
}

void CloseCombatEnemy::m_buildAnimations()
{
	Animation moveToPlayerAnimation = Animation();
	moveToPlayerAnimation.name = "MoveToPlayer";
	moveToPlayerAnimation.frames.push_back(getSpriteSheet()->getFrame("MoveToPlayer-0"));
	moveToPlayerAnimation.frames.push_back(getSpriteSheet()->getFrame("MoveToPlayer-1"));
	setAnimation(moveToPlayerAnimation);

	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-1"));
	setAnimation(idleAnimation);

	Animation patrolAnimation = Animation();
	patrolAnimation.name = "patrol";
	patrolAnimation.frames.push_back(getSpriteSheet()->getFrame("Patrol-0"));
	patrolAnimation.frames.push_back(getSpriteSheet()->getFrame("Patrol-1"));
	setAnimation(patrolAnimation);

	Animation AttackAnimation = Animation();
	AttackAnimation.name = "attack";
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-0"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-1"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-2"));
	setAnimation(AttackAnimation);

	Animation TakeDamageAnimation = Animation();
	TakeDamageAnimation.name = "take_damage";
	TakeDamageAnimation.frames.push_back(getSpriteSheet()->getFrame("TakeDmg-0"));
	TakeDamageAnimation.frames.push_back(getSpriteSheet()->getFrame("TakeDmg-1"));
	TakeDamageAnimation.frames.push_back(getSpriteSheet()->getFrame("TakeDmg-2"));
	setAnimation(TakeDamageAnimation);

	Animation DieAnimation = Animation();
	DieAnimation.name = "die";
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-0"));
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-1"));
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-2"));
	setAnimation(DieAnimation);
}
