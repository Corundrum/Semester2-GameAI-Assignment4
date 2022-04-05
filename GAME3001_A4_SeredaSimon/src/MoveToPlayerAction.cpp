#include "MoveToPlayerAction.h"
#include <iostream>

MoveToPlayerAction::MoveToPlayerAction()
{
	m_name = "Move To Player Action";
}

MoveToPlayerAction::~MoveToPlayerAction()
= default;

void MoveToPlayerAction::Action()
{
	if (getAgent()->getActionState() != MOVE_TO_PLAYER)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->moveToPlayer();
}
