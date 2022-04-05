#pragma once
#ifndef __MOVE_TO_LOS_ACTION__
#define __MOVE_TO_LOS_ACTION__

#include "ActionNode.h"

class MoveToLOSAction : public ActionNode
{
public:
	MoveToLOSAction();
	virtual ~MoveToLOSAction();

	void Action() override;
private:
};

#endif // !__PMOVE_TO_LOS_ACTION__