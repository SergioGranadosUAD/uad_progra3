// uad_progra3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

using namespace std;

#include "Include/CApp.h"
#include "Include/CAppCubeTest.h"
#include "Include/CAppObjLoader.h"
#include "Include/CAppGeometricFigures.h"
#include "Include/CAppEmpty.h"
#include "Include/CAppParcial1.h"
#include "Include/CAppParcial2.h"

int main(int argc, char** argv)
{
	int resWidth;
	int resHeight;
	bool isFullscreen;
	std::string videoMode;
	std::string className;
	std::string projectDir;
	if (argc > 1) {
		resWidth = stoi(argv[1]);
		resHeight = stoi(argv[2]);
		videoMode = argv[3];
		className = argv[4];
		projectDir = argv[5];
	}
	else {
		resWidth = 800;
		resHeight = 600;
		videoMode = "Windowed";
		className = "CAppParcial2";
		projectDir = "";
	}

	if (videoMode == "Windowed") {
		isFullscreen = false;
	}
	else {
		isFullscreen = true;
	}

	CApp *app = NULL;					// Pointer to BASE class CApp

	if (className == "CAppCubeTest") {
		app = new CAppCubeTest(resWidth, resHeight, isFullscreen);		// Using pointer to base class, create a new object of DERIVED class
	}
	else if (className == "CAppObjLoader") {
		app = new CAppObjLoader(resWidth, resHeight, isFullscreen);		// Using pointer to base class, create a new object of DERIVED class
	}
	else if (className == "CAppGeometricFigures") {
		app = new CAppGeometricFigures(resWidth, resHeight, isFullscreen);		// Using pointer to base class, create a new object of DERIVED class
	}
	else if (className == "CAppParcial1") {
		app = new CAppParcial1(resWidth, resHeight, isFullscreen);		// Using pointer to base class, create a new object of DERIVED class
	}
	else if (className == "CAppParcial2") {
		app = new CAppParcial2(resWidth, resHeight, isFullscreen, projectDir);		// Using pointer to base class, create a new object of DERIVED class
	}
	
	app->run();							// Run the app
	delete app;							// Delete pointer
	app = NULL;							// Set pointer to NULL

	return 0;
}

