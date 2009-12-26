#include "camera.h"
#include "systemmanager.h"

void* init_2D_camera_system(SystemManager* SM)
{
    Camera2D* Camera = new Camera2D();

    return Camera;
}

void destroy_2D_camera_system(void* System)
{
    delete (Camera2D*)System;
}

void declare_2D_camera_system(SystemManager* SM)
{
    declare_system(SM, system_manager_symbol(SM, "camera"), init_2D_camera_system, destroy_2D_camera_system);
}
