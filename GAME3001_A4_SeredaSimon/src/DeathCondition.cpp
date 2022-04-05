#include "DeathCondition.h"

DeathCondition::DeathCondition(const bool isDead)
{
	m_name = "is dead Condition";
	setIsDead(isDead);
}

DeathCondition::~DeathCondition()
= default;

void DeathCondition::setIsDead(const bool state)
{
	m_isDead = state;
}

bool DeathCondition::Condition()
{
	return m_isDead;
}