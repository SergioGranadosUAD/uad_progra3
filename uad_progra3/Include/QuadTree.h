#pragma once
#include "QuadTreeNode.h"


class QuadTree {
public:
	QuadTree() = default;
	~QuadTree() = default;

	void render(Camera* cam, vector<Hex*>* visibleCells);
	void populateQuadTree(AABB_2D startingAABB, vector<Hex*> cellNodes);
private:
	QuadTreeNode* m_root;
};