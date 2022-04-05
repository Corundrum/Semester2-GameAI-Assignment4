#include "Bullet.h"
#include "TextureManager.h"


Bullet::Bullet()
{
	TextureManager::Instance().load("../Assets/textures/CannonBall.png", "bullet");

	setWidth(12);
	setHeight(12);

	setType(TARGET);
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

	m_move();
}

void Bullet::clean()
{
}

void Bullet::m_move()
{
	getTransform()->position = getTransform()->position + getRigidBody()->velocity * 5.0f;
}


