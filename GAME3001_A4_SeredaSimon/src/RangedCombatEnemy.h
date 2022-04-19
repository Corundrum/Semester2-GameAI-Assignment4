#pragma once
#ifndef __RANGED_COMBAT_ENEMY__
#define __RANGED_COMBAT_ENEMY__

#include "BaseEnemy.h"
#include "DecisionTree.h"

class RangedCombatEnemy final : public BaseEnemy
{
public:
	RangedCombatEnemy();
	~RangedCombatEnemy();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	void idle() override;
	void death() override;
	void takeDamage() override;
	void patrol() override;
	void moveToRange() override;
	void moveToLOS() override;
	void attack() override;
	void flee() override;

private:

	float walkTimer = 1;

	void m_buildAnimations();
	bool isFacingLeft = 0;

	void m_buildTree();

};


#endif /* defined (__RANGED_COMBAT_ENEMY__) */