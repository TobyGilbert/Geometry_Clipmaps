#include "Water.h"
#include <glm/gtc/type_ptr.hpp>

Water::Water(){
    createShader();
    init();
}

Water::~Water(){
    delete m_shaderProgram;
    delete m_model;
    delete m_dudv;
    delete m_normal;
}

void Water::createShader(){
    m_shaderProgram = new ShaderProgram();
    m_vertShader = new Shader("shaders/ripplesVert.glsl", GL_VERTEX_SHADER);
    m_fragShader = new Shader("shaders/ripplesFrag.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram->attachShader(m_vertShader);
    m_shaderProgram->attachShader(m_fragShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColour");
    m_shaderProgram->link();
    m_shaderProgram->use();

    delete m_vertShader;
    delete m_fragShader;

    // Create texture object
    glGenTextures(1, &m_reflectTex);
    glBindTexture(GL_TEXTURE_2D, m_reflectTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &m_refractTex);
    glBindTexture(GL_TEXTURE_2D, m_refractTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_dudv = new Texture("textures/water_dudv.jpg");
    m_normal = new Texture("textures/water_normal.jpg");

    m_timeLoc = m_shaderProgram->getUniformLoc("uniTime");

    GLuint fogMinLoc = m_shaderProgram->getUniformLoc("fogMin");
    GLuint fogMaxLoc = m_shaderProgram->getUniformLoc("fogMax");
    GLuint reflectLoc = m_shaderProgram->getUniformLoc("reflectMap");
    GLuint refractLoc = m_shaderProgram->getUniformLoc("refractMap");
    GLuint normalLoc = m_shaderProgram->getUniformLoc("normalMap");
    GLuint dudvLoc = m_shaderProgram->getUniformLoc("dudv");
    GLuint viewPosLoc = m_shaderProgram->getUniformLoc("viewPos");

    glUniform1f(fogMaxLoc, 60.0);
    glUniform1f(fogMinLoc, 30.0);
    glUniform1i(reflectLoc, 0);
    glUniform1i(refractLoc, 1);
    glUniform1i(normalLoc, 2);
    glUniform1i(dudvLoc, 3);
    glUniform3f(viewPosLoc, 0.0 ,10.0, 10.0);


}

void Water::init(){
    m_model = new Model("models/water.obj");

    m_timer.start();
}

void Water::loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix){
    m_shaderProgram->use();
    GLuint modelViewProjectionMatrixLoc = m_shaderProgram->getUniformLoc("MVP");
    GLuint modelViewMatrixLoc = m_shaderProgram->getUniformLoc("MV");

    glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;
    glUniformMatrix4fv(modelViewProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
    glm::mat4 modelViewMatrix =_viewMatrix * _modelMatrix;
    glUniformMatrix4fv(modelViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

}

void Water::render(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_reflectTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_refractTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_normal->getTextureID());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_dudv->getTextureID());

    glUniform1f(m_timeLoc, m_timer.elapsed());

    glBindVertexArray(m_model->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_model->getNumVerts());
    glBindVertexArray(0);
}
