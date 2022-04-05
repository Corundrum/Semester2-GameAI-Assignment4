#pragma once
#ifndef __CLOSE_COMBAT_ENEMY__
#define __CLOSE_COMBAT_ENEMY__

#include "BaseEnemy.h"
#include "DecisionTree.h"
#include "Sprite.h"

class CloseCombatEnemy final : public BaseEnemy
{
public:
	CloseCombatEnemy();
	~CloseCombatEnemy();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	//new action functions
	void idle() override;
	void death() override;
	void takeDamage() override;
	void patrol() override;
	void moveToPlayer() override;
private:
	bool isWithinRadius = 0;

	float walkTimer = 1;

	// private movement variables
	void m_buildAnimations();
	bool isFacingLeft = 0;
};


#endif /* defined (__CLOSE_COMBAT_ENEMY__) */