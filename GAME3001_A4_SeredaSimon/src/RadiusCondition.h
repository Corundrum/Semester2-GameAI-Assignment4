#pragma once
#ifndef __RADIUS_CONDITION__
#define __RADIUS_CONDITION__
#include "ConditionNode.h"

class RadiusCondition : public ConditionNode
{
public:
	RadiusCondition(bool withinRadius = false);
	virtual ~RadiusCondition();

	//get and set
	void setIsWithinRadius(bool state);

	virtual bool Condition() override;
private:
	bool m_isWithinRadius;
};

#endif // !__RADIUS_CONDITION__