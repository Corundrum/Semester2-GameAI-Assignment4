#pragma once
#ifndef __IDLE_CONDITION__
#define __IDLE_CONDITION__
#include "ConditionNode.h"

class IdleCondition : public ConditionNode
{
public:
	IdleCondition(bool wants_to_idle = false);
	virtual ~IdleCondition();

	//get and set
	void setWantsToIdle(bool state);
	virtual bool Condition() override;
private:
	bool m_wantsToIdle;
};


#endif // !__CLOSE_COMBAT_CONDITION__
