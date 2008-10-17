#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_

#include "ploy/symbol.h"

struct GameSystem;
struct SystemManager;

typedef void* (*init_func)(SystemManager*);
typedef void (*destroy_func)(void*);
typedef void (*run_func)(GameSystem*);


void declare_system(SystemManager* SM, symbol name, init_func init, destroy_func destroy);
void set_run_system(SystemManager* SM, symbol name, run_func run);
void system_manager_run(SystemManager* SM);
SystemManager* create_system_manager();
void destroy_system_manager(SystemManager* SM);
void system_manager_init_systems(SystemManager* SM);
void system_manager_shutdown_systems(SystemManager* SM);

#endif //_SYSTEMMANAGER_H_
