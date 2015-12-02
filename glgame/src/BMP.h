#ifndef _BMP_H
#define _BMP_H

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <sstream>


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")


#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <glaux.h>	


void BMP_Texture(UINT textureArray[], LPSTR strFileName, int ID);




#endif