#ifndef _GLRENDER_H_
#define _GLRENDER_H_

struct gl_render;

void declare_gl_render_system(struct SystemManager* SM);
void gl_render_do_render(gl_render* renderer);

#endif //_GLRENDER_H_
