#include "Trees.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXTREES 100
Trees::Trees(int _numTrees, QString _pathToHeightmap, glm::vec4 _lightPosition, glm::vec3 _lightIntensity){
    m_numTrees = _numTrees;
    createShader(_lightPosition, _lightIntensity);
    initTrees(_pathToHeightmap);
}

Trees::~Trees(){

    delete m_tree;
    delete m_leaves;
    //delete m_barkTex;
    //delete m_leavesTex;
    delete m_shaderProgram;
}

void Trees::createShader(glm::vec4 _lightPosition, glm::vec3 _lightIntensity){
    m_shaderProgram = new ShaderProgram();
    m_vertShader = new Shader("shaders/TreeVert.glsl", GL_VERTEX_SHADER);
    m_fragShader = new Shader("shaders/TreeFrag.glsl", GL_FRAGMENT_SHADER);

    m_shaderProgram->attachShader(m_vertShader);
    m_shaderProgram->attachShader(m_fragShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColour");
    m_shaderProgram->link();
    m_shaderProgram->use();

    delete m_vertShader;
    delete m_fragShader;

//    m_barkTex = new Texture("textures/bark1.tga");
//    m_barkTex->bind(7);

//    m_leavesTex = new Texture("textures/leafs1.tga");
//    m_leavesTex->bind(8);

//    GLuint lightPosTreeLoc = m_shaderProgram->getUniformLoc("light.position");
//    GLuint lightIntTreeLoc = m_shaderProgram->getUniformLoc("light.intensity");
//    GLuint kdTreeLoc = m_shaderProgram->getUniformLoc("Kd");
//    GLuint kaTreeLoc = m_shaderProgram->getUniformLoc("Ka");
//    GLuint ksTreeLoc = m_shaderProgram->getUniformLoc("Ks");
//    GLuint shininessTreeLoc = m_shaderProgram->getUniformLoc("shininess");
    //GLuint barkTreeLoc = m_shaderProgram->getUniformLoc("bark");
    //GLuint leavesTreeLoc = m_shaderProgram->getUniformLoc("leaves");
    m_isBarkLoc = m_shaderProgram->getUniformLoc("isBark");
    m_isBillboardLoc = m_shaderProgram->getUniformLoc("isBillboard");

//    glUniform4f(lightPosTreeLoc, _lightPosition.x, _lightPosition.y, _lightPosition.z, _lightPosition.w);
//    glUniform3f(lightIntTreeLoc, _lightIntensity.x, _lightIntensity.y, _lightIntensity.z);
//    glUniform3f(kdTreeLoc, 0.5, 0.5, 0.5);
//    glUniform3f(kaTreeLoc, 0.5, 0.5, 0.5);
//    glUniform3f(ksTreeLoc, 1.0, 1.0, 1.0);
//    glUniform1f(shininessTreeLoc, 10.0);
//    glUniform1i(barkTreeLoc, 7);
//    glUniform1i(leavesTreeLoc, 8);
}

void Trees::initTrees(QString _pathToHeightmap){
    QImage heightmap(_pathToHeightmap);
    // load model
    m_tree = new Model("models/tree.obj");
    m_leaves = new Model("models/leaves.obj");
    glBindVertexArray(m_tree->getVAO());

    // Instance  offset buffer
    GLfloat m_offset[m_numTrees*3];
    srand(time(NULL));

    // COULD BE SPED UP USING TRANSFORM FEEDBACK !!
    for (int i=0; i<m_numTrees*3; i+=3){
        float x = -(0.0) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((1.0)-(0.0))));
        float z = -(0.0) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((1.0)-(0.0))));
        float y = getHeight((int)(x * heightmap.width()), (int)(z * heightmap.height()), heightmap);

        while (y < 0.09 || y > 0.13){
            x = -(0.0) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((1.0)-(0.0))));
            z = -(0.0) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((1.0)-(0.0))));
            y = getHeight((int)(x * heightmap.width()), (int)(z * heightmap.height()), heightmap);
        }

        m_offset[i] = x;
        m_offset[i+1] = y;
        m_offset[i+2] = z;

        m_positions.push_back(glm::vec2(x, z));
    }

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_offset), &m_offset[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glBindVertexArray(m_leaves->getVAO());
    GLuint leavesPositionBuffer;
    glGenBuffers(1, &leavesPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, leavesPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_offset), &m_offset[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    glVertexAttribDivisor(3, 1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &positionBuffer);
    glDeleteBuffers(1, &leavesPositionBuffer);
}

void Trees::renderToTexture(glm::mat4 _modelMatrix, Camera *_cam){
    // Rendering trees to a texture
    // create framebuffer
    glGenFramebuffers(1, &m_fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

    // create rendertexture
    GLuint renderTex;
    glGenTextures(1, &renderTex);
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, renderTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // bind the texture to the fbo
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);

    //Create depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);

    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    // Set the target for the fragment shader outputs
    GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1,drawBufs);

    // Unbind the framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // One pixel white texture
    GLuint whiteTextureHandle;
    GLubyte whiteTex[] = {255, 255, 255, 255};
    glActiveTexture(GL_TEXTURE10);
    glGenTextures(1, &whiteTextureHandle);
    glBindTexture(GL_TEXTURE_2D, whiteTextureHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteTex);

    glBindTexture(GL_TEXTURE_2D, 0);

    // Draw FBO texture
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboHandle);

    // Use white texture
    GLuint whiteTexLoc = m_shaderProgram->getUniformLoc("renderTex");
    glUniform1i(whiteTexLoc, 10);

    glm::mat4 modelMatrix;
    modelMatrix = glm::mat4();

    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0, 0.0, 2.0), glm::vec3(0.0 ,0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 projectionMatrix = glm::perspective(45.0f, 720.0f/576.0f, 0.0001f, 350.0f);
    loadMatricesToShader(modelMatrix, viewMatrix, projectionMatrix);
    glUniform1i(m_isBarkLoc, 1);
    glUniform1i(m_isBillboardLoc, 0);
    glBindVertexArray(m_tree->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_tree->getNumVerts());
    glBindVertexArray(0);

    loadMatricesToShader(modelMatrix, viewMatrix, projectionMatrix);
    glUniform1i(m_isBarkLoc, 0);
    glUniform1i(m_isBillboardLoc, 0);
    glBindVertexArray(m_leaves->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_leaves->getNumVerts());
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLuint fboTexLoc = m_shaderProgram->getUniformLoc("renderTex");
    glUniform1i(fboTexLoc, 9);

}


