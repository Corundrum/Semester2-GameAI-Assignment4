#pragma once
#ifndef __BASE_ENEMY__
#define __BASE_ENEMY__

#include "Agent.h"
#include "DecisionTree.h"

class BaseEnemy : public Agent
{
public:
	BaseEnemy();
	~BaseEnemy();
	static std::vector<BaseEnemy*> s_EnemiesObj;

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters and setters (accessors and mutators)
	float getMaxSpeed() const;
	float getTurnRate() const;
	float getAccelerationRate() const;
	glm::vec2 getDesiredVelocity() const;
	int getMaxHealth() const { return m_maxHealth; }
	int getHealth() const { return m_health; }

	void setMaxSpeed(float speed);
	void setTurnRate(float angle);
	void setAccelerationRate(float rate);
	void setDesiredVelocity(glm::vec2 target_position);
	void setMaxHealth(int health) { m_maxHealth = health; }
	void setHealth(int health) { m_health = health; }

	void drawHealthBar();

	// public functions
	void Seek();
	void LookWhereYoureGoing(glm::vec2 target_direction);

	//new action functions
	SDL_Rect getHitBox() { return hitBox; }

	const DecisionTree* getTree();
	bool debug = 0;
protected:
	// private movement variables
	float m_maxSpeed;
	float m_turnRate;
	float m_accelerationRate;
	int m_maxHealth;
	int m_health;

	SDL_Rect hitBox;

	// new for Lab 7
	std::vector<glm::vec2> m_patrol;
	int m_waypoint;

	// where we want to go
	glm::vec2 m_desiredVelocity;

	// private functions
	void m_move();

	//decision Tree
	DecisionTree* m_tree;
	void m_buildTree();
};


#endif /* defined (__CLOSE_COMBAT_ENEMY__) */