#pragma once
#ifndef __MOVE_TO_PLAYER_ACTION__
#define __MOVE_TO_PLAYER_ACTION__

#include "ActionNode.h"

class MoveToPlayerAction : public ActionNode
{
public:
	MoveToPlayerAction();
	virtual ~MoveToPlayerAction();

	void Action() override;
private:
};

#endif // !__PMOVE_TO_PLAYER_ACTION__