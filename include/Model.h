#ifndef __MODEL_H_
#define __MODEL_H_

#include <iostream>
#include <string>
#include <OpenGL/gl3.h>

class Model{
public:
   Model(std::string _path);
   Model();
   ~Model();
   //void loadModel(std::string _path);
   GLuint getVAO(){return m_VAO;}
   void loadCube();
   GLuint getNumVerts(){return m_numVerts;}
private:
   GLuint m_vertexVBO;
   GLuint m_normalsVBO;
   GLuint m_textureVBO;
   GLuint m_tangentsVBO;
   GLuint m_cubeVBO;
   GLuint m_VAO;
   GLuint m_numVerts;
   void loadModel(std::string _path);

};

#endif
