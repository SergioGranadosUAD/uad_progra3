#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "windows.h"

#include "../Include/Globals.h"
#include "../Include/CAppParcial1.h"
#include "../Include/Object3D.h"
#include "../Include/CTextureLoader.h"

/* */
CAppParcial1::CAppParcial1() :
	CAppParcial1(CGameWindow::DEFAULT_WINDOW_WIDTH, CGameWindow::DEFAULT_WINDOW_HEIGHT)
{
}

/* */
CAppParcial1::CAppParcial1(int window_width, int window_height) :
	CApp(window_width, window_height)
{
	cout << "Constructor: CAppEmpty(int window_width, int window_height)" << endl;

	// Initialize class member variables here
	// ======================================
	//
	// ======================================
}

/* */
CAppParcial1::~CAppParcial1()
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
void CAppParcial1::initialize()
{
	
}

/* */
void CAppParcial1::run()
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
void CAppParcial1::update(double deltaTime)
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
void CAppParcial1::render()
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

		// Get a matrix that has both the object rotation and translation
		CVector3 objectPosition = CVector3::ZeroVector();

		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)0.0, objectPosition);

		unsigned int modelShader = currentShaderID;
		unsigned int modelVAO = geometryID;
		unsigned int modelTexture = 0;
		float color[3] = { 1.0f, 1.0f, 1.0f };

		getOpenGLRenderer()->renderObject(
			&modelShader,
			&modelVAO,
			&modelTexture,
			mObject.getVertexIndexValues()->size()/3,
			color,
			&modelMatrix,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);
	}
}

/* */
void CAppParcial1::onMouseMove(float deltaX, float deltaY)
{
	// Update app-specific stuff when mouse moves here 
	// ===============================================
	//
	// ===============================================
}

/* */
void CAppParcial1::executeMenuAction()
{
	if (getMenu() != NULL)
	{
		// Execute app-specific menu actions here
		// ======================================
		//
		// ======================================
	}
}

void CAppParcial1::onF2(int mods) {
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
				(int)mObject.getVertexValues()->size()/3,
				mObject.getNormalValues()->data(),
				(int)mObject.getNormalValues()->size()/3,
				mObject.getUVCoordValues()->data(),
				(int)mObject.getUVCoordValues()->size()/3,
				mObject.getVertexIndexValues()->data(),
				(int)mObject.getVertexIndexValues()->size()/3,
				mObject.getNormalIndexValues()->data(),
				(int)mObject.getNormalIndexValues()->size()/3,
				mObject.getUVCoordIndexValues()->data(),
				(int)mObject.getUVCoordIndexValues()->size()/3
			);

			if (!loadedToGraphicsCard) {
				std::cout << "No funciono :(" << std::endl;
			}
		}
	}
}