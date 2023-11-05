#include "../Include/Hexagon.h"

void Hexagon::populateHexagonVectors(const float& size, const string& type)
{
	CVector3 center = CVector3::ZeroVector();

	for (int i = 1; i <= 6; ++i)
	{
		CVector3 corner;
		if (type == "pointy")
		{
			corner = pointy_hex_corner(center, size, i);
		}
		else if (type == "flat")
		{
			corner = flat_hex_corner(center, size, i);
		}

		m_cellVertices.push_back(corner.X);
		m_cellVertices.push_back(corner.Y);
		m_cellVertices.push_back(corner.Z);

		m_cellNormals.push_back(0.0f);
		m_cellNormals.push_back(1.0f);
		m_cellNormals.push_back(0.0f);

		m_cellUVs.push_back(0.0f);
		m_cellUVs.push_back(0.0f);
		m_cellUVs.push_back(0.0f);
	}

	m_cellVerticesIdx.push_back(0);
	m_cellVerticesIdx.push_back(1);
	m_cellVerticesIdx.push_back(5);

	m_cellVerticesIdx.push_back(1);
	m_cellVerticesIdx.push_back(2);
	m_cellVerticesIdx.push_back(5);

	m_cellVerticesIdx.push_back(5);
	m_cellVerticesIdx.push_back(2);
	m_cellVerticesIdx.push_back(4);

	m_cellVerticesIdx.push_back(2);
	m_cellVerticesIdx.push_back(3);
	m_cellVerticesIdx.push_back(4);

	for (int i = 0; i < 12; ++i)
	{
		m_cellNormalsIdx.push_back(0);
		m_cellUVsIdx.push_back(0);
	}
}

CVector3 Hexagon::pointy_hex_corner(const CVector3& center, const float& size, const unsigned int& i)
{
	float angle_deg = (60 * i) - 30;
	float angle_rad = PI_OVER_180 * angle_deg;
	return CVector3(center.X + (size * cos(angle_rad)),
					center.Y,
					center.Z + (size * sin(angle_rad)));
}

CVector3 Hexagon::flat_hex_corner(const CVector3& center, const float& size, const unsigned int& i)
{
	float angle_deg = 60 * i;
	float angle_rad = PI_OVER_180 * angle_deg;
	return CVector3(center.X + (size * cos(angle_rad)),
					center.Y,
					center.Z + (size * sin(angle_rad)));
}
