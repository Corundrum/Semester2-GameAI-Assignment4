#include "Obstacle.h"


#include "SoundManager.h"
#include "TextureManager.h"

Obstacle::Obstacle()
{
	TextureManager::Instance().load("../Assets/textures/tree.png", "obstacle");

	setWidth(88);
	setHeight(128);

	setType(OBSTACLE);

	getRigidBody()->isColliding = false;
}

Obstacle::~Obstacle()
= default;

void Obstacle::draw()
{
	TextureManager::Instance().draw("obstacle", getTransform()->position.x, getTransform()->position.y, this, 0, 255, true);
}

void Obstacle::update()
{
}

void Obstacle::clean()
{
}
