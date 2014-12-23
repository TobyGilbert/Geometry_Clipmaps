#ifndef __TEXTUREUTILS_H_
#define __TEXTUREUTILS_H_


#include <iostream>
#include <GLFW/glfw3.h>
#include <IL/il.h>

class TextureUtils{
public:
   static GLuint createTexture(const GLchar* path);
};

#endif
