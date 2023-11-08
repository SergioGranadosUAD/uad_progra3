#pragma once

#ifndef CAPPPARCIAL3_H
#define CAPPPARCIAL3_H

#include "Globals.h"
#include "CApp.h"
#include "Object3D.h"
#include "CVector3.h"
#include "Hexagon.h"
#include "Structs.h"
#include <unordered_map>

#define CELL_PADDING 0.1f

using std::vector;
using std::unordered_map;

// --------------------------------------------------------------------------------------------------------------------------------------
// Class that inherits from Base class CApp
// Base class CApp has members for: CGameWindow, CGameMenu, and COpenGLRenderer, which we can access through the public/protected methods
// --------------------------------------------------------------------------------------------------------------------------------------

class CAppParcial3 : public CApp
{
private:

	// ---------------------------------------------------
	// Private member variables specific to CAppEmpty only
	// ---------------------------------------------------
	//
	// ---------------------------------------------------

protected:

	// Method to initialize the menu 
	// (not needed in CAppEmpty, so return false)
	bool initializeMenu() { return false; }

public:
	// Constructors and destructor
	CAppParcial3();
	CAppParcial3(int window_width, int window_height, bool fullscreen, string projectDir);
	~CAppParcial3();

	// --------------------------------------------------------------------------------------------------------------------------
	// Inherited methods from CApp
	// Initialize(), update(), run(), and render() are PURE VIRTUAL methods, so they need to be implemented in this DERIVED class
	// --------------------------------------------------------------------------------------------------------------------------

	// Method to initialize any objects for this class
	void initialize();

	// Method to update any objecs based on time elapsed since last frame
	void update(double deltaTime);

	// Method to run the app
	void run();

	// Method to render the app
	void render();

	// ------------------------------------------------------------------------------------------------------------------------
	// Other inherited methods from CApp. These are optional to override.
	// Only provide an implementation in this DERIVED class if you need them to do something different than the base class CApp
	// -----------------------------------------------------------------------------------------------------------------------

	// Executes the action corresponding to the selected menu item
	void executeMenuAction();

	// Called when mouse moves within the window
	void onMouseMove(float deltaX, float deltaY);

	// -----------------------------------------
	// Public methods specific to CAppEmpty only
	// -----------------------------------------
	//
	// -----------------------------------------
	void onF2(int mods) override;

private:

	// ------------------------------------------
	// Private methods specific to CAppEmpty only
	// ------------------------------------------
	//
	// ------------------------------------------
	Hexagon m_hexagonObject;
	Object3D mObject;
	vector<unsigned int> mTextureID;
	unsigned int currentShaderID;
	unsigned int geometryID;
	string m_projectDir;

	unsigned int m_cellShaderID;
	unsigned int m_cellGeometryID;
	int m_numRows;
	int m_numColumns;
	float m_cellSize;
	string m_cellOrientation;
	unordered_map<string, ModelData> m_modelIDs;
	vector<ModelInstance> m_modelInstances;

	const float m_screenAdjustX = -5.f;
	const float m_screenAdjustY = -20.f;
	const float m_screenAdjustZ = -25.f;

	double mRotationSpeed = 50.0f;
	double mCurrentRotation = 0.0f;
	CVector3 mCurrentPosition{ 0.0f, -20.0f, -15.0f };
};

#endif // !CAPPPARCIAL3_H