#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <Box2D.h>
#include <stdio.h>
#include <list>
#include "script.h"
#include "game.h"
#include "input.h"
#include "player.h"
#include <SDL.h>
#include "ploy/symbol.h"


using namespace std;

symbol_table* g_symbol_table;

GLvoid DrawScene();
void OnTimer(int value);

b2World* PhysicsWorld;

PlayerInput* LocalPlayerInput;
PlayerInfo* MainPlayer;

Vector CameraFocusLocation;
float CameraAspect;
float CameraWidth;
float CameraApproach;
float FluidRadius;
float FluidMass;
float FluidFriction;

unsigned int TickCount;
unsigned int TickAccum;

void DestroyPhysics()
{
	delete PhysicsWorld;
	PhysicsWorld = NULL;
}

#undef main // SDL redifines this as SDL_main, we want to use our own.

int main(int argc, char* argv[])
{
	g_symbol_table = init_symbol_table();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Surface Tension\n");

	SDL_Init(SDL_INIT_JOYSTICK);
	

	glShadeModel(GL_SMOOTH);
	glClearColor(0.35f, 0.94f, 1.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glutDisplayFunc(DrawScene);
	//glutTimerFunc(20, OnTimer, 1);

	InitScheme();
	scheme_load_file_name(scheme_vm, "player.scm");
	scheme_load_file_name(scheme_vm, "Entry.scm");

	LocalPlayerInput = InitPlayerInput();

	//InitPhysics();

	// Doing this as close to the main loop as possible to not get alot of init mixed in with my first tick.
	TickCount = SDL_GetTicks();
	TickAccum = 0;
	glutMainLoop();

	DestroyPlayer(MainPlayer);
	DestroyPlayerInput(LocalPlayerInput);

	if(PhysicsWorld)
		DestroyPhysics();

	scheme_deinit(scheme_vm);

	SDL_Quit();

	destroy_symbol_table(g_symbol_table);
}

GLvoid DrawScene()
{
	int NewTick = SDL_GetTicks();
	TickAccum += NewTick - TickCount;
	
	TickCount = NewTick;

	float DeltaTime = 0.0f; // Box2D assumes fixed timestep, lock it?

	if(TickAccum >= 20)
		DeltaTime = 0.02f;
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	Vector DeltaCamera = MainPlayer->State.Location - CameraFocusLocation;
	DeltaCamera *= DeltaCamera.Length()*CameraApproach/CameraWidth;
	CameraFocusLocation += DeltaCamera;

	float CameraHeight = CameraWidth*CameraAspect;
	glOrtho(CameraFocusLocation.x-(CameraWidth/2.0f), CameraFocusLocation.x+(CameraWidth/2.0f), CameraFocusLocation.y-(CameraHeight/2.0f), CameraFocusLocation.y+(CameraHeight/2.0f), 0.0f, 2.0f);
	//glScalef(1.0f/CameraWidth, 1.0f/(CameraWidth*CameraAspect), 1.0);
	//gluOrtho2D(0, CameraWidth, CameraWidth*CameraAspect, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//RenderPhysics
	// I'd perfer to not interlace this with rendering, but I don't want to redo Physics debug rendering yet.

	while(TickAccum >= 20)
	{
		UpdateInput(LocalPlayerInput);
		UpdatePlayerInput(MainPlayer, LocalPlayerInput);
		UpdatePlayer(DeltaTime, MainPlayer);
		PhysicsWorld->Step(DeltaTime, 10);
		TickAccum -= 20;
	}
	if(DeltaTime == 0.0f)
		PhysicsWorld->Step(DeltaTime, 10);


	RenderPlayer(MainPlayer);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSwapBuffers();
	glutPostRedisplay();
}

void OnTimer(int)
{

	glutTimerFunc(20, OnTimer, 1);
}
