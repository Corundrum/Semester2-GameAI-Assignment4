#include "AttackAction.h"
#include <iostream>

AttackAction::AttackAction()
{
	m_name = "Attack Action";
}

AttackAction::~AttackAction()
= default;

void AttackAction::Action()
{
	if (getAgent()->getActionState() != ATTACK)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->attack();
}
