#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

struct VertexValues {
	unsigned int vertexIndex;
	unsigned int uVCoordIndex;
	unsigned int normalIndex;
};

class Object3D {
public:
	Object3D();
	virtual ~Object3D();
	void loadFile(const std::string& fileName);

private:
	void readVertexValues(std::stringstream& sstream);
	void readNormalValues(std::stringstream& sstream);
	void readUVCoordValues(std::stringstream& sstream);
	void buildFace(std::stringstream& sstream);
	void readTextures(const std::string& fileName);
	void TriangulateFace(const std::vector<VertexValues>& vertexList);

	const int TRIANGLE_VERTEX_COUNT = 3;

	bool mHasUVCoords = false;
	bool mHasTextures = false;
	std::vector<float> mVertexValues;
	std::vector<float> mNormalValues;
	std::vector<float> mUVCoordValues;
	std::vector<unsigned int> mVertexIndexValues;
	std::vector<unsigned int> mNormalIndexValues;
	std::vector<unsigned int> mUvCoordIndexValues;
	std::vector<std::string> mMaterialNameValues;
	std::vector<std::string> mTextureNameValues;
	std::vector<unsigned int> mTextureIndexValue;
	std::vector<unsigned int> mMaterialFaceCountValue;
};

