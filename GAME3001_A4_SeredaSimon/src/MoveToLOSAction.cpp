#include "MoveToLOSAction.h"
#include <iostream>

MoveToLOSAction::MoveToLOSAction()
{
	m_name = "Move To Line Of Sight Action";
}

MoveToLOSAction::~MoveToLOSAction()
= default;

void MoveToLOSAction::Action()
{
	if (getAgent()->getActionState() != MOVE_TO_LOS)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->moveToLOS();
}
