#pragma once
#ifndef __TAKE_DAMAGE_ACTION__
#define __TAKE_DAMAGE_ACTION__

#include "ActionNode.h"

class TakeDamageAction : public ActionNode
{
public:
	TakeDamageAction();
	virtual ~TakeDamageAction();

	void Action() override;
private:
};

#endif // !__ATTACK_ACTION__