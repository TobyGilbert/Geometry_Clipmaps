#ifndef __SHADER_H_
#define __SHADER_H_

#include <iostream>
#include <GLFW/glfw3.h>
#include <IL/il.h>
#include <glm/glm.hpp>
#include <string>

class Shader{
public:
   Shader(std::string _path, GLenum _type);
   virtual ~Shader();
   GLuint getShaderID();
private:
   GLuint m_shaderID;
};

#endif
