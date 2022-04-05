#include "IdleCondition.h"

IdleCondition::IdleCondition(const bool wants_to_idle)
{
	m_name = "idle Condition";
	setWantsToIdle(wants_to_idle);
}

IdleCondition::~IdleCondition()
= default;

void IdleCondition::setWantsToIdle(const bool state)
{
	m_wantsToIdle = state;
}

bool IdleCondition::Condition()
{
	return m_wantsToIdle;
}