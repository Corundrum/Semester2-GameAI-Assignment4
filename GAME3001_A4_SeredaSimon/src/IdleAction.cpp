#include "IdleAction.h"
#include <iostream>

IdleAction::IdleAction()
{
	m_name = "Idle Action";
}

IdleAction::~IdleAction()
= default;

void IdleAction::Action()
{
	if (getAgent()->getActionState() != IDLE)
	{
		std::cout << "Performing Idle" << m_name << std::endl;
	}
	getAgent()->idle();
}
