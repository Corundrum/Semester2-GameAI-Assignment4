#pragma once
#ifndef __BULLET__
#define __BULLET__

#include "DisplayObject.h"

class Bullet final : public DisplayObject {
public:
	Bullet();
	~Bullet();

	// Inherited via GameObject
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	void m_move();
}; 

#endif //!__BULLET__
