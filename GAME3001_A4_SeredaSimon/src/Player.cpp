#include "Player.h"
#include "TextureManager.h"
#include "EventManager.h"
#include "Util.h"
#include "BaseEnemy.h"
#include "SoundManager.h"
#include "CollisionManager.h"
#include "Game.h"

Player::Player(): m_currentAnimationState(PLAYER_IDLE)
{
	TextureManager::Instance().loadSpriteSheet(
		"../Assets/sprites/PlayerSpriteData.txt",
		"../Assets/sprites/player.png", 
		"playerSpriteSheet");

	SoundManager::Instance().load("../Assets/audio/Explosion.wav", "explosion", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/swordSwing.wav", "sword_swing", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Splat.wav", "splat", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/Footstep.wav", "footstep", SOUND_SFX);

	setSpriteSheet(TextureManager::Instance().getSpriteSheet("playerSpriteSheet"));
	
	// set size
	setWidth(96);
	setHeight(96);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	hitBox = { (int)getTransform()->position.x - getWidth() / 2 + 5, (int)getTransform()->position.y, getWidth() / 2 - 10, getHeight() / 2 };

	m_buildAnimations();
}

Player::~Player()
= default;

void Player::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the player according to animation state
	switch(m_currentAnimationState)
	{
	case PLAYER_IDLE:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("idle"),
				x, y, 0.12f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("idle"),
				x, y, 0.12f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case PLAYER_RUN:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("run"),
				x, y, 0.52f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("run"),
				x, y, 0.52f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		break;
	case PLAYER_COMBAT:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("attack"),
				x, y, 0.3f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("attack"),
				x, y, 0.3f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		if (getAnimation("attack").current_frame == 3)
		{
			getAnimation("attack").current_frame = 0;
			m_currentAnimationState = PLAYER_IDLE;
		}
		break;
	case PLAYER_SHOOT:
		if (!isFacingLeft)
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("shoot"),
				x, y, 0.3f, 0, 255, this, true);
		}
		else
		{
			TextureManager::Instance().playAnimation("playerSpriteSheet", getAnimation("shoot"),
				x, y, 0.3f, 0, 255, this, true, SDL_FLIP_HORIZONTAL);
		}
		if (getAnimation("shoot").current_frame == 4)
		{
			SoundManager::Instance().playSound("explosion");

			m_pBullets.push_back(new Bullet());
			m_pBullets.back()->getTransform()->position = glm::vec2(hitBox.x + hitBox.w * 0.5, hitBox.y + hitBox.h * 0.5);
			m_pBullets.back()->getRigidBody()->velocity = (Util::normalize(EventManager::Instance().getMousePosition() - glm::vec2(hitBox.x + hitBox.w * 0.5, hitBox.y + hitBox.h * 0.5))) * 3.0f;

			getAnimation("shoot").current_frame = 0;
			m_currentAnimationState = PLAYER_IDLE;
		}
	default:
		break;
	}
	
	for (auto bullet : m_pBullets)
	{
		bullet->draw();
	}

	//Util::DrawRect(glm::vec2(hitBox.x, hitBox.y), hitBox.w, hitBox.h, glm::vec4(1, 0, 0, 1));

}

