#pragma once
#ifndef __TILE_OBJECTS_H__
#define __TILE_OBJECTS_H__

#include "SDL.h"
#include "Game.h"
#include "TextureManager.h"

class TileObject
{
public:
	virtual ~TileObject() {};
	virtual void update() = 0;
	virtual void draw() = 0;
	SDL_Rect* getDst() { return &m_dst; }
	const bool getEnabled() const { return m_enabled; }
	void setEnabled(const bool e) { m_enabled = e; }

protected:
	SDL_Rect m_dst;
	bool m_enabled;

protected:
	TileObject() : m_dst({ 0, 0, 0, 0 }), m_enabled(true) {}
	TileObject(const SDL_Rect d) :m_dst(d), m_enabled(true) {}
};

class SpriteTileObject : public TileObject
{
public:
	virtual void draw() = 0;
	SDL_Rect* getSrc() { return &m_src; }

protected:
	SDL_Rect m_src;
protected:
	SpriteTileObject(const SDL_Rect s, const SDL_Rect d) :TileObject(d), m_src(s) {}
};


#endif // !__TILE_OBJECTS_H__
