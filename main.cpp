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
#include "systemmanager.h"
#include "coresystems.h"
#include "glrender.h"
#include "camera.h"
#include "vrender.h"


using namespace std;

symbol_table* g_symbol_table;

GLvoid DrawScene();
void OnTimer(int value);

void DestroyPhysics()
{
	delete PhysicsWorld;
	PhysicsWorld = NULL;
}

#undef main // SDL redifines this as SDL_main, we want to use our own.

int main(int argc, char* argv[])
{
	g_symbol_table = init_symbol_table();
	SystemManager* SM = create_system_manager(g_symbol_table);
	declare_sdl_system(SM);
	declare_tick_system(SM);
	declare_glut_system(SM);
	declare_2D_camera_system(SM);
	declare_gl_render_system(SM);
	declare_vector_render_system(SM);
	declare_script_system(SM);
	set_glut_system_run(SM);
	
	system_manager_init_systems(SM);

	scheme_load_file_name(scheme_vm, "player.scm");
	scheme_load_file_name(scheme_vm, "Entry.scm");

	LocalPlayerInput = InitPlayerInput();
	system_manager_run(SM);

	//InitPhysics();

	// Doing this as close to the main loop as possible to not get alot of init mixed in with my first tick.


	DestroyPlayer(MainPlayer);
	DestroyPlayerInput(LocalPlayerInput);

	if(PhysicsWorld)
		DestroyPhysics();

	system_manager_shutdown_systems(SM);
	destroy_system_manager(SM);


	destroy_symbol_table(g_symbol_table);
}

GLvoid DrawScene()
{

}

void OnTimer(int)
{
	glutTimerFunc(20, OnTimer, 1);
}
