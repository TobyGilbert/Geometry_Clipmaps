#ifndef __WATER_H_
#define __WATER_H_

#include <glm/glm.hpp>
#include "Model.h"
#include "ShaderProgram.h"
class Water {
public:
    Water();
    ~Water();
    void init();
    void loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);
    void render();
private:
    void createShader();
    Model *m_model;
    ShaderProgram *m_shaderProgram;
    Shader *m_vertShader;
    Shader *m_fragShader;
};

#endif
