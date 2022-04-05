#pragma once
#ifndef __IDLE_ACTION__
#define __IDLE_ACTION__

#include "ActionNode.h"

class IdleAction : public ActionNode
{
public:
	IdleAction();
	virtual ~IdleAction();

	void Action() override;
private:
};

#endif // !__ATTACK_ACTION__