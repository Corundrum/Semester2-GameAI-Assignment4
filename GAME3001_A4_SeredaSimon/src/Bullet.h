#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"

class Bullet final : public DisplayObject {
public:
	Bullet(bool dam = false);
	~Bullet();

	static 	std::vector<Bullet*> s_pBullets;


	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	static void updateField();
	virtual void clean() override;

private:
	void m_move();
	bool damagePlayer;
}; 

#endif //!__BULLET__
