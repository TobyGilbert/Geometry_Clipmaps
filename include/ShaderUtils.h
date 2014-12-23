#ifndef __SHADERUTILS_H_
#define __SHADERUTILS_H_


#include <iostream>
#include <GLFW/glfw3.h>
#include <fstream>
#include <streambuf>
#include <string>

class shaderUtils{
public:
   static GLuint createShaderFromFile(const GLchar* path, GLenum shaderType);
};


#endif
