#pragma once
#ifndef __AGENT__
#define __AGENT__

#include <glm/vec4.hpp>
#include "NavigationObject.h"
#include "ActionState.h"
#include "Obstacle.h"
#include "Animation.h"
#include <unordered_map>
#include "SpriteSheet.h"

class Agent : public NavigationObject
{
public:
	Agent();
	~Agent();

	// Inherited via GameObject
	void draw() override = 0;
	void update() override = 0;
	void clean() override = 0;

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getLOSDistance() const;
	bool hasLOS() const;
	float getCurrentHeading() const;
	glm::vec4 getLOSColour() const;

	glm::vec2 getLeftLOSEndPoint() const;
	glm::vec2 getMiddleLOSEndPoint() const;
	glm::vec2 getRightLOSEndPoint() const;
	bool* getCollisionWhiskers(); // Returns entire array.
	glm::vec4 getLineColor(int index);
	float getWhiskerAngle() const;

	const ActionState getActionState() { return m_state; }

	// setters
	void setTargetPosition(glm::vec2 new_position);
	void setCurrentDirection(glm::vec2 new_direction);
	void setLOSDistance(float distance);
	void setHasLOS(bool state);
	void setCurrentHeading(float heading);
	void setLOSColour(glm::vec4 colour);

	void setLeftLOSEndPoint(glm::vec2 point);
	void setMiddleLOSEndPoint(glm::vec2 point);
	void setRightLOSEndPoint(glm::vec2 point);
	void setLineColor(int index, glm::vec4 color);
	void setWhiskerAngle(float a);
	void updateWhiskers(float a);

	void setActionState(ActionState a) { m_state = a; }

	// New Tree Actions
	virtual void attack() {}
	virtual void moveToLOS() {}
	virtual void moveToPlayer() {}
	virtual void moveToRange() {}
	virtual void patrol() {}
	virtual void idle() {}
	virtual void death() {}
	virtual void takeDamage() {}

	//New Utilty

	bool checkAgentLOSToTarget(Agent* agent, DisplayObject* target_object, std::vector<Obstacle*>& obstacles);
	bool checkAgentLOSToTarget(Agent* agent, SDL_Rect* target_object, std::vector<Obstacle*>& obstacles);

	//Sprite Settings
	// getters
	SpriteSheet* getSpriteSheet();
	Animation& getAnimation(const std::string& name);

	// setters
	void setSpriteSheet(SpriteSheet* sprite_sheet);
	void setAnimation(const Animation& animation);


private:
	void m_changeDirection();
	float m_currentHeading; // angle the ship is looking
	glm::vec2 m_currentDirection; // vector the ship is moving
	glm::vec2 m_targetPosition; // vector representing the target.position

	// LOS
	float m_LOSDistance;
	bool m_hasLOS;
	glm::vec4 m_LOSColour;

	glm::vec2 m_leftLOSEndPoint;
	glm::vec2 m_middleLOSEndPoint;
	glm::vec2 m_rightLOSEndPoint;
	// Add second left whisker.
	// Add second right whisker.
	glm::vec4 m_lineColor[3]; // Change to 5 when you add the two extra whiskers.
	bool m_collisionWhiskers[3]; // ""

	float m_whiskerAngle;
	// May want second whiskerAngle for ImGUI - optional

	// action state
	ActionState m_state;

	//Sprite Settings
	// private utility functions
	bool m_animationExists(const std::string& id);

	SpriteSheet* m_pSpriteSheet;

	std::unordered_map<std::string, Animation> m_pAnimations;
};



#endif /* defined ( __AGENT__) */