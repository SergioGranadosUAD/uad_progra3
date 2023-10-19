#include "../stdafx.h"

#include <iostream>
using namespace std;

#include "windows.h"

#include "../Include/Globals.h"
#include "../Include/CAppParcial2.h"
#include "../Include/Object3D.h"
#include "../Include/CTextureLoader.h"
#include "../Include/MathHelper.h"

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

	double degrees = mRotationSpeed * deltaTime * 0.001f;

	mCurrentRotation += degrees;

	if (mCurrentRotation > 360.0f) {
		mCurrentRotation -= 360.0f;
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
		double mCurrentRadians = mCurrentRotation * PI_OVER_180;
		// =================================
		//
		// =================================

		// Get a matrix that has both the object rotation and translation
		CVector3 objectPosition = mCurrentPosition;
		objectPosition.X -= 5.0f;

		MathHelper::Matrix4 modelMatrix = MathHelper::SimpleModelMatrixRotationTranslation((float)mCurrentRadians, objectPosition);
		MathHelper::Matrix4 translationMatrix = MathHelper::TranslationMatrix(mCurrentPosition.X, mCurrentPosition.Y, mCurrentPosition.Z);
		MathHelper::Matrix4 rotationMatrix = MathHelper::RotAroundY(mCurrentRadians);

		MathHelper::Matrix4 modelMatrixSoldier = MathHelper::Multiply(rotationMatrix, translationMatrix);

		unsigned int modelShader = currentShaderID;
		unsigned int modelVAO = geometryID;
		unsigned int modelTexture = 0;
		float color[3] = { 1.0f, 1.0f, 1.0f };

		getOpenGLRenderer()->renderObject(
			&modelShader,
			&modelVAO,
			&modelTexture,
			mObject.getVertexIndexValues()->size() / 3,
			color,
			&modelMatrixSoldier,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);

		getOpenGLRenderer()->renderObject(
			&modelShader,
			&modelVAO,
			&modelTexture,
			mObject.getVertexIndexValues()->size() / 3,
			color,
			&modelMatrix,
			COpenGLRenderer::EPRIMITIVE_MODE::TRIANGLES,
			false
		);
		
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