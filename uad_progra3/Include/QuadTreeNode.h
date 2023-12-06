#pragma once
#include <vector>
#include "Structs.h"
#include "AABB_2D.h"
#include "CCamera.h"

#define CELL_FACE_COUNT 4
#define MIN_FACE_COUNT 300


using std::vector;

class QuadTreeNode
{
public:
	QuadTreeNode() = default;
	~QuadTreeNode();
	void initialize(AABB_2D boundingBox, vector<Hex*> cellNodes);
	void render(Camera* cam, vector<Hex*>* visibleCells);

private:
	QuadTreeNode* m_topLeft;
	QuadTreeNode* m_topRight;
	QuadTreeNode* m_bottomLeft;
	QuadTreeNode* m_bottomRight;
	vector<Hex*> m_nodeCells;
	AABB_2D m_nodeLimits;
};
