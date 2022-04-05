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
	

private:

};


#endif /* defined (__RANGED_COMBAT_ENEMY__) */