float Trees::getHeight(int _width, int _depth, QImage _image){
    QColor _pixel = _image.pixel(_width, _depth);
    return ((_pixel.redF()+_pixel.blueF()+_pixel.greenF())/3.0)/4.0;
}

void Trees::loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix){
    m_shaderProgram->use();
    GLuint modelTreeLoc = m_shaderProgram->getUniformLoc("modelMatrix");
    GLuint viewTreeLoc = m_shaderProgram->getUniformLoc("viewMatrix");
    GLuint modelViewTreeLoc = m_shaderProgram->getUniformLoc("modelViewMatrix");
    GLuint projTreeLoc = m_shaderProgram->getUniformLoc("projectionMatrix");
    GLuint normalTreeLoc = m_shaderProgram->getUniformLoc("normalMatrix");
    GLuint modelViewProjectionTreeLoc = m_shaderProgram->getUniformLoc("modelViewProjectionMatrix");
    // Calculate projection and modelview matrix
    glm::mat4 modelViewMatrix =_viewMatrix * _modelMatrix;

    glm::mat3 normalMatrix = glm::mat3(modelViewMatrix);
    normalMatrix = glm::inverse(normalMatrix);
    normalMatrix = glm::transpose(normalMatrix);

    glm::mat4 modelViewProjectionMatrix = _projectionMatrix * modelViewMatrix;

    //Tree uniforms
    glUniformMatrix4fv(modelTreeLoc, 1, GL_FALSE, glm::value_ptr(_modelMatrix));
    glUniformMatrix4fv(viewTreeLoc, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
    glUniformMatrix4fv(modelViewTreeLoc, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(projTreeLoc, 1, GL_FALSE, glm::value_ptr(_projectionMatrix));
    glUniformMatrix3fv(normalTreeLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(modelViewProjectionTreeLoc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

}

void Trees::render(){
    glUniform1i(m_isBarkLoc, 1);
    glBindVertexArray(m_tree->getVAO());
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_tree->getNumVerts(), m_numTrees);
    glBindVertexArray(0);

    glUniform1i(m_isBarkLoc, 0);
    glBindVertexArray(m_leaves->getVAO());
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_leaves->getNumVerts(), m_numTrees);
    glBindVertexArray(0);
}

void Trees::updateTrees(QString _pathToHeightmap){
   // m_tree->deleteVAO();
    //m_leaves->deleteVAO();
    m_positions.clear();
    initTrees(_pathToHeightmap);
}
