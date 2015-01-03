#ifndef __GEOMETRYCLIPMAP_H_
#define __GEOMETRYCLIPMAP_H_

#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <noise/noise.h>

#include "noiseutils.h"
#include "ShaderProgram.h"
#include "Texture.h"

class GeometryClipmap {
public:
    // ctor
    GeometryClipmap(int _levels);
    // dtor
    ~GeometryClipmap();
    // create the shader program
    void createShader();
    // Load the matrices required by the shader
    void loadMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);
    void loadClippedMatricesToShader(glm::mat4 _modelMatrix, glm::mat4 _viewMatrix, glm::mat4 _projectionMatrix);

    // create the heightmap, colourmap, normalmap and geometry
    void Initialise();
    // update the heightmap, normalmap colourmap if the camera is moved
    void update();
    // render the geometry
    void render();
    // sets if drawn in wireframe mode
    inline void setWireframe(bool _wireframe){m_wireframe = _wireframe;}
    // sets is hole is cut in geometry
    inline void setCutout(bool _cutout){m_cutout = _cutout;}
    // scales the height of the geometry
    inline void setHeight(int _height){m_height = float(_height/10.0);}
    // flags to dictate if the heightmap needs to be updated
    inline void setUpdating(bool _updating){m_updating = _updating;}
    // VAO to store the geometry
    GLuint m_VAO;
private:
    // holds vertices
    std::vector<glm::vec3> m_vert;
    // size of the grid
    GLuint m_gridSize;
    // number of levels of detail
    int m_levels;
    // camera position in world coords
    glm::vec3 m_viewPos;
    // model matrix
    glm::mat4 m_modelMatrix;
    // The shader program and shaders
    ShaderProgram *m_shaderProgram;
    Shader *m_vertShader;
    Shader *m_fragShader;
    // uniform shader locations
    GLuint m_modelViewProjectionLoc;
    GLuint m_modelViewLoc;
    GLuint m_heightLoc;
    GLuint m_cutoutLoc;
    GLuint m_geoTextureLoc;
    GLuint m_normalGeoTexLoc;
    GLuint m_colourLoc;
    // texture handle for the heightmap
    GLuint m_texHeightmap;
    // dictates whether is drawn in wireframe
    bool m_wireframe;
    // dictates whether drawn cutout in center of terrain
    bool m_cutout;
    // textures
    Texture *m_heightmapTex;
    Texture *m_normalmapTex;
    Texture *m_colourTex;
    Texture *m_grassTex;
    Texture *m_mudTex;
    Texture *m_rockTex;
    Texture *m_snowTex;
    Texture *m_sandTex;
    // float to scale terrain heights
    float m_height;
    // used to incriment camera position
    float m_zvalue;
    float m_xvalue;
    // heightmap update
    bool m_updating;
    // noise objects for heightmap generation
    noise::module::Perlin m_perlin;
    utils::NoiseMap m_noisemap;
    utils::NoiseMapBuilderPlane m_heightmapBuilder;
    utils::RendererImage m_heightmap;
    utils::Image m_image;
    utils::WriterBMP m_writer;
    utils::RendererImage m_colourMap;
    utils::Image m_imageColour;
    utils::WriterBMP m_writerColour;

};


#endif
