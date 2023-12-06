#include "../stdafx.h"

#include <iostream>
using namespace std;


#include "windows.h"


#include "../Include/Globals.h"
#include "../Include/CAppParcial3.h"
#include "../Include/Object3D.h"
#include "../Include/CTextureLoader.h"
#include "../Include/MathHelper.h"



/* */
CAppParcial3::CAppParcial3() :
	CAppParcial3(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT, false, "")
{
}

/* */
CAppParcial3::CAppParcial3(int window_width, int window_height, bool fullscreen, string projectDir) :
	CApp(window_width, window_height, fullscreen),
	m_projectDir(projectDir)
{
	cout << "Constructor: CAppEmpty(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================

	CVector3 eyePos{ 0.0f, 5.0f, 5.0f };
	CVector3 lookAtPos{ 0.0f, 0.0f, -10.0f };
	CVector3 upVector{ 0.0f, 1.0f, 0.0f };

	m_camera = new Camera(window_width, window_height, 75.0f, 0.001f, 1000.f, eyePos, lookAtPos, upVector);
}

/* */
CAppParcial3::~CAppParcial3()
{
	cout << "Destructor: ~CAppEmpty()" << endl;

	// Free memory allocated in this class instance here
	// =================================================
	//
	// =================================================

	for (unsigned int i : mTextureID) {
		getOpenGLRenderer()->deleteTexture(&i);
	}

	for (auto& obj : m_modelInstances)
	{
		getOpenGLRenderer()->freeGraphicsMemoryForObject(
			&obj.modelID
		);
	}

	getOpenGLRenderer()->freeGraphicsMemoryForObject(
		&m_cellGeometryID
	);

	getOpenGLRenderer()->freeGraphicsMemoryForObject(
		&geometryID
	);

	for (auto& hex : m_hexGrid)
	{
		delete hex;
	}
	
	delete m_camera;
}

/* */
void CAppParcial3::initialize()
{
	string filePath;
	if (!m_projectDir.empty())
	{
		filePath = m_projectDir;
		filePath.append("\\Resources\\MEDIA\\HEXGRID\\hexgrid_cfg.json");
	}
	else
	{
		filePath = "Resources/MEDIA/HEXGRID/hexgrid_cfg.json";
	}

	json data;
	ifstream file(filePath);

	data = json::parse(file);

	m_numColumns = data["HexGrid"]["numCols"];
	m_numRows = data["HexGrid"]["numRows"];
	m_cellSize = data["HexGrid"]["cellSize"];
	string orientation = data["HexGrid"]["orientation"];
	m_cellOrientation = orientation;

	loadHexagonObject();

	loadObjectsToGraphicMemory(data);

	loadObjectInstances(data);

	setupHexgrid();

	m_renderQuadTree.populateQuadTree(m_initialAABB, m_hexGrid);
}

/* */
void CAppParcial3::run()
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
void CAppParcial3::update(double deltaTime)
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
void CAppParcial3::render()
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

		vector<Hex*> visibleNodes;
		m_renderQuadTree.render(m_camera, &visibleNodes);

		MathHelper::Matrix4* viewMatrix = (MathHelper::Matrix4*)m_camera->getViewMatrix();
		MathHelper::Matrix4* projectionMatrix = (MathHelper::Matrix4*)m_camera->getProjectionMatrix();

		for (Hex* currentCell : visibleNodes)
		{
			getOpenGLRenderer()->renderObject(
				&currentCell->cellShader,
				&currentCell->cellVAO,
				&currentCell->cellTexture,
				m_hexagonObject.getCellVerticesIndex().size() / 3,
				currentCell->cellColor,
				&currentCell->positionMatrix,
				viewMatrix,
				projectionMatrix,
				COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
				false
			);

			if (currentCell->instanceInGrid != nullptr)
			{
				ModelInstance* instance = currentCell->instanceInGrid;
				getOpenGLRenderer()->renderObject(
					&instance->instanceShader,
					&instance->modelID,
					&instance->instanceTexture,
					instance->faceCount,
					instance->instanceColor,
					&instance->positionMatrix,
					viewMatrix,
					projectionMatrix,
					COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
					false
				);
			}
		}
	}
}

