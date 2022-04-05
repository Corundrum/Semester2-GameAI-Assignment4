#include "MoveToRangeAction.h"
#include <iostream>

MoveToRangeAction::MoveToRangeAction()
{
	m_name = "Move To Range Action";
}

MoveToRangeAction::~MoveToRangeAction()
= default;

void MoveToRangeAction::Action()
{
	if (getAgent()->getActionState() != MOVE_TO_RANGE)
	{
		std::cout << "Performing " << m_name << std::endl;
	}
	getAgent()->moveToRange();
}
