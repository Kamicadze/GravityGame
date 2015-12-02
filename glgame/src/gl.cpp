
#include "conf.h"
#include <cstdio>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <algorithm>
#include <time.h>
#include <math.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")





#include<vector>
#include <cmath>

#include <stdlib.h>
#include <sstream>


#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <glaux.h>	



using namespace std;

#include "util.h"
#include "glut.h"
#include "gl.h"
#include "BMP.h"




#define SCENE_FILE_NAME "../resources/mscene.bmp"
#define SCENE_FILE_NAME2 "../resources/mscene2.bmp"
BITMAPINFOHEADER bitmapInfoHeader;


int coin = 0;
int jump = 0;
int GO = 0;
int gravity = 1;
static int frameCount = 0, currentTime = 0, previousTime = 0, fps = 0; //fps 
int side = 90;
int grav = 0; 
int k0, k1, k2, k3, k4;
int game_stage = 0;
int score = 0;


#pragma region BMP
UINT TextureArray[7];
//UINT TextureArray2[10];
//UINT TextureArrayHUD[3];
//UINT TextureArraycyfry[10];
//UINT TextureArraycyfryWin[10];
//UINT TextureArraycyfryL[10];
//UINT TextureArraypasek[28];
//UINT TextureArrayMET[18];
//UINT TextureArrayship1[1];
static int obrazek = 5;
//bool is_first = true;
int al[1];
#pragma endregion 



#pragma region LOAD OBJ
struct vec3 {
	float x, y, z;
};
struct SFace {
	int v[3];
	int n[3];
	int t[3];
};
GLuint LoadObj(char * file) {

	FILE * fp = fopen(file, "r");

	if (fp == NULL) {
		printf("ERROR: Cannot read model file \"%s\".\n", file);
		return -1;
	}

	std::vector<vec3> * v = new std::vector<vec3>();
	std::vector<vec3> * n = new std::vector<vec3>();
	std::vector<vec3> * t = new std::vector<vec3>();
	std::vector<SFace> * f = new std::vector<SFace>();

	char buf[128];

	while (fgets(buf, 128, fp) != NULL) {
		if (buf[0] == 'v' && buf[1] == ' ') {
			vec3 * vertex = new vec3();
			sscanf(buf, "v %f %f %f", &vertex->x, &vertex->y, &vertex->z);
			v->push_back(*vertex);
		}
		if (buf[0] == 'v' && buf[1] == 't') {
			vec3 * vertex = new vec3();
			sscanf(buf, "vt %f %f", &vertex->x, &vertex->y);
			t->push_back(*vertex);
		}
		if (buf[0] == 'v' && buf[1] == 'n') {
			vec3 * vertex = new vec3();
			sscanf(buf, "vn %f %f %f", &vertex->x, &vertex->y, &vertex->z);
			n->push_back(*vertex);
		}
		if (buf[0] == 'f' && buf[1] == ' ') {
			SFace * face = new SFace();
			sscanf(buf, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				&face->v[0], &face->t[0], &face->n[0],
				&face->v[1], &face->t[1], &face->n[1],
				&face->v[2], &face->t[2], &face->n[2]
				);
			f->push_back(*face);
		}
	}

	fclose(fp);

	GLuint dlId;
	dlId = glGenLists(1);
	cout << dlId;
	glNewList(dlId, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < f->size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			vec3 * cv = &(*v)[((*f)[i].v[j] - 1)];
			vec3 * ct = &(*t)[((*f)[i].t[j] - 1)];
			vec3 * cn = &(*n)[((*f)[i].n[j] - 1)];
			glTexCoord2f(ct->x, ct->y);
			glNormal3f(cn->x, cn->y, cn->z);
			glVertex3f(cv->x, cv->y, cv->z);
		}
	}
	glEnd();
	glEndList();

	delete v;
	delete n;
	delete t;
	delete f;

	return dlId;

}
#pragma endregion



void map()
{
	if (game_stage == 0){

	}
//	else if (game_stage==0)
//	{
//#define SCENE_FILE_NAME "../resources/mscene2.bmp"
//	}
}
///////////////////////////////////////// utworzenie obiektu klasy CGL
CGL cgl;




