#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "2dtypes.h"
class SystemManager;

struct Camera2D
{
    Vector FocusLocation;
    float Width;
    float Approach;
    float Aspect;
};

void declare_2D_camera_system(SystemManager* SM);

#endif //_CAMERA_H_
