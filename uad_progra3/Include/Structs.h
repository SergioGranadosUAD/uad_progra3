#pragma once
#include "CVector3.h"

struct ModelData
{
	unsigned int modelID;
	unsigned int faceCount;
};

struct ModelInstance
{
	unsigned int modelID;
	unsigned int faceCount;
	unsigned int row;
	unsigned int column;

	float scale;
	CVector3 rotation;
};
