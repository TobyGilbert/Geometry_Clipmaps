#ifndef __WATER_H_
#define __WATER_H_

#include <glm/glm.hpp>
#include "Model.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include <QTime>
class Water {
public:
    Water();
    ~Water();
    void init();
    void loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);
    void render();
    GLuint* getReflectTex(){return &m_reflectTex;}
    GLuint* getRefractTex(){return &m_refractTex;}
private:
    void createShader();
    Model *m_model;
    ShaderProgram *m_shaderProgram;
    Shader *m_vertShader;
    Shader *m_fragShader;
    GLuint m_reflectTex;
    GLuint m_refractTex;
    Texture *m_normal;
    Texture *m_dudv;
    QTime m_timer;
    GLuint m_timeLoc;
};

#endif
