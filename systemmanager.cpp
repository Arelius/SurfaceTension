#include "systemmanager.h"

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
			return;
		}
		System = System->NextSystem;
	}
}

void system_manager_run(SystemManager* SM)
{
	SM->RunFunc(SM->RunSystem);
}

SystemManager* create_system_manager()
{
	SystemManager* SM = new SystemManager();
	SM->FirstSystem = NULL;
	SM->LastSystemSlot = &SM->FirstSystem;
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
