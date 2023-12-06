#include "../Include/QuadTreeNode.h"

QuadTreeNode::~QuadTreeNode()
{
	if (m_bottomLeft != nullptr)
	{
		delete m_bottomLeft;
	}

	if (m_bottomRight != nullptr)
	{
		delete m_bottomRight;
	}

	if (m_topLeft != nullptr)
	{
		delete m_topLeft;
	}

	if (m_topRight != nullptr)
	{
		delete m_topRight;
	}
}

void QuadTreeNode::initialize(AABB_2D boundingBox, vector<Hex*> cellNodes)
{
	m_nodeLimits = boundingBox;
	unsigned int faceCount = 0;
	for (auto& cell : cellNodes)
	{
		faceCount += CELL_FACE_COUNT;
		if (cell->instanceInGrid != nullptr)
		{
			faceCount += cell->instanceInGrid->faceCount;
		}
	}

	if (faceCount > MIN_FACE_COUNT && cellNodes.size() > 1)
	{
		//Get half points of the AABB.
		CVector3 topHalfPosition(CVector3::getMidPoint(m_nodeLimits.m_points[0], m_nodeLimits.m_points[1]));
		CVector3 bottomHalfPosition(CVector3::getMidPoint(m_nodeLimits.m_points[2], m_nodeLimits.m_points[3]));
		CVector3 leftHalfPosition(CVector3::getMidPoint(m_nodeLimits.m_points[0], m_nodeLimits.m_points[2]));
		CVector3 rightHalfPosition(CVector3::getMidPoint(m_nodeLimits.m_points[1], m_nodeLimits.m_points[3]));
		CVector3 centerPosition(CVector3::getMidPoint(m_nodeLimits.m_points[0], m_nodeLimits.m_points[3]));


		AABB_2D topLeftAABB;
		AABB_2D topRightAABB;
		AABB_2D bottomLeftAABB;
		AABB_2D bottomRightAABB;

		CVector3 topLeftPoints[4] = { m_nodeLimits.m_points[0], topHalfPosition, leftHalfPosition, centerPosition };
		CVector3 topRightPoints[4] = { topHalfPosition, m_nodeLimits.m_points[1], centerPosition, rightHalfPosition };
		CVector3 bottomLeftPoints[4] = { leftHalfPosition, centerPosition, m_nodeLimits.m_points[2], bottomHalfPosition };
		CVector3 bottomRightPoints[4] = { centerPosition, rightHalfPosition, bottomHalfPosition, m_nodeLimits.m_points[3] };

		topLeftAABB.setPoints(topLeftPoints);
		topRightAABB.setPoints(topRightPoints);
		bottomLeftAABB.setPoints(bottomLeftPoints);
		bottomRightAABB.setPoints(bottomRightPoints);

		vector<Hex*> topLeftCells;
		vector<Hex*> topRightCells;
		vector<Hex*> bottomLeftCells;
		vector<Hex*> bottomRightCells;

		for (auto& cell : cellNodes)
		{
			if (cell->cellCenter.getX() < centerPosition.getX())
			{
				if (cell->cellCenter.getZ() < centerPosition.getZ())
				{
					topLeftCells.push_back(cell);
				}
				else
				{
					bottomLeftCells.push_back(cell);
				}
			}
			else
			{
				if (cell->cellCenter.getZ() < centerPosition.getZ())
				{
					topRightCells.push_back(cell);
				}
				else
				{
					bottomRightCells.push_back(cell);
				}
			}
		}

		m_topLeft = new QuadTreeNode();
		m_topRight = new QuadTreeNode();
		m_bottomLeft = new QuadTreeNode();
		m_bottomRight = new QuadTreeNode();

		m_topLeft->initialize(topLeftAABB, topLeftCells);
		m_topRight->initialize(topRightAABB, topRightCells);
		m_bottomLeft->initialize(bottomLeftAABB, bottomLeftCells);
		m_bottomRight->initialize(bottomRightAABB, bottomRightCells);
	}
	else
	{
		m_nodeCells = cellNodes;
	}
}

void QuadTreeNode::render(Camera* cam, vector<Hex*>* visibleCells)
{
	if (cam->isAABBVisible(m_nodeLimits))
	{
		if (m_nodeCells.size() > 0)
		{
			for (auto& hex : m_nodeCells)
			{
				visibleCells->push_back(hex);
			}
		}
		else
		{
			if (m_topLeft != nullptr) 
			{
				m_topLeft->render(cam, visibleCells);
			}
			if (m_topRight != nullptr) {
				m_topRight->render(cam, visibleCells);
			}
			if (m_bottomLeft != nullptr)
			{
				m_bottomLeft->render(cam, visibleCells);
			}
			if (m_bottomRight != nullptr)
			{
				m_bottomRight->render(cam, visibleCells);
			}
		}
	}
}
