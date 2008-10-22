#include "glrender.h"
#include "coresystems.h"
#include "systemmanager.h"
#include <OpenGL/gl.h>

struct gl_render
{
};

void* init_gl_render_system(SystemManager* SM)
{
	gl_render* renderer = new gl_render();
	glut_system* glut = (glut_system*)system_manager_require(SM, "GLUT");
	glut_system_set_renderer(glut, renderer);

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
	//Vector DeltaCamera = MainPlayer->State.Location - CameraFocusLocation;
	//DeltaCamera *= DeltaCamera.Length()*CameraApproach/CameraWidth;
	//CameraFocusLocation += DeltaCamera;

	//float CameraHeight = CameraWidth*CameraAspect;
	//glOrtho(CameraFocusLocation.x-(CameraWidth/2.0f), CameraFocusLocation.x+(CameraWidth/2.0f), CameraFocusLocation.y-(CameraHeight/2.0f), CameraFocusLocation.y+(CameraHeight/2.0f), 0.0f, 2.0f);
	//glScalef(1.0f/CameraWidth, 1.0f/(CameraWidth*CameraAspect), 1.0);
	//gluOrtho2D(0, CameraWidth, CameraWidth*CameraAspect, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Do render of registered renderables.
	//RenderPlayer(MainPlayer);

	glColor3f(1.0f, 0.0f, 0.0f);
}

void declare_gl_render_system(SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "render"), init_gl_render_system, destroy_gl_render_system);
}
