#include "RangedCombatEnemy.h"

#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Util.h"

#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "PatrolAction.h"
#include "IdleAction.h"
#include "DeathAction.h"
#include "TakeDamageAction.h"
#include "FleeAction.h"

#include "Sprite.h"
#include "Player.h"

RangedCombatEnemy::RangedCombatEnemy()
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/slugSpriteData.txt",
		"../Assets/sprites/slugR.png",
		"slugRSpriteSheet");

	SoundManager::Instance().load("../Assets/audio/squish.wav", "squish", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/death.wav", "death", SOUND_SFX);

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("slugRSpriteSheet"));
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

	m_tree->getHealthyNode()->setHealth(true);

	m_buildAnimations();
}

RangedCombatEnemy::~RangedCombatEnemy() 
= default;

void RangedCombatEnemy::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	switch (getActionState())
	{
	case DEATH:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("die"),
				x, y, 0.08f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("die"),
				x, y, 0.08f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case TAKE_DAMAGE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("take_damage"),
				x, y, 0.08f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("take_damage"),
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
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("idle"),
				x, y, 0.02f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("idle"),
				x, y, 0.02f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case MOVE_TO_RANGE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case MOVE_TO_LOS:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("MoveToPlayer"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case PATROL:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("patrol"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("patrol"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case FLEE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("patrol"),
				x, y, 0.34f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("patrol"),
				x, y, 0.34f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case ATTACK:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("attack"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("slugRSpriteSheet", getAnimation("attack"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		if (getAnimation("attack").current_frame == 5)
		{
			Bullet::s_pBullets.push_back(new Bullet(true));
			Bullet::s_pBullets.back()->getTransform()->position = glm::vec2(hitBox.x + hitBox.w * 0.5, hitBox.y + hitBox.h * 0.5);
			Bullet::s_pBullets.back()->getRigidBody()->velocity = (Util::normalize(glm::vec2(Player::s_pPlayerObj->getTransform()->position.x, Player::s_pPlayerObj->getTransform()->position.y + 32) - glm::vec2(hitBox.x + hitBox.w * 0.5, hitBox.y + hitBox.h * 0.5)));


			getAnimation("attack").current_frame = 0;
			setActionState(NO_ACTION);
		}
		break;
	default:
		break;
	}
	glm::vec4 blue = glm::vec4(0, 0, 1, 1);
	glm::vec4 green = glm::vec4(0, 1, 0, 1);
	glm::vec4 circleColour;

	// draw LOS
	if (m_tree->getRadiusNode()->getIsWithinRadius())
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

void RangedCombatEnemy::update()
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

		//if within distance
		if (Util::distance(getTransform()->position, glm::vec2(Player::s_pPlayerObj->getTransform()->position.x + 12, Player::s_pPlayerObj->getTransform()->position.y + 28)) < 250)
		{
			if (!m_tree->getRadiusNode()->getIsWithinRadius())
			{
				m_tree->getRadiusNode()->setIsWithinRadius(true);
			}
		}
		else
		{
			if (m_tree->getRadiusNode()->getIsWithinRadius())
			{
				m_tree->getRadiusNode()->setIsWithinRadius(false);
			}
		}

		//if within combat distance
		if (Util::distance(getTransform()->position, glm::vec2(Player::s_pPlayerObj->getTransform()->position.x + 12, Player::s_pPlayerObj->getTransform()->position.y + 28)) <= 200 &&
			Util::distance(getTransform()->position, glm::vec2(Player::s_pPlayerObj->getTransform()->position.x + 12, Player::s_pPlayerObj->getTransform()->position.y + 28)) >= 130)
		{
			if (!m_tree->getCloseCombatNode()->getWithinCombatRange())
			{
				m_tree->getCloseCombatNode()->setWithinCombatRange(true);
			}
		}
		else
		{
			if (m_tree->getCloseCombatNode()->getWithinCombatRange() && getActionState() != ATTACK)
			{
				m_tree->getCloseCombatNode()->setWithinCombatRange(false);
			}
		}

		if (m_health < 3)
		{
			m_tree->getHealthyNode()->setHealth(false);
		}

		//set dead
		if (getHealth() <= 0)
		{
			m_tree->getDeathNode()->setIsDead(true);
		}

		//if moving, use timer for sound
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

void RangedCombatEnemy::clean()
{
}

void RangedCombatEnemy::idle()
{
	if (getActionState() != IDLE)
	{
		//initialize the action
		setActionState(IDLE);
	}
}

void RangedCombatEnemy::death()
{
	if (getActionState() != DEATH)
	{
		//initialize the action
		setActionState(DEATH);
		SoundManager::Instance().playSound("death");
		Player::s_pPlayerObj->addScore(10);
	}
}

void RangedCombatEnemy::takeDamage()
{
	if (getActionState() != TAKE_DAMAGE)
	{
		m_tree->getTakeDamageNode()->setTakingDamage(true);
		//initialize the action
		setHealth(getHealth() - 2);
		setActionState(TAKE_DAMAGE);
	}
}

void RangedCombatEnemy::patrol()
{
	if (getActionState() != PATROL)
	{
		setTargetPosition(m_patrol[m_waypoint]);
		//initialize the action
		setActionState(PATROL);
	}
	m_move(false);
}

void RangedCombatEnemy::moveToRange()
{
	if (getActionState() != MOVE_TO_RANGE)
	{
		//initialize the action
		setActionState(MOVE_TO_RANGE);
	}
	setTargetPosition(Player::s_pPlayerObj->getTransform()->position);
	if (Util::distance(this->getTransform()->position, Player::s_pPlayerObj->getTransform()->position) > 200)
	{
		m_move(false);
	}
	else if (Util::distance(this->getTransform()->position, Player::s_pPlayerObj->getTransform()->position) < 160)
	{
		m_move(true);
	}
}

void RangedCombatEnemy::moveToLOS()
{
	if (getActionState() != MOVE_TO_LOS)
	{
		//initialize the action
		setActionState(MOVE_TO_LOS);
	}
	setTargetPosition(findNextNode()->getTransform()->position);
	m_move(false);
}

void RangedCombatEnemy::attack()
{
	if (getActionState() != ATTACK)
	{
		//initialize the action
		setActionState(ATTACK);
	}
}

void RangedCombatEnemy::flee()
{
	if (getActionState() != FLEE)
	{
		//initialize the action
		setActionState(FLEE);
	}
	setTargetPosition(Player::s_pPlayerObj->getTransform()->position);
	m_move(true);
}

void RangedCombatEnemy::m_buildAnimations()
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
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-0"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-1"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-2"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-0"));
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

void RangedCombatEnemy::m_buildTree()
{
	// Create and add root node.
	m_tree->setDeathNode(new DeathCondition());
	m_tree->getTree().push_back(m_tree->getDeathNode());


	TreeNode* deathNode = m_tree->addNode(m_tree->getDeathNode(), new DeathAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(deathNode)->setAgent(this);
	m_tree->getTree().push_back(deathNode);


	m_tree->setHealthyNode(new EnemyHealthCondition());
	m_tree->addNode(m_tree->getDeathNode(), m_tree->getHealthyNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getHealthyNode());

	TreeNode* fleeNode = m_tree->addNode(m_tree->getHealthyNode(), new FleeAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(fleeNode)->setAgent(this);
	m_tree->getTree().push_back(fleeNode);

	m_tree->setTakeDamageNode(new TakeDamageCondition());
	m_tree->addNode(m_tree->getHealthyNode(), m_tree->getTakeDamageNode(), RIGHT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getTakeDamageNode());


	TreeNode* takeDamageNode = m_tree->addNode(m_tree->getTakeDamageNode(), new TakeDamageAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(takeDamageNode)->setAgent(this);
	m_tree->getTree().push_back(takeDamageNode);

	m_tree->setIdleNode(new IdleCondition());
	m_tree->addNode(m_tree->getTakeDamageNode(), m_tree->getIdleNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getIdleNode());


	TreeNode* idleNode = m_tree->addNode(m_tree->getIdleNode(), new IdleAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(idleNode)->setAgent(this);
	m_tree->getTree().push_back(idleNode);

	m_tree->setLOSNode(new LOSCondition());
	m_tree->addNode(m_tree->getIdleNode(), m_tree->getLOSNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getLOSNode());


	m_tree->setRadiusNode(new RadiusCondition());
	m_tree->addNode(m_tree->getLOSNode(), m_tree->getRadiusNode(), LEFT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getRadiusNode());

	TreeNode* patrolNode = m_tree->addNode(m_tree->getRadiusNode(), new PatrolAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(patrolNode)->setAgent(this);
	m_tree->getTree().push_back(patrolNode);

	TreeNode* moveToLOSNode = m_tree->addNode(m_tree->getRadiusNode(), new MoveToLOSAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToLOSNode)->setAgent(this);
	m_tree->getTree().push_back(moveToLOSNode);


	m_tree->setCloseCombatNode(new CloseCombatCondition());
	m_tree->addNode(m_tree->getLOSNode(), m_tree->getCloseCombatNode(), RIGHT_TREE_NODE);
	m_tree->getTree().push_back(m_tree->getCloseCombatNode());

	TreeNode* moveToRangeNode = m_tree->addNode(m_tree->getCloseCombatNode(), new MoveToRangeAction(), LEFT_TREE_NODE);
	dynamic_cast<ActionNode*>(moveToRangeNode)->setAgent(this);
	m_tree->getTree().push_back(moveToRangeNode);

	TreeNode* attackNode = m_tree->addNode(m_tree->getCloseCombatNode(), new AttackAction(), RIGHT_TREE_NODE);
	dynamic_cast<ActionNode*>(attackNode)->setAgent(this);
	m_tree->getTree().push_back(attackNode);
}