void Player::update()
{
	for (auto bullet : m_pBullets)
	{
		bullet->update();
	}

	// checks
	hitBox = { (int)getTransform()->position.x + 5, (int)getTransform()->position.y + 20, getWidth() / 2 - 10, getHeight() / 2 - 5};

	for (unsigned i = 0; i < m_pBullets.size(); i++)
	{
		for (auto enemy : BaseEnemy::s_EnemiesObj)
		{
			if (CollisionManager::AABBCheck(m_pBullets[i], &enemy->getHitBox()))
			{
				enemy->takeDamage();
				m_pBullets[i] = nullptr;
				m_pBullets.erase(m_pBullets.begin() + i);
				m_pBullets.shrink_to_fit();
				break;
			}
		}
	}
	//controls
	if (m_currentAnimationState != PLAYER_COMBAT && m_currentAnimationState != PLAYER_SHOOT)
	{
		
		setAnimationState(PLAYER_IDLE);
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
		{
			setAnimationState(PLAYER_RUN);
			getTransform()->position.y -= 2.5;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_S))
		{
			setAnimationState(PLAYER_RUN);
			getTransform()->position.y += 2.5;
		}
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			if (!isFacingLeft)
			{
				isFacingLeft = true;
			}
			setAnimationState(PLAYER_RUN);
			getTransform()->position.x -= 2.5;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			if (isFacingLeft)
			{
				isFacingLeft = false;
			}
			setAnimationState(PLAYER_RUN);
			getTransform()->position.x += 2.5;
		}

		if (walkTimer >= 1 && m_currentAnimationState == PLAYER_RUN)
		{
			walkTimer = 0.7;
			SoundManager::Instance().playSound("footstep");
		}

		Attack();
		Shoot();

	}


	// timers
	const float delta_time = TheGame::Instance().getDeltaTime();

	if (walkTimer < 1)
	{
		walkTimer += delta_time;
	}

}

void Player::clean()
{
}

void Player::Attack()
{
	if (EventManager::Instance().getMouseButton(LEFT))
	{
		if (!mouseLeft)
		{
			if (EventManager::Instance().getMousePosition().x > getTransform()->position.x)
			{
				isFacingLeft = false;
			}
			else
			{
				isFacingLeft = true;
			}
			SwordSlash();
			mouseLeft = true;
			m_currentAnimationState = PLAYER_COMBAT;
		}
	}
	else
	{
		if (mouseLeft)
		{
			mouseLeft = false;
		}
	}
}

void Player::SwordSlash()
{
	int dir;
	if (!isFacingLeft)
	{
		dir = 0;
	}
	else
	{
		dir = -29;
	}

	SDL_Rect SwordHitBox = { hitBox.x + dir, hitBox.y - 6, 64, 60 };
	SoundManager::Instance().playSound("sword_swing");

	for (auto enemy : BaseEnemy::s_EnemiesObj)
	{
		if (SDL_HasIntersection(&SwordHitBox, &enemy->getHitBox()))
		{
			enemy->takeDamage();
			SoundManager::Instance().playSound("splat");
		}
	}


}

void Player::Shoot()
{
	if (EventManager::Instance().getMouseButton(RIGHT))
	{
		if (!mouseRight)
		{
			if (EventManager::Instance().getMousePosition().x > getTransform()->position.x)
			{
				isFacingLeft = false;
			}
			else
			{
				isFacingLeft = true;
			}

			mouseRight = true;
			m_currentAnimationState = PLAYER_SHOOT;
		}
	}
	else
	{
		if (mouseRight)
		{
			mouseRight = false;
		}
	}
}

void Player::addScore(int score)
{
	m_score += score;
}

void Player::setAnimationState(const PlayerAnimationState new_state)
{
	m_currentAnimationState = new_state;
}

void Player::m_buildAnimations()
{
	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-0"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-1"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-2"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-3"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-4"));
	idleAnimation.frames.push_back(getSpriteSheet()->getFrame("Idle-5"));
	setAnimation(idleAnimation);

	Animation runAnimation = Animation();
	runAnimation.name = "run";
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-0"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-1"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-2"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-3"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-4"));
	runAnimation.frames.push_back(getSpriteSheet()->getFrame("Run-5"));
	setAnimation(runAnimation);

	Animation AttackAnimation = Animation();
	AttackAnimation.name = "attack";
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-0"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-1"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-2"));
	AttackAnimation.frames.push_back(getSpriteSheet()->getFrame("Atk-3"));
	setAnimation(AttackAnimation);

	Animation shootAnimation = Animation();
	shootAnimation.name = "shoot";
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-0"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-1"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-2"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-3"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-4"));
	shootAnimation.frames.push_back(getSpriteSheet()->getFrame("Shoot-5"));
	setAnimation(shootAnimation);

	Animation DieAnimation = Animation();
	DieAnimation.name = "die";
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-0"));
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-1"));
	DieAnimation.frames.push_back(getSpriteSheet()->getFrame("Die-2"));
	setAnimation(DieAnimation);
}
