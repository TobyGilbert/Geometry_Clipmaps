#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QGLWidget>
#include <QEvent>
#include <QResizeEvent>
#include <QMessageBox>
#include <Camera.h>
#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"
#include "Bmp.h"
#include "GeometryClipmap.h"
#include "Skybox.h"
#include "grasshair.h"
#include "Water.h"
#include "MesoTerrain.h"

class OpenGLWidget : public QGLWidget
{
    Q_OBJECT //must include to gain access to qt stuff

public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    explicit OpenGLWidget(const QGLFormat _format, QWidget *_parent=0);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~OpenGLWidget();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the virtual initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL();
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief called to resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(const int _w, const int _h );
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief mouse move
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent(QMouseEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief a timer event function from the Q_object
    //----------------------------------------------------------------------------------------------------------------------
    void timerEvent(QTimerEvent *);//
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief keyboard events
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event);
    //----------------------------------------------------------------------------------------------------------------------
    void keyReleaseEvent(QKeyEvent *_event);
    // mouse press action
    void mousePressEvent(QMouseEvent *_event);
    // scroll wheel action
    void wheelEvent(QWheelEvent *_event);

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Our Camera
    //----------------------------------------------------------------------------------------------------------------------
    Camera *m_cam;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Model matrix
    //----------------------------------------------------------------------------------------------------------------------
    glm::mat4 m_modelMatrix;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Height of the window
    //----------------------------------------------------------------------------------------------------------------------
    int m_height;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Width of the window
    //----------------------------------------------------------------------------------------------------------------------
    int m_width;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Mouse transforms
    //----------------------------------------------------------------------------------------------------------------------
    glm::mat4 m_mouseGlobalTX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief model pos
    //----------------------------------------------------------------------------------------------------------------------
    glm::vec3 m_modelPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Spin face x
    //----------------------------------------------------------------------------------------------------------------------
    float m_spinXFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Sping face y
    //----------------------------------------------------------------------------------------------------------------------
    float m_spinYFace;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief rotate bool
    //----------------------------------------------------------------------------------------------------------------------
    bool m_rotate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief translate bool
    //----------------------------------------------------------------------------------------------------------------------
    bool m_translate;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief
    //----------------------------------------------------------------------------------------------------------------------
    int m_origX;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief
    //----------------------------------------------------------------------------------------------------------------------
    int m_origY;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief
    //----------------------------------------------------------------------------------------------------------------------
    int m_origXPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief
    //----------------------------------------------------------------------------------------------------------------------
    int m_origYPos;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Geometry clipmap object
    //----------------------------------------------------------------------------------------------------------------------
    GeometryClipmap *m_geometryClipmap;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief camera view angle
    //----------------------------------------------------------------------------------------------------------------------
    float m_viewAngle;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief skybox Object
    //----------------------------------------------------------------------------------------------------------------------
    Skybox *m_sky;
    // dictates whether terrain is drawn in wireframe
    bool m_wireframe;
    // dictates whether cutout in terrain is drawn (can be removed)
    bool m_cutout;
    // Water object
    Water *m_water;
    // Meso terrain object (obj)
    MesoTerrain *m_mesoTerrain;
public slots:
    // Scales the height of the terrain
    void setHeight(int _height);
    // Sets the camera moving over the terrain
    void movingCamera(bool _moving);
};

#endif // OPENGLWIDGET_H
