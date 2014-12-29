#include "Water.h"
#include <glm/gtc/type_ptr.hpp>

Water::Water(){
    createShader();
    init();
}

Water::~Water(){
    delete m_shaderProgram;
    delete m_model;
}

void Water::createShader(){
    m_shaderProgram = new ShaderProgram();
    m_vertShader = new Shader("shaders/waterVert.glsl", GL_VERTEX_SHADER);
    m_fragShader = new Shader("shaders/waterFrag.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram->attachShader(m_vertShader);
    m_shaderProgram->attachShader(m_fragShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColour");
    m_shaderProgram->link();
    m_shaderProgram->use();

    delete m_vertShader;
    delete m_fragShader;

    GLuint fogMinLoc = m_shaderProgram->getUniformLoc("fogMin");
    GLuint fogMaxLoc = m_shaderProgram->getUniformLoc("fogMax");

    glUniform1f(fogMinLoc, 3.0);
    glUniform1f(fogMaxLoc, 4.0);

}

void Water::init(){
    m_model = new Model("models/water.obj");
}

void Water::loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix){
    m_shaderProgram->use();
    GLuint modelViewProjectionMatrixLoc = m_shaderProgram->getUniformLoc("MVP");
    GLuint modelViewMatrixLoc = m_shaderProgram->getUniformLoc("modelViewMatrix");

    glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
    glUniformMatrix4fv(modelViewProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
    glm::mat4 modelViewMatrix =_viewMatrix * _modelMatrix;
    glUniformMatrix4fv(modelViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

}

void Water::render(){
    glBindVertexArray(m_model->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_model->getNumVerts());
    glBindVertexArray(0);
}
