#include "Bullet.h"
#include "BaseEnemy.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Player.h"


Bullet::Bullet(bool dam)
{
	TextureManager::Instance().load("../Assets/textures/CannonBall.png", "bullet");

	setWidth(12);
	setHeight(12);

	setType(TARGET);
	damagePlayer = dam;
}

Bullet::~Bullet()
= default;

void Bullet::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the target
	TextureManager::Instance().draw("bullet", x, y, this, 0, 255, false);
}

void Bullet::update()
{
}

void Bullet::updateField()
{
	for (unsigned i = 0; i < s_pBullets.size(); i++)
	{
		s_pBullets[i]->m_move();
		if (!s_pBullets[i]->damagePlayer)
		{
			for (auto enemy : BaseEnemy::s_EnemiesObj)
			{
				if (CollisionManager::AABBCheck(s_pBullets[i], &enemy->getHitBox()))
				{
					enemy->takeDamage();
					s_pBullets[i] = nullptr;
					s_pBullets.erase(s_pBullets.begin() + i);
					s_pBullets.shrink_to_fit();
					break;
				}
			}
		}
		else
		{
			if (CollisionManager::AABBCheck(s_pBullets[i], &Player::s_pPlayerObj->getHitBox()))
			{
				Player::s_pPlayerObj->takeDamage(20);
				s_pBullets[i] = nullptr;
				s_pBullets.erase(s_pBullets.begin() + i);
				s_pBullets.shrink_to_fit();
				break;
			}
		}
	}
}

void Bullet::clean()
{
}

void Bullet::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}

std::vector<Bullet*> Bullet::s_pBullets;
