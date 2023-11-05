#include "../Include/Object3D.h"

Object3D::Object3D() {

}

Object3D::~Object3D() {

}

bool Object3D::loadFile(const std::string& fileName) {
	std::ifstream file(fileName);
	std::string line;
	//int materialFaceCount = 0;
	while (std::getline(file, line)) {
		std::stringstream sstream(line);
		std::string firstWord;
		sstream >> firstWord;
		if (firstWord != "#") {
			if (firstWord == "mtllib") {
				std::string mtlLibName;
				sstream >> mtlLibName;
				readTextures(mtlLibName);
			}
			else if (firstWord == "v") {
				readVertexValues(sstream);
			}
			else if (firstWord == "vn") {
				readNormalValues(sstream);
			}
			else if (firstWord == "vt") {
				readUVCoordValues(sstream);
			}
			else if (firstWord == "usemtl") {

			}
			else if (firstWord == "f") {
				buildFace(sstream);
				//materialFaceCount++;
			}
		}
	}

	if (mUVCoordValues.empty()) {
		mUVCoordValues.resize(mUvCoordIndexValues.size(), 0);
	}
	//Cambiar
	return true;
}

void Object3D::readVertexValues(std::stringstream& sstream) {
	float posX = 0;
	float posY = 0;
	float posZ = 0;

	sstream >> posX >> posY >> posZ;
	mVertexValues.push_back(posX);
	mVertexValues.push_back(posY);
	mVertexValues.push_back(posZ);
}

void Object3D::readNormalValues(std::stringstream& sstream) {
	float posX = 0;
	float posY = 0;
	float posZ = 0;

	sstream >> posX >> posY >> posZ;
	mNormalValues.push_back(posX);
	mNormalValues.push_back(posY);
	mNormalValues.push_back(posZ);
}

void Object3D::readUVCoordValues(std::stringstream& sstream) {
	float posX = 0;
	float posY = 0;
	float posZ = 0;

	sstream >> posX >> posY >> posZ;
	mUVCoordValues.push_back(posX);
	mUVCoordValues.push_back(posY);
	mUVCoordValues.push_back(posZ);
}

void Object3D::buildFace(std::stringstream& sstream) {
	std::vector<VertexValues> vertexList;
	std::string vertexData;
	while (sstream >> vertexData) {
		std::stringstream vertexStream(vertexData);
		while (vertexStream.good()) {
			VertexValues currentVertex;
			std::string vertexValue;

			getline(vertexStream, vertexValue, '/');
			currentVertex.vertexIndex = std::stoi(vertexValue);

			getline(vertexStream, vertexValue, '/');
			if (vertexValue.empty())
			{
				currentVertex.uVCoordIndex = 1;
			}
			else
			{
				currentVertex.uVCoordIndex = std::stoi(vertexValue);
			}

			getline(vertexStream, vertexValue, '/');
			currentVertex.normalIndex = std::stoi(vertexValue);

			vertexList.push_back(currentVertex);
		}
	}

	if (vertexList.size() == TRIANGLE_VERTEX_COUNT) {
		for (int i = 0; i < vertexList.size(); i++) {
			mVertexIndexValues.push_back(vertexList[i].vertexIndex-1);
			mUvCoordIndexValues.push_back(vertexList[i].uVCoordIndex-1);
			mNormalIndexValues.push_back(vertexList[i].normalIndex-1);
		}
	}
	else if (vertexList.size() > TRIANGLE_VERTEX_COUNT) {
		TriangulateFace(vertexList);
	}
}

void Object3D::readTextures(const std::string& fileName) {
	std::ifstream file(fileName);
	std::string line;
	int textureIndex = 0;
	while (std::getline(file, line)) {
		std::stringstream sstream(line);
		std::string firstWord;
		sstream >> firstWord;
		if (firstWord == "newmtl") {
			std::string materialName;
			sstream >> materialName;
			mMaterialNameValues.push_back(materialName);

			if (!mHasTextures) {
				mHasTextures = true;
			}

		}
		if (firstWord == "map_Ka") {
			std::string TextureFile;
			sstream >> TextureFile;
			mTextureNameValues.push_back(TextureFile);
			mTextureIndexValue.push_back(textureIndex);
		}

		textureIndex++;
	}
}

void Object3D::TriangulateFace(const std::vector<VertexValues>& vertexList) {
	VertexValues pivotVertex = vertexList[0];
	VertexValues middleVertex;
	VertexValues lastVertex;

	for (int i = 0; i < vertexList.size() - 2; i++) {
		middleVertex = vertexList[i + 1];
		lastVertex = vertexList[i + 2];

		mVertexIndexValues.push_back(pivotVertex.vertexIndex-1);
		mUvCoordIndexValues.push_back(pivotVertex.uVCoordIndex-1);
		mNormalIndexValues.push_back(pivotVertex.normalIndex-1);

		mVertexIndexValues.push_back(middleVertex.vertexIndex-1);
		mUvCoordIndexValues.push_back(middleVertex.uVCoordIndex-1);
		mNormalIndexValues.push_back(middleVertex.normalIndex-1);

		mVertexIndexValues.push_back(lastVertex.vertexIndex-1);
		mUvCoordIndexValues.push_back(lastVertex.uVCoordIndex-1);
		mNormalIndexValues.push_back(lastVertex.normalIndex-1);
	}

}