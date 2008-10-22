#ifndef _CORESYSTEMS_H_
#define _CORE_SYSTEMS_H_

struct symbol_table;
struct SystemManager;
struct glut_system;

void declare_sdl_system(SystemManager* SM);
void declare_tick_system(SystemManager* SM);
void declare_glut_system(SystemManager* SM);
void set_glut_system_run(SystemManager* SM);

void glut_system_set_renderer(glut_system* glut, struct gl_render* renderer);

#endif //_CORE_SYSTEMS_H_
