#include "DeathAction.h"
#include <iostream>

DeathAction::DeathAction()
{
	m_name = "death Action";
}

DeathAction::~DeathAction()
= default;

void DeathAction::Action()
{
	if (getAgent()->getActionState() != DEATH)
	{
		std::cout << "Performing death" << m_name << std::endl;
	}
	getAgent()->death();
}
