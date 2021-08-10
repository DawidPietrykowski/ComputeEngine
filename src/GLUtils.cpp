#pragma once
#include "GLUtils.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sys/stat.h>

using std::cout;
using std::endl;

bool glErrorCheck() {
    bool no_err = true;

    GLenum error = glGetError();
    while (error) {
        std::string err_str;
        switch (error) {
        case GL_NO_ERROR:
            err_str = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            err_str = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            err_str = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            err_str = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err_str = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            err_str = "GL_OUT_OF_MEMORY";
            break;
        default:
            err_str = "GL_UNKNOWN_ERROR";
            break;
        }

        cout << err_str << endl;

        error = glGetError();

        no_err = false;
        return no_err;
    }

    return no_err;
}

void glClearError() {
    while (glGetError() != GL_NO_ERROR);
}

void PrintSizeLimits() {
    GLint size;
    GLint size2;

    glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &size);
    std::cout << "GL_MAX_TEXTURE_BUFFER_SIZE is " << ((float)size * 32.0f / 1073741824.0f) << " GB" << endl;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &size);
    std::cout << "GL_MAX_SHADER_STORAGE_BLOCK_SIZE is " << ((float)size / 1073741824.0f) << " GB" << endl;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &size);
    std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE is " << ((float)size / 1024.0f) << " KB" << endl;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
    std::cout << "GL_MAX_TEXTURE_SIZE is " << ((float)size / 1024.0f) << " MB" << endl;
    glGetIntegerv(GL_MAX_IMAGE_UNITS, &size);
    std::cout << "GL_MAX_IMAGE_UNITS is " << size << endl;
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &size);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &size2);
    std::cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE  is " << size << ", " << size2 << endl;
}