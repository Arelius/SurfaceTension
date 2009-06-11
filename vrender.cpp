#include "vrender.h"
#include "glrender.h"
#include "systemmanager.h"

struct vector_render
{
};

void vector_render_render(gl_render* renderer, render_system* system)
{
	vector_render* vrenderer = (vector_render*)system;
}

void* init_vector_render_system(SystemManager* SM)
{
	vector_render* vrenderer = new vector_render();
	gl_render* renderer = (gl_render*)system_manager_require(SM, "glrender");
	
	gl_render_register_render_system(renderer, vrenderer,  vector_render_render);
	
	return vrenderer;
}

void destroy_vector_render_system(void* System)
{
	delete (vector_render*)System;
}

void declare_vector_render_system(struct SystemManager* SM)
{
	declare_system(SM, system_manager_symbol(SM, "vector_render"), init_vector_render_system, destroy_vector_render_system);
}
