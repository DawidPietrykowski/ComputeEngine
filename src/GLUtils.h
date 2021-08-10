#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define ASSERT(x) if(!(x)){ std::cout << "[ERROR] " << #x << '\n';\
__debugbreak();}

#define GLCall(x) \
    glClearError();\
    x;\
    if(!(glErrorCheck())){ std::cout << "[ERROR] " << #x << '\n';\
    __debugbreak();}

#if 1
#define LOG(msg) \
    std::cout << '[' << __TIME__ << "] Renderer: " << msg << std::endl
#else
#define LOG(msg) \
    std::cout << __FILENAME__ << "(" << __LINE__ << "): " << msg << std::endl
#endif

void PrintSizeLimits();
bool glErrorCheck();
void glClearError();