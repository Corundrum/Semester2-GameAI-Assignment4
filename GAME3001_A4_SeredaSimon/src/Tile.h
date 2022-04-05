#pragma once
#ifndef __TILEDLEVEL_H_
#define __TILEDLEVEL_H_

#include <fstream>
#include <map>
#include <vector>
#include "Game.h"
#include "TileObjects.h"
#include "TextureManager.h"

class Tile : public SpriteTileObject
{
public:
	Tile(SDL_Rect s, SDL_Rect d, bool o = false, bool h = false);
	Tile* Clone();

	void update();
	void draw();
	void clean();
	bool isObstacle();
	void setXY(float x, float y);

private:
	bool m_obstacle;
	bool m_hazard;
};

class TiledLevel : public TileObject
{
public:
	TiledLevel(const unsigned short r, const unsigned short c, const int w, const int h, const char* tileData, const char* levelData, const std::string tileKey);
	~TiledLevel();

	void update();
	void draw();
	void clean();

	std::vector<Tile*>& getObstacles() { return m_obstacles; }
	
private:
	const std::string m_tileKey;
	int m_rows, m_cols;
	std::map<std::string, Tile*> m_tiles;
	std::vector<std::vector<Tile*>> m_level;
	std::vector<Tile*> m_obstacles;
};

#endif // !__TILEDLEVEL_H_
