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
	bool loadFile(const std::string& fileName);

	std::vector<float>* getVertexValues() { return &mVertexValues; };
	std::vector<float>* getNormalValues() { return &mNormalValues; };
	std::vector<float>* getUVCoordValues() { return &mUVCoordValues; };
	std::vector<unsigned short>* getVertexIndexValues() { return &mVertexIndexValues; };
	std::vector<unsigned short>* getNormalIndexValues() { return &mNormalIndexValues; };
	std::vector<unsigned short>* getUVCoordIndexValues() { return &mUvCoordIndexValues; };
	std::vector<std::string>* getMaterialNameValues() { return &mMaterialNameValues; };
	std::vector<std::string>* getTextureNameValues() { return &mTextureNameValues; };
	bool hasTextures() const { return mHasTextures; };
	bool hasUVs() const { return mHasUVCoords; };

private:
	void readVertexValues(std::stringstream& sstream);
	void readNormalValues(std::stringstream& sstream);
	void readUVCoordValues(std::stringstream& sstream);
	void buildFace(std::stringstream& sstream);
	void readTextures(const std::string& fileName);
	void TriangulateFace(const std::vector<VertexValues>& vertexList);

	const int TRIANGLE_VERTEX_COUNT = 3;

	//Cambiar
	bool mHasUVCoords = true;

	bool mHasTextures = false;
	std::vector<float> mVertexValues;
	std::vector<float> mNormalValues;
	std::vector<float> mUVCoordValues;
	std::vector<unsigned short> mVertexIndexValues;
	std::vector<unsigned short> mNormalIndexValues;
	std::vector<unsigned short> mUvCoordIndexValues;
	std::vector<std::string> mMaterialNameValues;
	std::vector<std::string> mTextureNameValues;
	std::vector<unsigned int> mTextureIndexValue;
	std::vector<unsigned int> mMaterialFaceCountValue;
};

