#pragma once
#ifndef __PLAYER__
#define __PLAYER__

#include "PlayerAnimationState.h"
#include "Sprite.h"
#include "Bullet.h"

class Player final : public Sprite
{
public:
	Player();
	~Player();
	static Player* s_pPlayerObj;

	// Life Cycle Methods
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	void Attack();
	void SwordSlash();
	void Shoot();

	void addScore(int score);
	int getScore() { return m_score; }

	SDL_Rect getHitBox() { return hitBox; }

	// setters
	void setAnimationState(PlayerAnimationState new_state);

private:
	void m_buildAnimations();

	int m_score = 0;

	std::vector<Bullet*> m_pBullets;

	SDL_Rect hitBox;

	bool mouseLeft;
	bool mouseRight;

	float walkTimer = 1;

	bool isFacingLeft = 0;
	PlayerAnimationState m_currentAnimationState;
};

#endif /* defined (__PLAYER__) */