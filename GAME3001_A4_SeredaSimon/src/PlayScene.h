#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Heuristic.h"
#include "Player.h"
#include "Target.h"
#include "PathNode.h"
#include "Obstacle.h"
#include "Label.h"

#include "Tile.h"
#include "TileObjects.h"

#include "CloseCombatEnemy.h"
#include "RangedCombatEnemy.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;

	Label* m_score;

	bool m_isGridEnabled;
	
	std::vector<Obstacle*> m_pObstacles;

	//path node objects and functions
	std::vector<PathNode*> m_pGrid;
	void m_buildGrid();
	void m_toggleGrid(bool state);
	
	bool m_checkAgentLOS(Agent* agent, DisplayObject* target_object);
	bool m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object);
	void m_checkAllNodesWithTarget(DisplayObject* target_object);
	void m_clearNodes();
	void m_setPathNodeLOSDistance(int distance);

	TileObject* GetGo(const std::string& s);

	int playerHealth = 100;

	int m_LOSMode;
	int m_obstacleBuffer;
	int m_pathNodeLOSDistance;
	
	

	//tilemap
	std::vector<std::pair<std::string, TileObject*>> m_tiles;

};

#endif /* defined (__PLAY_SCENE__) */