#include "TakeDamageAction.h"
#include <iostream>

TakeDamageAction::TakeDamageAction()
{
	m_name = "Take Damage Action";
}

TakeDamageAction::~TakeDamageAction()
= default;

void TakeDamageAction::Action()
{
	if (getAgent()->getActionState() != TAKE_DAMAGE)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->takeDamage();
}
