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

int main()
{
	CApp *app = NULL;					// Pointer to BASE class CApp
	app = new CAppParcial2(800, 600);		// Using pointer to base class, create a new object of DERIVED class
	app->run();							// Run the app
	delete app;							// Delete pointer
	app = NULL;							// Set pointer to NULL

	return 0;
}

