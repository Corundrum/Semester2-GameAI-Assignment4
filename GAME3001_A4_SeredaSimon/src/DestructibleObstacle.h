#pragma once
#ifndef __DESTRUCTIBLE_OBSTACLE__
#define __DESTRUCTIBLE_OBSTACLE__
#include "DisplayObject.h"

class DestructibleObstacle final : public DisplayObject
{
public:
	// constructors
	DestructibleObstacle();

	// destructor
	~DestructibleObstacle();


	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	int health;
	int maxHealth;


private:
	
};

#endif /* defined (__OBSTACLE__) */