#include "DestructibleObstacle.h"

#include "SoundManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Util.h"

#include "Bullet.h"

DestructibleObstacle::DestructibleObstacle()
{
	TextureManager::Instance().load("../Assets/textures/chest.png", "chest");

	setWidth(42);
	setHeight(42);

	setType(OBSTACLE);

	getRigidBody()->isColliding = false;

	maxHealth = 10;
	health = maxHealth;
}

DestructibleObstacle::~DestructibleObstacle()
= default;

void DestructibleObstacle::draw()
{
	float health_percent = (float)health / maxHealth;

	Util::DrawFilledRect(glm::vec2(getTransform()->position.x - getWidth() / 2 - 2, getTransform()->position.y - 32), 48, 8, glm::vec4(0, 0, 0, 1));
	if (health > 0)
	{
		Util::DrawFilledRect(glm::vec2(getTransform()->position.x - getWidth() / 2 - 2, getTransform()->position.y - 32), 48 * health_percent, 8, glm::vec4(1, 0.6, 0.2, 1));
	}

	Util::DrawRect(glm::vec2(getTransform()->position.x - getWidth() / 2, getTransform()->position.y - getHeight() / 2), 42, 42, glm::vec4(1, 0, 0, 1));

	TextureManager::Instance().draw("chest", getTransform()->position.x, getTransform()->position.y, this, 0, 255, true);
}

void DestructibleObstacle::update()
{
	for (unsigned i = 0; i < Bullet::s_pBullets.size(); i++)
	{
		if (CollisionManager::AABBCheck(Bullet::s_pBullets[i], this))
		{
			health -= 2;
			Bullet::s_pBullets[i] = nullptr;
			Bullet::s_pBullets.erase(Bullet::s_pBullets.begin() + i);
			Bullet::s_pBullets.shrink_to_fit();
			break;
		}
	}
}

void DestructibleObstacle::clean()
{
}
