#pragma once
#include "CVector3.h"
#include "MathHelper.h"

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
	MathHelper::Matrix4 positionMatrix;
	unsigned int instanceShader;
	unsigned int instanceTexture;
	float instanceColor[3];
};

struct Hex
{
	unsigned int row;
	unsigned int column;
	CVector3 cellCenter;
	MathHelper::Matrix4 positionMatrix;
	ModelInstance* instanceInGrid;
	unsigned int cellShader;
	unsigned int cellVAO;
	unsigned int cellTexture;
	float cellColor[3];
};
