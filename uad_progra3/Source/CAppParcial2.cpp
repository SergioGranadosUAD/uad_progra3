#include "../stdafx.h"

#include <iostream>
using namespace std;


#include "windows.h"

#include "../Dependencies/JSON/nlohmann/json.hpp"
#include "../Include/Globals.h"
#include "../Include/CAppParcial2.h"
#include "../Include/Object3D.h"
#include "../Include/CTextureLoader.h"
#include "../Include/MathHelper.h"

using nlohmann::json;

/* */
CAppParcial2::CAppParcial2() :
	CAppParcial2(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT, false)
{
}

/* */
CAppParcial2::CAppParcial2(int window_width, int window_height, bool fullscreen) :
	CApp(window_width, window_height, fullscreen)
{
	cout << "Constructor: CAppEmpty(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppParcial2::~CAppParcial2()
{
	cout << "Destructor: ~CAppEmpty()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================

	for (unsigned int i : mTextureID) {
		getOpenGLRenderer()->deleteTexture(&i);
	}

	getOpenGLRenderer()->freeGraphicsMemoryForObject(
		&geometryID
	);
}

/* */
void CAppParcial2::initialize()
{
	//Leer json
	json data;
	ifstream file("Resources/MEDIA/HEXGRID/hexgrid_cfg.json");

	data = json::parse(file);

	m_numColumns = data["HexGrid"]["numCols"];
	m_numRows = data["HexGrid"]["numRows"];
	m_cellSize = data["HexGrid"]["cellSize"];
	string orientation = data["HexGrid"]["orientation"];
	m_cellOrientation = orientation;

	m_hexagonObject.populateHexagonVectors(m_cellSize, m_cellOrientation);

	m_cellShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
	bool loadedToGraphicsCard;
	//Cargar geometría del hexagono.
	loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
		&m_cellShaderID,
		&m_cellGeometryID,
		m_hexagonObject.getCellVertices().data(),
		(int)m_hexagonObject.getCellVertices().size() / 3,
		m_hexagonObject.getCellNormals().data(),
		(int)m_hexagonObject.getCellNormals().size() / 3,
		m_hexagonObject.getCellUVs().data(),
		(int)m_hexagonObject.getCellUVs().size() / 3,
		m_hexagonObject.getCellVerticesIndex().data(),
		(int)m_hexagonObject.getCellVerticesIndex().size() / 3,
		m_hexagonObject.getCellNormalsIndex().data(),
		(int)m_hexagonObject.getCellNormalsIndex().size() / 3,
		m_hexagonObject.getCellUVsIndex().data(),
		(int)m_hexagonObject.getCellUVsIndex().size() / 3
	);

	if (!loadedToGraphicsCard) {
		std::cout << "No funciono :(" << std::endl;
	}

	//Cargar los objetos en la memoria gráfica.
	for (auto& obj : data["Models"])
	{
		string objectName = obj["name"];
		string objectPath = obj["filename"];

		unsigned int objectShaderID;
		unsigned int objectGeometryID;

		Object3D actualObject;

		if (!actualObject.loadFile("Resources/MEDIA/" + objectPath))
		{
			cout << "Unable to load 3D model" << endl;
		}
		else
		{
			std::string materialName;

			//Cambiar por versión local.
			std::vector<std::string>* materialNameValues = mObject.getMaterialNameValues();
			for (int i = 0; i < materialNameValues->size(); ++i) {
				unsigned int textureID;
				materialName = materialNameValues->at(i);
				if (CTextureLoader::loadTexture(materialName.c_str(), &textureID, getOpenGLRenderer()))
				{
					mTextureID.push_back(textureID);
				}
			}

			if (actualObject.hasTextures() && mObject.hasUVs())
			{
				// Switch shaders to textured object ones
				objectShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

			}
			else
			{
				// Load color shader
				objectShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
			}

			loadedToGraphicsCard = false;

			unsigned short* sus = actualObject.getUVCoordIndexValues()->data();

			//Cargar geometría
			loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
				&objectShaderID,
				&objectGeometryID,
				actualObject.getVertexValues()->data(),
				(int)actualObject.getVertexValues()->size() / 3,
				actualObject.getNormalValues()->data(),
				(int)actualObject.getNormalValues()->size() / 3,
				actualObject.getUVCoordValues()->data(),
				(int)actualObject.getUVCoordValues()->size() / 3,
				actualObject.getVertexIndexValues()->data(),
				(int)actualObject.getVertexIndexValues()->size() / 3,
				actualObject.getNormalIndexValues()->data(),
				(int)actualObject.getNormalIndexValues()->size() / 3,
				actualObject.getUVCoordIndexValues()->data(),
				(int)actualObject.getUVCoordIndexValues()->size() / 3
			);

			if (loadedToGraphicsCard) 
			{
				ModelData dataToSave;
				dataToSave.modelID = objectGeometryID;
				dataToSave.faceCount = actualObject.getVertexIndexValues()->size() / 3;

				m_modelIDs.insert({ objectName, dataToSave });
			}
			else 
			{
				std::cout << "No funciono :(" << std::endl;
			}
		}

		
	}

	for (auto& obj : data["ModelInstances"]) {
		ModelInstance actualInstance;
		ModelData retrievedData = m_modelIDs.at(obj["model"]);
		actualInstance.modelID = retrievedData.modelID;
		actualInstance.faceCount = retrievedData.faceCount;
		actualInstance.row = obj["row"];
		actualInstance.column = obj["column"];
		actualInstance.scale = obj["scale"];
		
		std::vector<float> rotations = obj["rotation"];
		actualInstance.rotation = CVector3(rotations[0], rotations[1], rotations[2]);

		m_modelInstances.push_back(actualInstance);
	}

	bool si;
}

