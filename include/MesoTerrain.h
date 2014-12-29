#ifndef __MESOTERRAIN_H_
#define __MESOTERRAIN_H_

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Model.h"

class MesoTerrain{
public:
    MesoTerrain();
    ~MesoTerrain();
    void init();
    void loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);
    void render();
private:
    void createShader();
    ShaderProgram *m_shaderProgram;
    Shader *m_vertShader;
    Shader *m_fragShader;
    Model *m_model;
};

#endif
