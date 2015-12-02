
#include "conf.h"
#include "glut.h"
#include "gl.h"
#include <windows.h>  // Header File For Windows
#include <gl\gl.h>   // Header File For The OpenGL32 Library
#include <gl\glu.h>   // Header File For The GLu32 Library
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include<PLAYSOUNDAPI.H>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <time.h>
#include <math.h>
#include<vector>
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include "BMP.h"

// obsluga klawiatury

void keyUp( unsigned char key,int a,int b ) {
	cgl.keyUp( key, a, b );	
}

void keyPress( unsigned char key, int a, int b ) {
	cgl.keyPress( key, a, b );
}

// przerysowanie ekranu
void display(void) {
	cgl.display();
}


//
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize( 880, 800 );
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
    glutCreateWindow("Game");
	//PlaySound("Carnal.wav", NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);

    glutDisplayFunc(display);
	
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	 
	cgl.init();
    glutMainLoop();
}