/* */
void CAppParcial2::run()
{
	// Check if CGameWindow object AND Graphics API specific Window library have been initialized
	if (canRun())
	{
		// Create the Window 
		// Note: The following create() method creates the Window itself. Do NOT make any OpenGL calls before this line, your app WILL crash.
		if (getGameWindow()->create(CAPP_PROGRA3_EMPTYAPP_WINDOW_TITLE))
		{
			initialize();

			// Set initial clear screen color
			getOpenGLRenderer()->setClearScreenColor(0.25f, 0.0f, 0.75f);

			// Enter main loop
			cout << "Entering Main loop" << endl;
			getGameWindow()->mainLoop(this);
		}
	}
}

/* */
void CAppParcial2::update(double deltaTime)
{
	// Do not update if delta time is < 0
	if (deltaTime <= 0.0f)
	{
		return;
	}


	// Update app-specific stuff here
	// ===============================
	//
	// ===============================
}

/* */
void CAppParcial2::render()
{
	CGameMenu* menu = getMenu();

	// If menu is active, render menu
	if (menu != NULL
		&& menu->isInitialized()
		&& menu->isActive())
	{
		//...
	}
	else // Otherwise, render app-specific stuff here...
	{
		// =================================
		//
		// =================================

		float initialPosX = -((m_cellSize * m_numRows) * 0.5f ) + m_screenAdjustX;
		float initialPosY = m_screenAdjustY;
		float initialPosZ = -((m_cellSize * m_numColumns) * 0.5f) + m_screenAdjustZ;

		for (int col = 0; col < m_numColumns; ++col)
		{
			for (int row = 0; row < m_numRows; ++row)
			{
				float actualPosX = 0;
				float actualPosZ = 0;

				if (m_cellOrientation == "pointy")
				{
					float width = sqrt(3) * m_cellSize + CELL_PADDING;
					float height = 1.5f * m_cellSize + CELL_PADDING;
					actualPosX = initialPosX + (width * row);
					actualPosZ = initialPosZ + (height * col);

					if (col % 2 != 0)
					{
						actualPosX += width * .5f;
					}
				}
				else if(m_cellOrientation == "flat")
				{
					float width = 1.5f * m_cellSize + CELL_PADDING;
					float height = sqrt(3) * m_cellSize + CELL_PADDING;
					actualPosZ = initialPosZ + (width * col);
					actualPosX = initialPosX + (height * row);

					if (row % 2 != 0)
					{
						actualPosZ += m_cellSize;
					}
				}
				
				
				MathHelper::Matrix4 actualPosMatrix = MathHelper::TranslationMatrix(actualPosX, initialPosY, actualPosZ);

				unsigned int cellShader = m_cellShaderID;
				unsigned int cellVAO = m_cellGeometryID;
				unsigned int cellTexture = 0;
				float cellColor[3] = { 2.0f, 2.0f, 2.0f };

				getOpenGLRenderer()->renderObject(
					&cellShader,
					&cellVAO,
					&cellTexture,
					m_hexagonObject.getCellVerticesIndex().size() / 3,
					cellColor,
					&actualPosMatrix,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);

				for (auto& instance : m_modelInstances)
				{
					if (instance.row == row && instance.column == col)
					{
						MathHelper::Matrix4 instanceTranslationRotationMatrix = MathHelper::SimpleModelMatrixRotationTranslation(
							instance.rotation.getY(),
							CVector3(actualPosX, initialPosY, actualPosZ)
						);

						MathHelper::Matrix4 instanceScaleMatrix = MathHelper::ScaleMatrix(instance.scale, instance.scale, instance.scale);
						MathHelper::Matrix4 finalInstanceMatrix = MathHelper::Multiply(instanceScaleMatrix, instanceTranslationRotationMatrix);

						unsigned int instanceShader = m_cellShaderID;
						unsigned int instanceVAO = instance.modelID;
						unsigned int instanceTexture = 0;
						float instanceColor[3] = { 0.0, 0.0f, 2.0f };

						getOpenGLRenderer()->renderObject(
							&instanceShader,
							&instanceVAO,
							&instanceTexture,
							instance.faceCount,
							instanceColor,
							&finalInstanceMatrix,
							COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
							false
						);
					}
				}
			}


		}

		// Get a matrix that has both the object rotation and translation
		/*CVector3 objectPosition = mCurrentPosition;
		objectPosition.X -= 5.0f;

		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation(0, objectPosition);
		MathHelper::Matrix4 translationMatrix = MathHelper::TranslationMatrix(mCurrentPosition.X, mCurrentPosition.Y, mCurrentPosition.Z);
		MathHelper::Matrix4 rotationMatrix = MathHelper::RotAroundY(0);

		MathHelper::Matrix4 modelMatrixSoldier = MathHelper::Multiply(rotationMatrix, translationMatrix);

		unsigned int modelShader = m_cellShaderID;
		unsigned int modelVAO = m_cellGeometryID;
		unsigned int modelTexture = 0;
		float color[3] = { 1.0f, 1.0f, 1.0f };

		getOpenGLRenderer()->renderObject(
			&modelShader,
			&modelVAO,
			&modelTexture,
			m_hexagonObject.getCellVerticesIndex().size() / 3,
			color,
			&modelMatrixSoldier,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);

		getOpenGLRenderer()->renderObject(
			&modelShader,
			&modelVAO,
			&modelTexture,
			m_hexagonObject.getCellVerticesIndex().size() / 3,
			color,
			&modelMatrix,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);*/
		
	}
}

