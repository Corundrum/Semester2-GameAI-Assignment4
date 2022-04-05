#pragma once
#ifndef __LOS_CONDITION__
#define __LOS_CONDITION__
#include "ConditionNode.h"

class LOSCondition : public ConditionNode
{
public:
	LOSCondition(bool LOS = false);
	virtual ~LOSCondition();

	//get and set
	void setLOS(bool state);
	
	virtual bool Condition() override;
private:
	bool m_hasLOS;
};

#endif // !__LOS_CONDITION__
