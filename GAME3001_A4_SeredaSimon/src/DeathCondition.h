#pragma once
#ifndef __DEATH_CONDITION__
#define __DEATH_CONDITION__
#include "ConditionNode.h"

class DeathCondition : public ConditionNode
{
public:
	DeathCondition(bool isDead = false);
	virtual ~DeathCondition();

	//get and set
	void setIsDead(bool state);
	virtual bool Condition() override;
private:
	bool m_isDead;
};


#endif // !__DEATH_CONDITION__