/* */
void CAppParcial2::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppParcial2::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppParcial2::onF2(int mods) {
	setMenuActive(true);

	std::wstring wideStringBuffer = L"";
	wideStringBuffer.resize(MAX_PATH);

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = L" Obj Files\0*.obj\0 Stl Files\0*.stl\0 3DS Files\0*.3ds\0 All files\0*.*\0";
	ofn.lpstrFile = &wideStringBuffer[0];
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = L"Select a model file";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), NULL, 0, NULL, NULL);
		std::string multibyteString(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wideStringBuffer[0], (int)wideStringBuffer.size(), &multibyteString[0], size_needed, NULL, NULL);
		cout << "Filename to load: " << multibyteString.c_str() << endl;

		if (!mObject.loadFile(multibyteString.c_str()))
		{
			cout << "Unable to load 3D model" << endl;
		}
		else
		{
			std::string materialName;

			setMenuActive(false);
			std::vector<std::string>* materialNameValues = mObject.getMaterialNameValues();
			for (int i = 0; i < materialNameValues->size(); ++i) {
				unsigned int textureID;
				materialName = materialNameValues->at(i);
				if (CTextureLoader::loadTexture(materialName.c_str(), &textureID, getOpenGLRenderer()))
				{
					mTextureID.push_back(textureID);
				}
			}

			if (mObject.hasTextures() && mObject.hasUVs())
			{
				// Switch shaders to textured object ones
				currentShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_TEXTURED_OBJECT);

			}
			else
			{
				// Load color shader
				currentShaderID = getOpenGLRenderer()->getShaderProgramID(SHADER_PROGRAM_COLOR_OBJECT);
			}

			bool loadedToGraphicsCard;

			//Cargar geometría
			loadedToGraphicsCard = getOpenGLRenderer()->allocateGraphicsMemoryForObject(
				&currentShaderID,
				&geometryID,
				mObject.getVertexValues()->data(),
				(int)mObject.getVertexValues()->size() / 3,
				mObject.getNormalValues()->data(),
				(int)mObject.getNormalValues()->size() / 3,
				mObject.getUVCoordValues()->data(),
				(int)mObject.getUVCoordValues()->size() / 3,
				mObject.getVertexIndexValues()->data(),
				(int)mObject.getVertexIndexValues()->size() / 3,
				mObject.getNormalIndexValues()->data(),
				(int)mObject.getNormalIndexValues()->size() / 3,
				mObject.getUVCoordIndexValues()->data(),
				(int)mObject.getUVCoordIndexValues()->size() / 3
			);

			if (!loadedToGraphicsCard) {
				std::cout << "No funciono :(" << std::endl;
			}
		}
	}
}