#include "glrender.h"
#include "coresystems.h"
#include "systemmanager.h"
#include <OpenGL/gl.h>
#include "2dtypes.h"
#include "player.h"
#include "camera.h"

struct render_subsystem
{
	render_system* system;
	subsystem_render render_func;
	render_subsystem* next;
	
};

struct gl_render
{
	render_subsystem* subsystem_list;
	render_subsystem** subsystem_next;
    Camera2D* view_camera;
};

void* init_gl_render_system(SystemManager* SM)
{
	gl_render* renderer = new gl_render();
	renderer->subsystem_list = NULL;
	renderer->subsystem_next = &renderer->subsystem_list;
	glut_system* glut = (glut_system*)system_manager_require(SM, "GLUT");
	glut_system_set_renderer(glut, renderer);
    renderer->view_camera = (Camera2D*)system_manager_require(SM, "camera");

	glShadeModel(GL_SMOOTH);
	glClearColor(0.35f, 0.94f, 1.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	return renderer;
}

void destroy_gl_render_system(void* System)
{
	delete (gl_render*)System;
}

void gl_render_do_render(gl_render* renderer)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Camera System.
	Vector DeltaCamera = MainPlayer->State.Location - renderer->view_camera->FocusLocation;
	DeltaCamera *= DeltaCamera.Length()*renderer->view_camera->Approach/renderer->view_camera->Width;
	renderer->view_camera->FocusLocation += DeltaCamera;

	float CameraHeight = renderer->view_camera->Width*renderer->view_camera->Aspect;
	glOrtho(renderer->view_camera->FocusLocation.x-(renderer->view_camera->Width/2.0f), renderer->view_camera->FocusLocation.x+(renderer->view_camera->Width/2.0f), renderer->view_camera->FocusLocation.y-(CameraHeight/2.0f), renderer->view_camera->FocusLocation.y+(CameraHeight/2.0f), 0.0f, 2.0f);
	glScalef(1.0f/renderer->view_camera->Width, 1.0f/(renderer->view_camera->Width*renderer->view_camera->Aspect), 1.0);
	glOrtho(0, renderer->view_camera->Width, renderer->view_camera->Width*renderer->view_camera->Aspect, 0, -1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glOrtho(0, 255, 255, 0, -1, 1);

	render_subsystem* curr_render_system = renderer->subsystem_list;
	while(curr_render_system)
	{
		curr_render_system->render_func(renderer, curr_render_system->system);
		curr_render_system = curr_render_system->next;
	}

	// Do render of registered renderables.
	RenderPlayer(MainPlayer);

	glColor3f(1.0f, 0.0f, 0.0f);
}

void declare_gl_render_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "glrender"), init_gl_render_system, destroy_gl_render_system);
}

void gl_render_register_render_system(gl_render* renderer, render_system* system, subsystem_render render_func)
{
	render_subsystem* new_link = new render_subsystem();
	new_link->system = system;
	new_link->render_func = render_func;
	*renderer->subsystem_next = new_link;
	renderer->subsystem_next = &new_link->next;
}