///////////////////////////////////////// inicjalizacja srodowiska graficznego oraz zmiennych 
void CGL::init( void ) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective( 9, 1, 0.01, 10 );
	gluLookAt( 0, 0, 1, 0, 0, 0, 0, 1, 0 );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

	// ustawienie parametrow zrodla swiatla, wlaczenie oswietlenia
	float vec[4];
	SET_VEC4( vec, 0.7f, 0.7f, 0.4f, 1.0f );
	glLightfv(GL_LIGHT0, GL_POSITION, vec );
	SET_VEC4( vec, 0.7f, 0.7f, 0.7f, 1.0f );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, vec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, vec);
	glLightfv(GL_LIGHT0, GL_EMISSION, vec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, vec);    
	glEnable(GL_LIGHT0);    
	glEnable(GL_LIGHTING);

	// wlaczenie cieniowania Gouraud'a (smooth shading)
	glShadeModel( GL_SMOOTH ); 

	// ustawienie koloru tla
	glClearColor( 0.2f, 0.2f, 0.2f, 1.0f ); 

	// skasowanie bufora klawiatury
	for( int i=0; i < 256; i++ )
			cgl.keyStates[i] = false;

	// poczatkowe polozenie obiektu
	posX = 0.02;
	posY = 0.95;
	rotation = 0;	
	posXOld = posX;
	posYOld = posY;
	rotationOld = rotation;
	
	// odczytanie i wyswietlenie wersji OpenGL
	char* version = (char*)glGetString(GL_VERSION);
	printf("GL version: %s\n", version);	

	// odczytanie struktury sceny z zewnetrznego pliku (!!! dane bitmapy odczytane sa w formacie BGR888)
	
	mscene = CUTIL::LoadBitmapFile( SCENE_FILE_NAME, &bitmapInfoHeader );
	if( mscene == NULL ) {
		printf( "ERROR: cannot open input file with the scene data\n" );
		exit(1);
	}
	mscene_width = (int)bitmapInfoHeader.biWidth;
	mscene_height = (int)bitmapInfoHeader.biHeight;
	mscene_line_length = (int)(bitmapInfoHeader.biSizeImage / mscene_height );
	printf( "bitmap size: %d,%d (line length:%d, size:%d (%d))\n", mscene_width, mscene_height, mscene_line_length, bitmapInfoHeader.biSizeImage, (mscene_width*mscene_height*3) );
	/*
	for( int c = 0; c < mscene_height; c++ ) {
		for( int w = 0; w < mscene_width; w++ ) {

			int idx = xy2idx( w, c );

			printf("(%d,%d,%d)",mscene[idx],mscene[idx+1],mscene[idx+2] );
		}
		printf("\n");
	}*/
	BMP_Texture(TextureArray, "../resources/fake2.bmp", 0);
	BMP_Texture(TextureArray, "../resources/cave.bmp", 1);
	BMP_Texture(TextureArray, "../resources/gold.bmp", 2);
	BMP_Texture(TextureArray, "../resources/lava.bmp", 3);
	BMP_Texture(TextureArray, "../resources/water.bmp", 4);
	BMP_Texture(TextureArray, "../resources/Kali.bmp", 5);
	BMP_Texture(TextureArray, "../resources/metal.bmp", 6);
	al[0] = LoadObj("../resources/SM_Kali5.obj");




	//CUTIL::renderBitmapString(posX, posY, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
	// poczatkowe stany gry (! wazna jest kolejnosc stanow na liscie)
	state_list.push_back( STATE_DRAW_SCENE );
	state_list.push_back( STATE_DRAW_OBJECT );
	state_list.push_back( STATE_NEW_LIGHT );
	state_list.push_back( STATE_COLLISIONS_SCENE );
	state_list.push_back( STATE_COLLISIONS );
	state_list.push_front( STATE_MISSLE );
}


///////////////////////////////////////// glowna procedura przerysowania ekranu

bool is_first = true;

