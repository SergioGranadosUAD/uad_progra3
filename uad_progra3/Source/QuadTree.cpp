#include "../Include/QuadTree.h"

void QuadTree::render(Camera* cam, vector<Hex*>* visibleCells)
{
	m_root->render(cam, visibleCells);
}

void QuadTree::populateQuadTree(AABB_2D startingAABB, vector<Hex*> cellNodes)
{
	m_root = new QuadTreeNode();
	m_root->initialize(startingAABB, cellNodes);
}


