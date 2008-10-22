#include "coresystems.h"
#include "systemmanager.h"
#include "ploy/symbol.h"
#include <SDL.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "glrender.h"
#include <assert.h>

struct sdl_system
{
};

void* init_sdl_system(SystemManager* SM)
{
	SDL_Init(SDL_INIT_JOYSTICK);
	return new sdl_system();
}

void destroy_sdl_system(void* System)
{
	SDL_Quit();
	delete (sdl_system*)System;
}

void declare_sdl_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "SDL"), init_sdl_system, destroy_sdl_system);
}

struct tick_system
{
	unsigned int TickCount;
	unsigned int TickAccum;
};

void* init_tick_system(SystemManager* SM)
{
	system_manager_require(SM, "SDL");
	tick_system* Ticker = new tick_system();
	Ticker->TickCount = SDL_GetTicks();
	Ticker->TickAccum = 0;

	return Ticker;
}

void destroy_tick_system(void* System)
{
	delete (tick_system*)System;
}

void tick_system_tick(tick_system* System)
{
	int NewTick = SDL_GetTicks();
	System->TickAccum += NewTick - System->TickCount;
	
	System->TickCount = NewTick;

	float DeltaTime = 0.0f; // Box2D assumes fixed timestep, lock it?

	if(System->TickAccum >= 20)
		DeltaTime = 0.02f;

	while(System->TickAccum >= 20)
	{
		// Run All Ticks
		//UpdateInput(LocalPlayerInput);
		//UpdatePlayerInput(MainPlayer, LocalPlayerInput);
		//UpdatePlayer(DeltaTime, MainPlayer);
		//PhysicsWorld->Step(DeltaTime, 10);
		System->TickAccum -= 20;
	}
	if(DeltaTime == 0.0f)
	{
		// Run Needed Ticks (Should only be Box2d's debug rendering, should be able to remove.
		//PhysicsWorld->Step(DeltaTime, 10);
	}
}

void declare_tick_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "ticker"), init_tick_system, destroy_tick_system);
}

struct glut_system
{
	tick_system* Ticker;
	gl_render* renderer;
};

// GLUT doesn't want us to pass a user pointer into the draw func, so we use a global
glut_system* GLUT_sys;

void* init_glut_system(SystemManager* SM)
{
	GLUT_sys  = new glut_system;
	GLUT_sys->Ticker = (tick_system*)system_manager_require(SM, "ticker");
	GLUT_sys->renderer = NULL;
	// Need way to get these in.
	int argc = 0;
	char** argv = NULL;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Surface Tension\n");

	return GLUT_sys;
}

void destroy_glut_system(void* System)
{

	delete (glut_system*)System;
}

void glut_system_set_renderer(glut_system* glut, gl_render* renderer)
{
	glut->renderer = renderer;
}

GLvoid glut_system_draw_func()
{
	assert(GLUT_sys->renderer);
	assert(GLUT_sys->Ticker);

	tick_system_tick(GLUT_sys->Ticker);

	gl_render_do_render(GLUT_sys->renderer);
	glutSwapBuffers();
	glutPostRedisplay();

}

void glut_system_run(void* System)
{
	
	glutDisplayFunc(glut_system_draw_func);
	glutMainLoop();

}

void declare_glut_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "GLUT"), init_glut_system, destroy_glut_system);
}

void set_glut_system_run(SystemManager* SM)
{
	set_run_system(SM, system_manager_symbol(SM, "GLUT"), glut_system_run);
}