void CGL::display( void ) {

	if( is_first ) {
		t_old = CUTIL::getTime();
		is_first = false;
	}
	
	float t = CUTIL::getTime(); // aktualny czas
	float dt = t - t_old; // czas, ktory uplynal od poprzedniego wywolania display()
	t_old = t;
	
	 
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// obsluga klawiatury	
	keyOperations(); 
	
	// transformacja calej sceny
	glPushMatrix();	

	// przesuniecie planszy na srodek okna
	glTranslatef( -0.0f, 0.0f, 0.0f );

	// zmiana widoku kamery
	//glTranslatef( 0.0f, 0.0f, 0.1f ); // przesuniecie korekcyjne	
	//glRotatef( -30, 1, 0, 0 ); // pochylenie kamery (3PP)
	//glRotatef( -rotation+90, 0, 0, 1 ); // obrot planszy zgodnie z obrotem gracza
	glTranslatef( -posX, -posY, 0.0f );	// przesuniecie planszy do polozenia gracza


	// glowna petla obslugujaca stany gry
	int z = 0;
	int idx;
	bool bb;
	list<CMissle*>::iterator it;
	list<int> states = state_list;
	list<int>::iterator state;
	for( state = states.begin(); state != states.end(); ++state) {
		switch( *state ) {
	
			//////////////////////// dodatkowe zrodlo swiatla
			case STATE_NEW_LIGHT:
				// polozenie zrodla swiatla (LIGHT1)
				//glMaterialfv( GL_FRONT, GL_DIFFUSE, colorGray );
				//glMaterialfv( GL_FRONT, GL_AMBIENT, colorGray );
				glMaterialfv( GL_FRONT, GL_EMISSION, colorWhite );
				glPushMatrix();
				glTranslatef( 0.7f, 0.7f, 0.3f );
				glScalef(0.01f,0.01f,0.01f);
				glutSolidSphere( 1, 20, 20 );
				glPopMatrix();
				float vec[4];
				SET_VEC4( vec, 0.7f, 0.7f, 0.3f, 1.0f );
				glLightfv(GL_LIGHT0, GL_POSITION, vec );
				glMaterialfv( GL_FRONT, GL_EMISSION, colorDim );
				break;
			
			
				


			///////////////////	obrot obiektu w lewo
			case STATE_ROTATE_OBJECT_LEFT:
				rotationOld = rotation;
				rotation = rotation + dt * 0.2f;
				state_list.remove( STATE_ROTATE_OBJECT_LEFT );
				break;

			///////////////////	obrot obiektu w prawo
			case STATE_ROTATE_OBJECT_RIGHT:
				rotationOld = rotation;
				rotation = rotation - dt * 0.2f;
				state_list.remove( STATE_ROTATE_OBJECT_RIGHT );
				break;

			///////////////////	ruch obiektu do przodu
			case STATE_MOVE_OBJECT_UP:
			{
										 static int chuj = 0;
										 posXOld = posX;
										 posYOld = posY;
										 //	float j = 10.0;
										 //state_list.remove(STATE_FALL);
										
										 for (int i = 0; i < 10; i++)
										 {
											 posY = posY + dt*0.00002 + (0.00002*i);
										 }

										 //state_list.remove(STATE_FALL);
										 if (findState(STATE_COLLISIONS_SCENE)) {
											 bb = xy2mscene(posX, posY, idx);
											 if (bb || (mscene[idx] == SCENE_COLLISION && mscene[idx + 1] == SCENE_COLLISION && mscene[idx + 2] == SCENE_COLLISION)) {
												 posX = posXOld;
												 posY = posYOld;
											 }
										 }

										 state_list.remove(STATE_MOVE_OBJECT_UP);
										 //state_list.remove(STATE_FALL);

										 break;
			}
					
	
					

						
			case STATE_MAP:
			



			case STATE_FALL:
				
				posXOld = posX;
				posYOld = posY;

					for (int i = 0; i < 20; i++)
					{
						posY = posY - (dt*0.00001 - (0.00001*i))*gravity;
						if (findState(STATE_COLLISIONS_SCENE)) {
							bb = xy2mscene(posX, posY, idx);
							if (bb || (mscene[idx] == SCENE_COLLISION && mscene[idx + 1] == SCENE_COLLISION && mscene[idx + 2] == SCENE_COLLISION)) {
								posX = posXOld;
								posY = posYOld;
							}
						}
					}
				
				/*if (findState(STATE_COLLISIONS_SCENE)) {
					bb = xy2mscene(posX, posY, idx);
					if (bb || (mscene[idx] == SCENE_COLLISION && mscene[idx + 1] == SCENE_COLLISION && mscene[idx + 2] == SCENE_COLLISION)) {
						posX = posXOld;
						posY = posYOld;
					}
				}*/

				state_list.remove(STATE_FALL);


				break;

			case STARE_GRAV_SWAP:
				GO = 0;
				posX = 0.02;
				posY = 0.95;
				dt = 0;
				//rotation = 0;
				//posXOld = posX;
				//posYOld = posY;
				//rotationOld = rotation;
				coin = 0;
				
	//				SCENE_FILE_NAME "../resources/mscene1.bmp";
				
				//map();
				state_list.remove(STARE_GRAV_SWAP);
//				break;


			case STATE_MOVE_OBJECT_BACKWARD:
				side = 270;
				posXOld = posX;
				posYOld = posY;
				posX = posX - dt * 0.0001f * cos(rotation * 3.14f / 180.0f);
				posY = posY - dt * 0.0001f * sin(rotation * 3.14f / 180.0f);


				if (findState(STATE_COLLISIONS_SCENE)) {
					bb = xy2mscene(posX, posY, idx);
					if (bb || (mscene[idx] == SCENE_COLLISION && mscene[idx + 1] == SCENE_COLLISION && mscene[idx + 2] == SCENE_COLLISION)) {
						posX = posXOld;
						posY = posYOld;
					}
				}

				state_list.remove(STATE_MOVE_OBJECT_BACKWARD);
				break;

			case STATE_MOVE_OBJECT_FORWARD:

				side = 90;
				posXOld = posX;
				posYOld = posY;
				posX = posX + dt * 0.0001f * cos( rotation * 3.14f / 180.0f );
				posY = posY + dt * 0.0001f * sin( rotation * 3.14f / 180.0f );

			

				// wykrywanie kolizji z elementami sceny zdefiniowanymi w tablicy
				if( findState( STATE_COLLISIONS_SCENE ) ) {
					bb = xy2mscene( posX, posY, idx );
					if( bb || (mscene[idx] == SCENE_COLLISION && mscene[idx+1] == SCENE_COLLISION && mscene[idx+2] == SCENE_COLLISION) ) {
						posX = posXOld;
						posY = posYOld;
					}
				}

				state_list.remove( STATE_MOVE_OBJECT_FORWARD );
				break;

			/////////////////// kolizje
			case STATE_COLLISIONS:
				if (detectCollision(collisionTarget))
					//pmaterial = MATERIAL_PLAYER_BLUE;//to zamieniæ na kasowanie gracza
				{
					if (mscene[idx] == 0 && mscene[idx + 1] == 0 && mscene[idx + 2] == 255)
					{

						PlaySound("GO.wav", NULL, SND_ASYNC | SND_FILENAME | NULL);
						CUTIL::renderBitmapString(620, 55, GLUT_BITMAP_TIMES_ROMAN_24, "time left:");
						GO = 1;

					}
					else if (mscene[idx] == 0 && mscene[idx + 1] == 255 && mscene[idx + 2] == 255)
					{
						PlaySound("Coin.wav", NULL, SND_ASYNC | SND_FILENAME | NULL);
						//CUTIL::renderBitmapString(-3.9, 2.8f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
						mscene[idx] = 0;
						mscene[idx + 1] = 0;
						mscene[idx + 2] = 0;
						coin = coin + 100;
					}

					else if (mscene[idx] == 255 && mscene[idx + 1] == 0 && mscene[idx + 2] == 0)
					{
						PlaySound("win.wav", NULL, SND_ASYNC | SND_FILENAME | NULL);
						score = CUTIL::getTime();
						game_stage = 1;
						GO = 1;
						coin = coin + (100000 - game_stage);
						cout << coin << endl << game_stage;
						/*		FILE* fp = fopen("score.txt", "r");
									if (fp == NULL)
									return ;
									fscanf(fp, "%d %d %d %d %d", &k0, &k1, &k2, &k3, &k4);
									fclose(fp);*/
					}
					else if (mscene[idx] == 0 && mscene[idx + 1] == 255 && mscene[idx + 2] == 0)
					{
						PlaySound("77074__benboncan__bricks-falling.wav", NULL, SND_ASYNC | SND_FILENAME | NULL);
						mscene[idx] = 0;
						mscene[idx + 1] = 0;
						mscene[idx + 2] = 0;

					}
				}
				
				break;

			///////////////////	rysowanie obiektu
			case STATE_DRAW_OBJECT:
				drawObject();
				break;					

			///////////////////	rysowanie obiektu
			case STATE_DRAW_SCENE:
				drawScene();
				break;	

			////////////////// ruch, kolizje i kasowanie pociskow
			case STATE_MISSLE:
				for( it = missle_list.begin(); it != missle_list.end(); it++) {

					(*it)->x = (*it)->x + dt * 0.0005f * cos( (*it)->dir * 3.14f / 180.0f );
					(*it)->y = (*it)->y + dt * 0.0005f * sin( (*it)->dir * 3.14f / 180.0f );

					// kolizje pocisku ze scianami planszy
					bb = xy2mscene( (*it)->x, (*it)->y, idx );
					if( bb || (mscene[idx] == SCENE_COLLISION && mscene[idx+1] == SCENE_COLLISION && mscene[idx+2] == SCENE_COLLISION) ) {
						/*mscene[idx] = 0;
						mscene[idx+1] = 0;
						mscene[idx+2] = 0;*/
						delete (*it);
						missle_list.erase( it );
						break;
					}

					// skasowanie pocisku po przekroczeniu granic planszy 
					if( (*it)->x < 0 || (*it)->x > 1 || (*it)->y < 0 || (*it)->y > 1 ) {
						delete (*it);
						missle_list.erase( it );
						break;
					}
					else
						drawMissle( *it );
				}
				break;

			////////////////// ruch pociskow
			case STATE_WAIT:
				t_wait = t_wait + dt;
				break;
			



		}
		
	}

	glPopMatrix();



	// wyswietlenie tylniego ekranu
	glutSwapBuffers(); 
	
	// wymuszenie przerysowania podczas nastêpnej synchronizacji ekranu
	glutPostRedisplay(); 


}


