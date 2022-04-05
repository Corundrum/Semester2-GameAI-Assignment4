#include "RadiusCondition.h"

RadiusCondition::RadiusCondition(const bool withinRadius)
{
	m_name = "Radius Condition";
	setIsWithinRadius(withinRadius);
}

RadiusCondition::~RadiusCondition()
= default;

void RadiusCondition::setIsWithinRadius(const bool state)
{
	m_isWithinRadius = state;
}

bool RadiusCondition::Condition()
{
	return m_isWithinRadius;
}
