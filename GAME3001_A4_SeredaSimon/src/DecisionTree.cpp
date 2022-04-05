#include "DecisionTree.h"
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"
#include "IdleAction.h"
#include "DeathAction.h"
#include "TakeDamageAction.h"
#include <iostream>

DecisionTree::DecisionTree()
= default;

DecisionTree::DecisionTree(Agent* agent)
{
	m_agent = agent;
}

DecisionTree::~DecisionTree()
= default;

Agent* DecisionTree::getAgent() const
{
	return m_agent;
}

void DecisionTree::setAgent(Agent* agent)
{
	m_agent = agent;
}

IdleCondition* DecisionTree::getIdleNode() const
{
	return m_idleNode;
}

DeathCondition* DecisionTree::getDeathNode() const
{
	return m_deathNode;
}

TakeDamageCondition* DecisionTree::getTakeDamageNode() const
{
	return m_takeDamageNode;
}

LOSCondition* DecisionTree::getLOSNode() const
{
	return m_LOSNode;
}

RadiusCondition* DecisionTree::getRadiusNode() const
{
	return m_RadiusNode;
}

CloseCombatCondition* DecisionTree::getCloseCombatNode() const
{
	return m_CloseCombatNode;
}

std::vector<TreeNode*>& DecisionTree::getTree()
{
	return m_treeNodeList;
}

void DecisionTree::setIdleNode(IdleCondition* node)
{
	m_idleNode = node;
}

void DecisionTree::setDeathNode(DeathCondition* node)
{
	m_deathNode = node;
}

void DecisionTree::setTakeDamageNode(TakeDamageCondition* node)
{
	m_takeDamageNode = node;
}

void DecisionTree::setLOSNode(LOSCondition* node)
{
	m_LOSNode = node;
}

void DecisionTree::setRadiusNode(RadiusCondition* node)
{
	m_RadiusNode = node;
}

void DecisionTree::setCloseCombatNode(CloseCombatCondition* node)
{
	m_CloseCombatNode = node;
}

TreeNode* DecisionTree::addNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type)
{
	switch (type)
	{
	case LEFT_TREE_NODE:
		parent->m_pLeft = child_node;
		break;
	case RIGHT_TREE_NODE:
		parent->m_pRight = child_node;
	}
	child_node->m_pParent = parent;
	return child_node;
}

void DecisionTree::display()
{
	for (auto* node : m_treeNodeList)
	{
		std::cout << node->m_name << std::endl;
	}
}

void DecisionTree::update()
{
	//do some checks in the scene
}

void DecisionTree::clean()
{
	for (auto* node : m_treeNodeList)
	{
		delete node;
		node = nullptr;
	}
	m_treeNodeList.clear();
	m_treeNodeList.shrink_to_fit();
	//wrangle remaining pointers
	m_idleNode = nullptr;
	m_deathNode = nullptr;
	m_takeDamageNode = nullptr;
	m_LOSNode = nullptr;
	m_RadiusNode = nullptr;
	m_CloseCombatNode = nullptr;
}

void DecisionTree::makeDecision()
{
	auto currentNode = m_treeNodeList[0];
	while (!currentNode->m_isLeaf)
	{
		currentNode = dynamic_cast<ConditionNode*>(currentNode)->Condition() ? (currentNode->m_pRight) : (currentNode->m_pLeft);
	}
	static_cast<ActionNode*>(currentNode)->Action();
}
