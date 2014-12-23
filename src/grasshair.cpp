#include "grasshair.h"

//to do
//fix shader

grassHair::grassHair()
{
    m_grassSize = 1.0;
    m_maxHeight = 1.0;
    m_minHeight = 0.0;
    m_maxAngle = 90.0;
    createShader();
}
//----------------------------------------------------------------------------------------------------------------------
grassHair::grassHair(GLuint _VAO){
    m_VAO = _VAO;
    m_grassSize = 1.0;
    m_maxHeight = 1.0;
    m_minHeight = 0.0;
    m_maxAngle = 90.0;
    createShader();
}
//----------------------------------------------------------------------------------------------------------------------
void grassHair::createShader(){
    //create our shader program
    m_shaderProgram = new ShaderProgram();
    Shader *vert = new Shader("shaders/grassHairVert.glsl", GL_VERTEX_SHADER);
    Shader *geom = new Shader("shaders/grassHairGeom.glsl", GL_GEOMETRY_SHADER);
    Shader *frag = new Shader("shaders/grassHairFrag.glsl", GL_FRAGMENT_SHADER);

    //attach and link our shaders to our program
    m_shaderProgram->attachShader(vert);
    m_shaderProgram->attachShader(geom);
    m_shaderProgram->attachShader(frag);
    m_shaderProgram->link();
    m_shaderProgram->use();


    //get rid of the junk
    delete vert;
    delete geom;
    delete frag;

    glEnable(GL_DEPTH_TEST); // for removal of hidden surfaces

    //set up our light

    GLuint lightPosGrassLoc = m_shaderProgram->getUniformLoc("light.position");
    GLuint lightIntGrassLoc = m_shaderProgram->getUniformLoc("light.intensity");
    GLuint kdGrassLoc = m_shaderProgram->getUniformLoc("Kd");
    GLuint kaGrassLoc = m_shaderProgram->getUniformLoc("Ka");
    GLuint ksGrassLoc = m_shaderProgram->getUniformLoc("Ks");
    GLuint shininessGrassLoc = m_shaderProgram->getUniformLoc("shininess");

    glm::vec4 lightPositon(1.0, 1.0, 1.0, 1.0);
    glm::vec3 lightIntensity(0.8, 0.8, 0.8);
    glUniform4f(lightPosGrassLoc, lightPositon.x, lightPositon.y, lightPositon.z, lightPositon.w);
    glUniform3f(lightIntGrassLoc, lightIntensity.x, lightIntensity.y, lightIntensity.z);
    glUniform3f(kdGrassLoc, 0.5, 0.5, 0.5);
    glUniform3f(kaGrassLoc, 0.5, 0.5, 0.5);
    glUniform3f(ksGrassLoc, 0.5, 0.5, 0.5);
    glUniform1f(shininessGrassLoc, 100.0);

    //set our grass size
    GLuint grassSizeLoc = m_shaderProgram->getUniformLoc("grassSize");
    glUniform1f(grassSizeLoc,m_grassSize);

    //set up our grass texture
    m_grassTexture = new Texture("textures/grassBlade.png");
    m_grassTexture->bind(10);
    GLuint grassBladeTexLoc = m_shaderProgram->getUniformLoc("grassTexture");
    glUniform1i(grassBladeTexLoc, 10);

}

//----------------------------------------------------------------------------------------------------------------------
void grassHair::draw(glm::mat4 _modelMatrix, Camera *_cam, int _numPoints){
    loadMatricesToShader(_modelMatrix,_cam);

//    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, _numPoints);
    glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------------------------------------
void grassHair::loadMatricesToShader(glm::mat4 _modelMatrix, Camera *_cam){
    // Calculate projection matrix
    int scale = 1;
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(scale,scale,scale));
    glm::mat4 projectionMatrix = _cam->getProjectionMatrix();

    glm::mat4 modelViewMatrix = _cam->getViewMatrix() * _modelMatrix;

    glm::mat3 normalMatrix = glm::mat3(modelViewMatrix);
    normalMatrix = glm::inverse(normalMatrix);
    normalMatrix = glm::transpose(normalMatrix);

    glm::mat4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

    m_shaderProgram->use();
    // Find uniform locations
    GLuint modelViewLoc = m_shaderProgram->getUniformLoc("modelViewMatrix");
    GLuint projLoc = m_shaderProgram->getUniformLoc("projectionMatrix");
    GLuint normalLoc = m_shaderProgram->getUniformLoc("normalMatrix");
    GLuint modelViewProjectionLoc = m_shaderProgram->getUniformLoc("modelViewProjectionMatrix");
    GLuint grassSizeLoc = m_shaderProgram->getUniformLoc("grassSize");
    GLuint maxHeightLoc = m_shaderProgram->getUniformLoc("maxHeight");
    GLuint minHeightLoc = m_shaderProgram->getUniformLoc("minHeight");
    GLuint maxAngleLoc = m_shaderProgram->getUniformLoc("maxAngle");
    GLuint numStrandsLoc = m_shaderProgram->getUniformLoc("numStrands");


    glUniform1i(numStrandsLoc,m_numStrandsPerFace);
    glUniform1f(maxHeightLoc,m_maxHeight);
    glUniform1f(minHeightLoc,m_minHeight);
    glUniform1f(maxAngleLoc,m_maxAngle);
    glUniform1f(grassSizeLoc,m_grassSize);


    //glUniformMatrix4fv(m_modelLoc, 1, false, glm::value_ptr(m_modelMatrix));
    glUniformMatrix4fv(modelViewLoc, 1, false, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(projLoc, 1, false, glm::value_ptr(projectionMatrix));
    glUniformMatrix3fv(normalLoc, 1, false, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(modelViewProjectionLoc, 1, false, glm::value_ptr(modelViewProjectionMatrix));
}

//----------------------------------------------------------------------------------------------------------------------
