#include "MesoTerrain.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

MesoTerrain::MesoTerrain(){
    createShader();
    init();
}

MesoTerrain::~MesoTerrain(){
    delete m_model;
    delete m_shaderProgram;
}

void MesoTerrain::createShader(){
    m_shaderProgram = new ShaderProgram();
    m_vertShader = new Shader("shaders/PhongVert.glsl", GL_VERTEX_SHADER);
    m_fragShader = new Shader("shaders/PhongFrag.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram->attachShader(m_vertShader);
    m_shaderProgram->attachShader(m_fragShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColour");
    m_shaderProgram->link();
    m_shaderProgram->use();

    delete m_vertShader;
    delete m_fragShader;

    GLuint lightPosLoc = m_shaderProgram->getUniformLoc("light.position");
    GLuint lightIntLoc = m_shaderProgram->getUniformLoc("light.intensity");
    GLuint KdLoc = m_shaderProgram->getUniformLoc("Kd");
    GLuint KaLoc = m_shaderProgram->getUniformLoc("Ka");
    GLuint KsLoc = m_shaderProgram->getUniformLoc("Ks");
    GLuint shininessLoc = m_shaderProgram->getUniformLoc("shininess");

    glUniform4f(lightPosLoc, 0, 1.0, 5.0, 1.0);
    glUniform3f(lightIntLoc, 0.5, 0.5, 0.5);
    glUniform3f(KdLoc, 0.5, 0.5, 0.5);
    glUniform3f(KaLoc, 0.5, 0.5, 0.5);
    glUniform3f(KsLoc, 0.5, 0.5, 0.5);
    glUniform1f(shininessLoc, 10.0);
}

void MesoTerrain::init(){
    m_model = new Model("models/mesoTerrain.obj");
}

void MesoTerrain::loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix){
    m_shaderProgram->use();

    GLuint modelViewLoc = m_shaderProgram->getUniformLoc("modelViewMatrix");
    GLuint normalLoc = m_shaderProgram->getUniformLoc("normalMatrix");
    GLuint projectionLoc = m_shaderProgram->getUniformLoc("projectionMatrix");
    GLuint MVPLoc = m_shaderProgram->getUniformLoc("modelViewProjectionMatrix");

    glm::mat4 modelView = _viewMatrix*_modelMatrix;
    glm::mat3 normal = glm::inverseTranspose(glm::mat3(modelView));
    glm::mat4 MVP = _projectionMatrix * modelView;

    glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix3fv(normalLoc, 1, GL_FALSE, glm::value_ptr(normal));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));
}

void MesoTerrain::render(){
    glBindVertexArray(m_model->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_model->getNumVerts());
    glBindVertexArray(0);
}


