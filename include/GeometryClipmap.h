#ifndef __GEOMETRYCLIPMAP_H_
#define __GEOMETRYCLIPMAP_H_


#include <OpenGL/gl3.h>

#include "glm/glm.hpp"
#include <vector>

#include "ShaderProgram.h"
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"

class GeometryClipmap {
public:
    GeometryClipmap(int _levels);
    ~GeometryClipmap();
    void createShader();
    void loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);
    void Initialise();
    void update();
    void render();
    void moveCamera(glm::vec3 _direction);
    inline void setWireframe(bool _wireframe){m_wireframe = _wireframe;}
    inline void setCutout(bool _cutout){m_cutout = _cutout;}
    inline void setHeight(int _height){m_height = float(_height/10.0);}
    inline void setUpdating(bool _updating){m_updating = _updating;}
    GLuint m_VAO;
private:
    std::vector<glm::vec3> m_vert;
    GLuint m_gridSize;
    int m_levels;
    glm::vec3 m_viewPos;
    glm::mat4 m_modelMatrix;
    ShaderProgram *m_shaderProgram;
    Shader *m_vertShader;
    Shader *m_fragShader;
    GLuint m_modelViewProjectionLoc;
    GLuint m_modelViewLoc;
    GLuint m_heightLoc;
    GLuint m_texHeightmap;

    bool m_wireframe;
    GLuint m_cutoutLoc;
    bool m_cutout;
    GLuint m_geoTextureLoc;
    GLuint m_normalGeoTexLoc;
    GLuint m_colourLoc;
    Texture *m_heightmapTex;
    Texture *m_normalmapTex;
    Texture *m_colourTex;
    Texture *m_grassTex;
    Texture *m_mudTex;
    Texture *m_rockTex;
    Texture *m_snowTex;

    float m_height;

    float m_zvalue;
    float m_xvalue;

    bool m_updating;





};


#endif
