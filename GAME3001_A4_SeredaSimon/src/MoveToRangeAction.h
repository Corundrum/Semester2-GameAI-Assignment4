#pragma once
#ifndef __MOVE_TO_RANGE_ACTION__
#define __MOVE_TO_RANGE_ACTION__

#include "ActionNode.h"

class MoveToRangeAction : public ActionNode
{
public:
	MoveToRangeAction();
	virtual ~MoveToRangeAction();

	void Action() override;
private:
};

#endif // !__PMOVE_TO_PLAYER_ACTION__