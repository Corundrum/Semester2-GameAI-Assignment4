#pragma once
#ifndef __TAKE_DAMAGE_CONDITION__
#define __TAKE_DAMAGE_CONDITION__
#include "ConditionNode.h"

class TakeDamageCondition : public ConditionNode
{
public:
	TakeDamageCondition(bool is_taking_damage = false);
	virtual ~TakeDamageCondition();

	//get and set
	void setTakingDamage(bool state);
	virtual bool Condition() override;
private:
	bool m_isTakingDamage;
};


#endif // !__CLOSE_COMBAT_CONDITION__
