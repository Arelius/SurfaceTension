#ifndef _GLRENDER_H_
#define _GLRENDER_H_

struct gl_render;

typedef void render_system;
typedef void (*subsystem_render)(gl_render* renderer, render_system* system);

void declare_gl_render_system(struct SystemManager* SM);
void gl_render_do_render(gl_render* renderer);
void gl_render_register_render_system(gl_render* renderer, render_system* system, subsystem_render render_func);

#endif //_GLRENDER_H_
