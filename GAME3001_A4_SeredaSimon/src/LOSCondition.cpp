#pragma once
#include "LOSCondition.h"
#include <iostream>

LOSCondition::LOSCondition(const bool LOS)
{
	m_name = "LOS Condition";
	setLOS(LOS);
}

LOSCondition::~LOSCondition()
= default;

void LOSCondition::setLOS(const bool state)
{
	m_hasLOS = state;
}

bool LOSCondition::Condition()
{
	return m_hasLOS;
}
