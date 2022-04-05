#include "PatrolAction.h"
#include <iostream>

PatrolAction::PatrolAction()
{
	m_name = "Patrol Action";
}

PatrolAction::~PatrolAction()
= default;

void PatrolAction::Action()
{
	if (getAgent()->getActionState() != PATROL)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->patrol();
}
