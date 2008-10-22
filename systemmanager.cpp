#include "systemmanager.h"
#include <assert.h>

struct GameSystem
{
	symbol Name;
	init_func Init;
	destroy_func Destroy;
	void* SystemPointer;
	GameSystem* NextSystem;
};

struct SystemManager
{
	GameSystem* FirstSystem;
	GameSystem** LastSystemSlot;
	GameSystem* RunSystem;
	run_func RunFunc;
	symbol_table* symbols;
};

void declare_system(SystemManager* SM, symbol name, init_func init, destroy_func destroy)
{
	GameSystem* System = new GameSystem;
	
	System->Name = name;
	System->Init = init;
	System->Destroy = destroy;
	System->SystemPointer = NULL;
	System->NextSystem = NULL;

	*SM->LastSystemSlot = System;
	SM->LastSystemSlot = &System->NextSystem;
}

void set_run_system(SystemManager* SM, symbol name, run_func run)
{
	GameSystem* System = SM->FirstSystem;
	while(System)
	{
		if(System->Name == name)
		{
			SM->RunSystem = System;
			SM->RunFunc = run;
			return;
		}
		System = System->NextSystem;
	}
}

void system_manager_run(SystemManager* SM)
{
	SM->RunFunc(SM->RunSystem->SystemPointer);
}

SystemManager* create_system_manager(symbol_table* tbl)
{
	SystemManager* SM = new SystemManager();
	SM->FirstSystem = NULL;
	SM->LastSystemSlot = &SM->FirstSystem;
	SM->RunSystem = NULL;
	SM->symbols = tbl;
	return SM;
}

// Doesn't destroy the systems.
void destroy_system_manager(SystemManager* SM)
{
	GameSystem* System = SM->FirstSystem;
	while(System)
	{
		GameSystem* Tmp = System->NextSystem;
		delete System;
		System = Tmp;
	}
	delete SM;
}

void system_manager_init_systems(SystemManager* SM)
{
	GameSystem* System = SM->FirstSystem;
	while(System)
	{
		System->SystemPointer = System->Init(SM);
		System = System->NextSystem;
	}
}

void system_manager_shutdown_systems(SystemManager* SM)
{
	GameSystem* System = SM->FirstSystem;
	while(System)
	{
		System->Destroy(System->SystemPointer);
		System->SystemPointer = NULL;
		System = System->NextSystem;
	}
}

GameSystem* system_manager_find(SystemManager* SM, const char* Sys)
{
	GameSystem* System = SM->FirstSystem;
	symbol system_symbol = symbol_from_string(SM->symbols, Sys);
	while(System)
	{
		if(System->Name == system_symbol)
			return System;
		System = System->NextSystem;
	}
	return NULL;
}

// This needs to ensure it's been initialized already
void* system_manager_require(SystemManager* SM, const char* Sys)
{
	GameSystem* System = system_manager_find(SM, Sys);
	assert(System);
	return System->SystemPointer;
}

void* system_manager_request(SystemManager* SM, const char* Sys)
{
	GameSystem* System = system_manager_find(SM, Sys);
	if(System)
		return System->SystemPointer;
	return NULL;
}

symbol_table* system_manager_get_symbol_table(SystemManager* SM)
{
	return SM->symbols;
}

symbol system_manager_symbol(SystemManager* SM, const char* str)
{
	return symbol_from_string(SM->symbols, str);
}
