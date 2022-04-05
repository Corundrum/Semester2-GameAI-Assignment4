#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__

#include "Agent.h"
#include "CloseCombatCondition.h"
#include "LOSCondition.h"
#include "RadiusCondition.h"
#include "IdleCondition.h"
#include "DeathCondition.h"
#include "TakeDamageCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	//Constructor
	DecisionTree();
	DecisionTree(Agent* agent);

	//Destructor
	~DecisionTree();

	//Getters and Setters
	Agent* getAgent() const;
	void setAgent(Agent* agent);

	IdleCondition* getIdleNode() const;
	DeathCondition* getDeathNode() const;
	TakeDamageCondition* getTakeDamageNode() const;
	LOSCondition* getLOSNode() const;
	RadiusCondition* getRadiusNode() const;
	CloseCombatCondition* getCloseCombatNode() const;
	//RangedCombatCondition* getRangedCombatNode() const;
	std::vector<TreeNode*>& getTree();

	void setIdleNode(IdleCondition* node);
	void setDeathNode(DeathCondition* node);
	void setTakeDamageNode(TakeDamageCondition* node);
	void setLOSNode(LOSCondition* node);
	void setRadiusNode(RadiusCondition* node);
	void setCloseCombatNode(CloseCombatCondition* node);
	//void setRangedCombatNode(RangedCombatCondition* node);

	//Public Functions
	TreeNode* addNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void display();
	void update();
	void clean();

	void makeDecision(); //in order tree traversal

private:
	//Private Instance Members
	Agent* m_agent;

	IdleCondition* m_idleNode;
	DeathCondition* m_deathNode;
	TakeDamageCondition* m_takeDamageNode;
	LOSCondition* m_LOSNode;
	RadiusCondition* m_RadiusNode;
	CloseCombatCondition* m_CloseCombatNode;

	std::vector<TreeNode*> m_treeNodeList;
};

#endif // !__DECISION_TREE__

