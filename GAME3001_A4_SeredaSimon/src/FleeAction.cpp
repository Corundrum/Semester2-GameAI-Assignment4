#include "FleeAction.h"
#include <iostream>

FleeAction::FleeAction()
{
	m_name = "Flee Action";
}

FleeAction::~FleeAction()
= default;

void FleeAction::Action()
{
	if (getAgent()->getActionState() != FLEE)
		std::cout << "Performing " << m_name << std::endl;
	getAgent()->flee();
}