/* */
void CAppParcial3::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppParcial3::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppParcial3::onF2(int mods) {
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

void CAppParcial3::onArrowUp(int mods)
{
	if (mods & KEY_MOD_CONTROL)
	{
		m_camera->moveUp(m_movementSpeed, mods);
	}
	else
	{
		m_camera->moveForward(m_movementSpeed, mods);
	}
}

void CAppParcial3::onArrowDown(int mods)
{
	if (mods & KEY_MOD_CONTROL)
	{
		m_camera->moveUp(-m_movementSpeed, mods);
	}
	else
	{
		m_camera->moveForward(-m_movementSpeed, mods);
	}
}

void CAppParcial3::onArrowLeft(int mods)
{
	m_camera->strafe(-m_movementSpeed, mods);
}

void CAppParcial3::onArrowRight(int mods)
{
	m_camera->strafe(m_movementSpeed, mods);
}

void CAppParcial3::loadHexagonObject()
{
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
}

void CAppParcial3::loadObjectsToGraphicMemory(json& data)
{
	//Cargar los objetos en la memoria gráfica.
	for (auto& obj : data["Models"])
	{
		bool loadedToGraphicsCard;
		string objectName = obj["name"];
		string objectPath = obj["filename"];

		unsigned int objectShaderID;
		unsigned int objectGeometryID;

		Object3D actualObject;

		string objectFilePath;

		if (!m_projectDir.empty())
		{
			objectFilePath = m_projectDir;
			objectFilePath.append("\\Resources\\MEDIA\\");
		}
		else
		{
			objectFilePath = "Resources/MEDIA/";
		}

		if (!actualObject.loadFile(objectFilePath + objectPath))
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
}

void CAppParcial3::loadObjectInstances(json& data)
{
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
}

void CAppParcial3::setupHexgrid()
{
	bool isPointy = false;
	float width = 0;
	float height = 0;

	if (m_cellOrientation == "pointy")
	{
		width = sqrt(3) * m_cellSize + CELL_PADDING;
		height = 1.5f * m_cellSize + CELL_PADDING;

		isPointy = true;
	}
	else if (m_cellOrientation == "flat")
	{
		width = 1.5f * m_cellSize + CELL_PADDING;
		height = sqrt(3) * m_cellSize + CELL_PADDING;

		isPointy = false;

	}

	float initialPosX = -((width * m_numRows) * 0.5f) + m_screenAdjustX;
	float initialPosY = m_screenAdjustY;
	float initialPosZ = -((height * m_numColumns) * 0.5f) + m_screenAdjustZ;

	for (int col = 0; col < m_numColumns; ++col)
	{
		for (int row = 0; row < m_numRows; ++row)
		{
			Hex* currentCell = new Hex();
			currentCell->column = col;
			currentCell->row = row;

			 float actualPosX = initialPosX + (width * row);
			 float actualPosZ = initialPosZ + (height * col);

			 if (isPointy)
			 {
				 if (col % 2 != 0)
				 {
					 actualPosX -= width * .5f;
				 }
			 }
			 else
			 {
				 if (row % 2 != 0)
				 {
					 actualPosZ += height * .5f;
				 }
			 }

			currentCell->cellCenter = CVector3(actualPosX, initialPosY, actualPosZ);
			currentCell->positionMatrix = MathHelper::TranslationMatrix(actualPosX, initialPosY, actualPosZ);

			currentCell->cellShader = m_cellShaderID;
			currentCell->cellVAO = m_cellGeometryID;
			currentCell->cellTexture = 0;
			currentCell->cellColor[0] = 2.0f;
			currentCell->cellColor[1] = 2.0f;
			currentCell->cellColor[2] = 2.0f;

			
			bool instanceFound = false;
			for (auto& instance : m_modelInstances)
			{
				if (instance.row == col && instance.column == row)
				{
					MathHelper::Matrix4 instanceTranslationRotationMatrix = MathHelper::SimpleModelMatrixRotationTranslation(
						instance.rotation.getY(),
						CVector3(actualPosX, initialPosY, actualPosZ)
					);

					MathHelper::Matrix4 instanceScaleMatrix = MathHelper::ScaleMatrix(instance.scale, instance.scale, instance.scale);
					instance.positionMatrix = MathHelper::Multiply(instanceScaleMatrix, instanceTranslationRotationMatrix);

					instance.instanceShader = m_cellShaderID;
					unsigned int instanceVAO = instance.modelID;
					instance.instanceTexture = 0;
					instance.instanceColor[0] = 0.0f;
					instance.instanceColor[1] = 0.0f;
					instance.instanceColor[2] = 2.0f;

					currentCell->instanceInGrid = &instance;

					instanceFound = true;
				}
				if (!instanceFound)
				{
					currentCell->instanceInGrid = nullptr;
				}
			}

			m_hexGrid.push_back(currentCell);
		}
	}

	CVector3 topLeftPoint(initialPosX - width, initialPosY, initialPosZ - height);
	CVector3 topRightPoint(initialPosX + (width * m_numRows) + width, initialPosY, initialPosZ - height);
	CVector3 bottomleftPoint(initialPosX - width, initialPosY, initialPosZ + (height * m_numColumns) + height);
	CVector3 bottomRightPoint(initialPosX + (width * m_numRows) + width, initialPosY, initialPosZ + (height * m_numColumns) + height);
	CVector3 pointsAABB[4] = { topLeftPoint, topRightPoint, bottomleftPoint, bottomRightPoint };
	m_initialAABB.setPoints(pointsAABB);
}
