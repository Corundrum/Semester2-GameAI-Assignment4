#include "EnemyHealthCondition.h"

EnemyHealthCondition::EnemyHealthCondition(bool healthy)
{
	setHealth(healthy);
	m_name = "Enemy Health Condition";
}

EnemyHealthCondition::~EnemyHealthCondition()
= default;

void EnemyHealthCondition::setHealth(bool healthy)
{
	m_healthy = healthy;
}

bool EnemyHealthCondition::Condition()
{
	return m_healthy;
}