///////////////////////////////////////// rysowanie obiektu
void CGL::drawMissle( CMissle* cm ) {

	

	setMaterial( MATERIAL_WALL_RED );

	glPushMatrix();
	glTranslatef( cm->x, cm->y, 0.0f );
	glScalef( 0.01f, 0.01f, 0.01f );	

	//if( findState( STATE_COLLISIONS ) ) {
		//glGetFloatv( GL_MODELVIEW_MATRIX, collisionTarget.m );
		//collisionTarget.r = 0.025f;
	//}

	glutSolidSphere( 0.5, 10, 10 );
	glPopMatrix();

}


///////////////////////////////////////// rysowanie obiektu
void CGL::drawObject( void ) {

	//setMaterial( pmaterial );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TextureArray[5]);

	glPushMatrix();
	glTranslatef( posX, posY, 0.0f );
	glRotatef( rotation, 0.0f, 0.0f, 1.0f );
	glRotatef( side, 0.0f, 1.0f, 0.0f );
	glRotatef(grav, 1.0f, 0.0f, 0.0f);

	glScalef(15.0f / mscene_height*0.03f, 15.0f  / mscene_height*0.03f, 15.0f  / mscene_height*0.03f);

	if( findState( STATE_COLLISIONS ) ) {
		glGetFloatv( GL_MODELVIEW_MATRIX, collisionTarget.m );
		collisionTarget.r = 0.010f;
	}
	
	
	glCallList(al[0]);
	
	//glutSolidTeapot( 1 );
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

///////////////////////////////////////// rysowanie obiektow na scenie w oparciu o dane bitmapy
void CGL::drawScene( void ) {


	
	glEnable(GL_TEXTURE_2D);

	// Ustawienie sposobu teksturowania - GL_MODULATE sprawia, ¿e œwiat³o ma wp³yw na teksturê; GL_DECAL i GL_REPLACE rysuj¹ teksturê tak jak jest
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Ustawienie materia³u
	float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

	glBindTexture(GL_TEXTURE_2D, TextureArray[1]);
	glPushMatrix();
	glBegin(GL_QUADS);         // Rysujemy kwadraty
	
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexCoord2{ s, i, f, d }[v](S, T; *v);
	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-10.0f, 10.0f, -2.0f); // górny lewy

	glTexCoord2f(0.0f, 20.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(10.0f, 10.0f, -2.0f);         // górny prawy

	glTexCoord2f(20.0f, 20.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(10.0f, -10.0f, -2.0f);         // dolny prawy

	glTexCoord2f(20.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-10.0f, -10.0f, -2.0f);         // dolny lewy

	
	glEnd();         // Koniec rysowania kwadratu 

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	if( findState( STATE_COLLISIONS ) )
		collision_list.clear(); // skasowanie listy kolizji

	
	glPushMatrix();

	for( int c = 0; c < mscene_height; c++ ) {
		for( int w = 0; w < mscene_width; w++ ) {

			int idx = xy2idx( w, c );

			if( mscene[idx] == 255 && mscene[idx+1] == 255 && mscene[idx+2] == 255 ) { // biale elementy

				// kolor obiektu
				//setMaterial( MATERIAL_WALL_GRAY );
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
				float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

				glBindTexture(GL_TEXTURE_2D, TextureArray[1]);
				glPushMatrix();
				glTranslatef((float)w / (float)mscene_width - 0.0003f + 0.5f / (float)mscene_width-0.0003f , (float)c / (float)mscene_height - 0.0003f + 0.5f / (float)mscene_height - 0.0003f, 0.0f);
				glScalef(0.5f / (float)mscene_width, 0.5f / (float)mscene_height, 0.5f / (float)mscene_width - 0.0001f);





				CUTIL::drawBox();

				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			}

			if( mscene[idx] == 0 && mscene[idx+1] == 255 && mscene[idx+2] == 0 ) { // zielone elementy

				// kolor obiektu
				//setMaterial( MATERIAL_WALL_GREEN );
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
				float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

				glBindTexture(GL_TEXTURE_2D, TextureArray[0]);
				glPushMatrix();
				glTranslatef((float)w / (float)mscene_width - 0.0003f + 0.5f / (float)mscene_width, (float)c / (float)mscene_height - 0.0003f + 0.5f / (float)mscene_height, 0.0f);
				glScalef(0.5f / (float)mscene_width, 0.5f / (float)mscene_height, 0.5f / (float)mscene_width - 0.0001f);
				CUTIL::drawBox();

				if (findState(STATE_COLLISIONS)) {
					addCollision(0.005f);

				}

				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			}


			if( mscene[idx] == 0 && mscene[idx+1] == 0 && mscene[idx+2] == 255 ) { // czerwone elementy

				// kolor obiektu
				//setMaterial( MATERIAL_WALL_RED );
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
				float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

				glBindTexture(GL_TEXTURE_2D, TextureArray[3]);
				glPushMatrix();
				glTranslatef((float)w / (float)mscene_width - 0.0005f + 0.5f / (float)mscene_width - 0.0005f, (float)c / (float)mscene_height - 0.0005f + 0.5f / (float)mscene_height - 0.0005f, 0.0f);
				glScalef( 0.5f/(float)mscene_width, 0.5f/(float)mscene_height, 0.5f/(float)mscene_width-0.0001f );			
				CUTIL::drawBox();

				if( findState(STATE_COLLISIONS) ) {
					addCollision( 0.005f );
				
				}
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			}
			if (mscene[idx] == 255 && mscene[idx + 1] == 0 && mscene[idx + 2] == 0) { // niebieskie elementy

				// kolor obiektu
				//setMaterial(MATERIAL_WALL_BLUE);
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
				float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

				glBindTexture(GL_TEXTURE_2D, TextureArray[4]);
				glPushMatrix();
				glTranslatef((float)w / (float)mscene_width - 0.0005f + 0.5f / (float)mscene_width - 0.0005f, (float)c / (float)mscene_height - 0.0005f + 0.5f / (float)mscene_height - 0.0005f, 0.0f);
				glScalef(0.5f / (float)mscene_width , 0.5f / (float)mscene_height, 0.5f / (float)mscene_width - 0.0001f);
				CUTIL::drawBox();
				if (findState(STATE_COLLISIONS)) {
					addCollision(0.0005f);
				}
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			}

			if (mscene[idx] == 0 && mscene[idx + 1] == 255 && mscene[idx + 2] == 255) { // ¿ó³te elementy

				// kolor obiektu
				//setMaterial(MATERIAL_WALL_YELLOW);
				glEnable(GL_TEXTURE_2D);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
				float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
				float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

				glBindTexture(GL_TEXTURE_2D, TextureArray[2]);
				glPushMatrix();
				glTranslatef((float)w / (float)mscene_width - 0.0005f + 0.5f / (float)mscene_width - 0.0005f, (float)c / (float)mscene_height - 0.0005f + 0.5f / (float)mscene_height - 0.0005f, 0.0f);
				glScalef(0.5f / (float)mscene_width, 0.5f / (float)mscene_height, 0.5f / (float)mscene_width - 0.0001f);
				CUTIL::drawBox();
				if (findState(STATE_COLLISIONS)) {
					addCollision(0.0005f);

					//if (mscene[idx] == SCENE_COLLISION && mscene[idx + 1] == SCENE_COLLISION && mscene[idx + 2] == SCENE_COLLISION) {
					//	mscene[idx] = 0;
					//	mscene[idx + 1] = 0;
					//	mscene[idx + 2] = 0;
					//}
				}
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);
			}

			//if (mscene[idx] == 0 && mscene[idx + 1] == 64 && mscene[idx + 2] == 128) { // brazowe elementy

			//	// kolor obiektu
			//	//setMaterial(MATERIAL_WALL_YELLOW);
			//	glEnable(GL_TEXTURE_2D);
			//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//	float m_amb[] = { 0.7f, 0.7f, 0.7f, 1.0f };
			//	float m_dif[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			//	float m_spe[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			//	glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
			//	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_dif);
			//	glMaterialfv(GL_FRONT, GL_SPECULAR, m_spe);

			//	glBindTexture(GL_TEXTURE_2D, TextureArray[6]);
			//	glPushMatrix();
			//	glTranslatef((float)w / (float)mscene_width - 0.0005f + 0.5f / (float)mscene_width - 0.0005f, (float)c / (float)mscene_height - 0.0005f + 0.5f / (float)mscene_height - 0.0005f, 0.0f);
			//	glScalef(0.5f / (float)mscene_width, 0.5f / (float)mscene_height, 0.5f / (float)mscene_width - 0.0001f);
			//	CUTIL::drawBox();
			//	
			//		if (findState(STATE_WAIT)) {
			//			if (t_wait > t_wait_time)
			//				state_list.remove(STATE_WAIT);
			//		}
			//		else {
			//			CMissle* cm = new CMissle();
			//			cm->x = posX;
			//			cm->y = posY;
			//			cm->dir = rotation;
			//			missle_list.push_back(cm);

			//			// kolejny pocisk bedzie mozna utworzyc po 100 [msec]
			//			t_wait = 0;
			//			t_wait_time = 1000;
			//			state_list.push_front(STATE_WAIT);
			//		}

			//	
			//	glPopMatrix();
			//	glDisable(GL_TEXTURE_2D);
			//}

		}
	}

	glPopMatrix();
	
}


///////////////////////////////////////// obsluga klawiatury

void CGL::keyUp( unsigned char key,int a,int b ) {
	
	keyStates[key] = false;	
}

void CGL::keyPress( unsigned char key,int a,int b ) {

	keyStates[key] = true;
}
	
void CGL::keyOperations( void ) {	
	
	if ( keyStates[27] ) {
		exit(0);
	}
	

		// obrot obiektu w lewo
		if (keyStates['q']) {
			if (!findState(STATE_ROTATE_OBJECT_LEFT)) {
				state_list.remove(STATE_ROTATE_OBJECT_RIGHT);
				state_list.push_front(STATE_ROTATE_OBJECT_LEFT);
			}
		}

		// obrot obiektu w prawo
		if (keyStates['e']) {
			if (!findState(STATE_ROTATE_OBJECT_RIGHT)) {
				state_list.remove(STATE_ROTATE_OBJECT_LEFT);
				state_list.push_front(STATE_ROTATE_OBJECT_RIGHT);
			}
		}

		// ruch obiektu do przodu
		if (keyStates['d']) {
			if (GO == 0){
				if (!findState(STATE_MOVE_OBJECT_FORWARD)) {
					state_list.push_front(STATE_MOVE_OBJECT_FORWARD);
				}
			}
			else{}
		}
		if (keyStates['a']) {
			if (GO == 0)
			{

				if (!findState(STATE_MOVE_OBJECT_BACKWARD)) {
					state_list.push_front(STATE_MOVE_OBJECT_BACKWARD);

				}
			}
			else{}
		}
		
		//STARE_GRAV_SWAP
		if (keyStates[' ']) {
			if (findState(STATE_WAIT)) {
				if (t_wait > t_wait_time)
				{
					state_list.remove(STATE_WAIT);
				}

			}
			else if (GO==0)
			{
				
				t_wait = 0;
				t_wait_time = 1000;
				PlaySound("172207__fins__teleport.wav", NULL, SND_ASYNC | SND_FILENAME | NULL);
				state_list.push_front(STATE_WAIT);
				grav = grav + 180;
				side = side*(-1);
				gravity = gravity* (-1);
				
			}
			else{}
			
		}
		if (keyStates['z']) {

			if (!findState(STARE_GRAV_SWAP)) {
				state_list.push_front(STARE_GRAV_SWAP);
				if (game_stage == 0)
				{
					mscene = CUTIL::LoadBitmapFile(SCENE_FILE_NAME, &bitmapInfoHeader);
				}
				else
				{
					mscene = CUTIL::LoadBitmapFile(SCENE_FILE_NAME2, &bitmapInfoHeader);
				}
			}
		}

//			if (findState(STATE_WAIT)) {
//				if (t_wait > t_wait_time)
//				{
//						state_list.remove(STATE_WAIT);
//				}
//
//			}
//
//			else if (!findState(STATE_MOVE_OBJECT_UP)) {
//				state_list.remove(STATE_FALL);
////				int timeInterval = currentTime - previousTime;
//				//{
//					state_list.push_front(STATE_MOVE_OBJECT_UP);
//				//}
//				jump = jump + 1;
//				t_wait = 0;
//				t_wait_time = 100;
//				state_list.push_front(STATE_WAIT);
//				if (jump == 2)
//				{
//
//					t_wait = 0;
//					t_wait_time = 900;
//					state_list.push_front(STATE_WAIT);
//				}
//			}
//
//		}
		else if (!findState(STATE_FALL)) {

			state_list.push_front(STATE_FALL);
		}

		
		// strzelanie
		if (keyStates['c']) {

			if (findState(STATE_WAIT)) {
				if (t_wait > t_wait_time)
					state_list.remove(STATE_WAIT);
			}
			else {
				CMissle* cm = new CMissle();
				cm->x = posX;
				cm->y = posY;
				cm->dir = rotation;
				missle_list.push_back(cm);

				// kolejny pocisk bedzie mozna utworzyc po 100 [msec]
				t_wait = 0;
				t_wait_time = 100;
				state_list.push_front(STATE_WAIT);
			}

		}
		
	
	

}
void CGL::getFPS()
{
	frameCount++;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if (timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);
		previousTime = currentTime;
		frameCount = 0;
	}
	cout << "\n\n" << fps;
}

void CGL::drawGUI()
{
	glColor3f(1.0, 1.0, 1.0);
	char a[100];
	sprintf(a, "Czas do nast. poziomu  %.2f", CUTIL::getTime());
	glRasterPos3f(posX, posY, 2.0f);
	for (int i = 0; a[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, a[i]);
	sprintf(a, "Poziom  %d", 1);
	glRasterPos3f(posX+1, posY+1, 2.0f);
	for (int i = 0; a[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, a[i]);

	sprintf(a, "Punkty  %d", coin);
	glRasterPos3f(2.3f, 2.8f, -1.0f);
	for (int i = 0; a[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, a[i]);
}







