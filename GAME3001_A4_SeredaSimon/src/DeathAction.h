#pragma once
#ifndef __DEATH_ACTION__
#define __DEATH_ACTION__

#include "ActionNode.h"

class DeathAction : public ActionNode
{
public:
	DeathAction();
	virtual ~DeathAction();

	void Action() override;
private:
};

#endif // !__ATTACK_ACTION__#pragma once
