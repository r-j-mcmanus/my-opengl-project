#include "renderer.h"
#include <iostream>


/*
Clears the GL error buffer
*/
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/*
CleLogs any errors in the GL error buffer
*/
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
        return false;
    }
    return true;
}