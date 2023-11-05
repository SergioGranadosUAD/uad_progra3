#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "CVector3.h"
#include "MathHelper.h"

using std::vector;
using std::string;

class Hexagon
{
public:
	Hexagon() = default;
	virtual ~Hexagon() = default;

	void populateHexagonVectors(const float& size, const string& type);
	CVector3 pointy_hex_corner(const CVector3& center, const float& size, const unsigned int& i);
	CVector3 flat_hex_corner(const CVector3& center, const float& size, const unsigned int& i);

	inline vector<float> getCellVertices() { return m_cellVertices; };
	inline vector<float> getCellNormals() { return m_cellNormals; };
	inline vector<float> getCellUVs() { return m_cellUVs; };
	inline vector<unsigned short> getCellVerticesIndex() { return m_cellVerticesIdx; };
	inline vector<unsigned short> getCellNormalsIndex() { return m_cellNormalsIdx; };
	inline vector<unsigned short> getCellUVsIndex() { return m_cellUVsIdx; };

private:
	vector<float> m_cellVertices;
	vector<float> m_cellNormals;
	vector<float> m_cellUVs;
	vector<unsigned short> m_cellVerticesIdx;
	vector<unsigned short> m_cellNormalsIdx;
	vector<unsigned short> m_cellUVsIdx;
};