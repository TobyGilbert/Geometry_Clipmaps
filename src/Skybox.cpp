#include "Skybox.h"

#include <QGLWidget>

Skybox::Skybox(){
    initSkybox();
    createShader();
}

Skybox::~Skybox(){
    // Delete skybox model
    delete m_model;
    // Delete skybox shader program
    delete m_shaderProgram;
}

void Skybox::initSkybox(){
    m_model = new Model("models/skyBox.obj");
}

GLuint Skybox::loadCubeMap(std::string _pathToFile){
    GLuint texID;

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
    const char * suffixes[] = { "posx", "negx", "posy",
       "negy", "posz", "negz" };
    GLuint targets[] = {
       GL_TEXTURE_CUBE_MAP_POSITIVE_X,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
       GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
       GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (int i =0; i<6; i++){
       GLuint texture;
       glGenTextures(1, &texture);
       glBindTexture(GL_TEXTURE_2D, texture);
       std::string texName = std::string(_pathToFile) + "_" + suffixes[i] + ".png";

       QString name = QString::fromUtf8(texName.c_str());
       QImage *image = new QImage(name);
       QImage tex = QGLWidget::convertToGLFormat(*image);
       glTexImage2D(targets[i],0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}

void Skybox::createShader(){
    m_shaderProgram = new ShaderProgram();
    m_vertShader = new Shader("shaders/skyboxVert.glsl", GL_VERTEX_SHADER);
    m_fragShader =  new Shader("shaders/skyboxFrag.glsl", GL_FRAGMENT_SHADER);
    m_shaderProgram->attachShader(m_vertShader);
    m_shaderProgram->attachShader(m_fragShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColour");
    m_shaderProgram->link();
    m_shaderProgram->use();

    delete m_vertShader;
    delete m_fragShader;

    ilInit();

    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texID);
    const char * suffixes[] = { "posx", "negx", "posy",
                                "negy", "posz", "negz" };
    GLuint targets[] = {
       GL_TEXTURE_CUBE_MAP_POSITIVE_X,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
       GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
       GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
       GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (int i =0; i<6; i++){
       std::string texName = std::string("textures/skyCubeMap") + "_" + suffixes[i] + ".png";
       ILuint image = ilGenImage();
       ilBindImage(image);
       ILboolean loadSuccess = ilLoadImage(texName.c_str());
       if(!loadSuccess){
          std::cerr<<"Failed to load image: "<<texName<<std::endl;
          ilBindImage(NULL);
          ilDeleteImage(image);
       }
       ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

       if(!convertSuccess){
          std::cerr<<"Failed to convert image: "<<texName<<std::endl;
          ilBindImage(NULL);
          ilDeleteImage(image);
       }
       glTexImage2D(targets[i], 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::update(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);

    GLuint cubeMapLoc = m_shaderProgram->getUniformLoc("cubeMapTex");
    glUniform1i(cubeMapLoc, 0);
}

void Skybox::loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix){
    m_shaderProgram->use();
    GLuint MVPLoc = m_shaderProgram->getUniformLoc("modelViewProjectionMatrix");

    glm::mat4 modelViewProjectionMatrix = _projectionMatrix * _viewMatrix * _modelMatrix;

    glUniformMatrix4fv(MVPLoc, 1, false, glm::value_ptr(modelViewProjectionMatrix));
}

void Skybox::render(){
    update();
    glBindVertexArray(m_model->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, m_model->getNumVerts());
    glBindVertexArray(0);
}
