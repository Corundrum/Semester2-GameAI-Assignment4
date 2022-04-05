#include "TakeDamageCondition.h"

TakeDamageCondition::TakeDamageCondition(const bool is_taking_damage)
{
	m_name = "taking damage Condition";
	setTakingDamage(is_taking_damage);
}

TakeDamageCondition::~TakeDamageCondition()
= default;

void TakeDamageCondition::setTakingDamage(const bool state)
{
	m_isTakingDamage = state;
}

bool TakeDamageCondition::Condition()
{
	return m_isTakingDamage;
}