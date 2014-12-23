#include <QGuiApplication>

#include "OpenGLWidget.h"
#include <iostream>
#include <QCursor>
#include <sys/time.h>
#include <OpenGL/glu.h>
#include <cmath>
#include "Bmp.h"


const static float INCREMENT=0.01;
const static float ZOOM=0.005;



OpenGLWidget::OpenGLWidget(const QGLFormat _format, QWidget *_parent) : QGLWidget(_format,_parent), m_wireframe(false), m_cutout(false){
    // set this widget to have the initial keyboard focus
    setFocus();
    setFocusPolicy( Qt::StrongFocus );
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    m_rotate=false;
    // mouse rotation values set to 0
    m_spinXFace=0;
    m_spinYFace=0;
    // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
    this->resize(_parent->size());
}
//----------------------------------------------------------------------------------------------------------------------
OpenGLWidget::~OpenGLWidget(){
    delete m_cam;
    delete m_geometryClipmap;
    delete m_sky;
    //delete m_grass;
}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::initializeGL(){
    glClearDepth(1.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // as re-size is not explicitly called we need to do this.
    glViewport(0,0,width(),height());

    // Initialize the camera
    m_cam = new Camera(glm::vec3(0.0, 0.3, 0.10), width(), height(), glm::vec3(0.0, 1.0, 0.0));

    // Create Geometry
    m_geometryClipmap = new GeometryClipmap(6);

    //m_grass = new grassHair(m_geometryClipmap->m_VAO);

    //Create Skybox
    m_sky = new Skybox();

    startTimer(0);

}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::resizeGL(const int _w, const int _h){
    // set the viewport for openGL
    glViewport(0,0,_w,_h);
    m_cam->setShape(_w, _h);
}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::timerEvent(QTimerEvent *){
    updateGL();
}

//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::paintGL(){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    //Initialise the model matrix
    glm::mat4 rotx;
    glm::mat4 roty;

    rotx = glm::rotate(rotx, m_spinXFace, glm::vec3(1.0, 0.0, 0.0));
    roty = glm::rotate(roty, m_spinYFace, glm::vec3(0.0, 1.0, 0.0));

    //Initialise the model matrix
    m_mouseGlobalTX = glm::mat4();
    m_mouseGlobalTX = rotx*roty;
    m_mouseGlobalTX[3][0] = m_modelPos.x;
    m_mouseGlobalTX[3][1] = m_modelPos.y;
    m_mouseGlobalTX[3][2] = m_modelPos.z;

    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = m_mouseGlobalTX;
    //m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0, -10.0, 0.0));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(5.0, 1.0, 5.0));
    //m_modelMatrix = glm::rotate(m_modelMatrix, m_viewAngle, glm::vec3(0.0, 1.0, 0.0));
    m_geometryClipmap->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());

    // Set if drawn in wireframe
    m_geometryClipmap->setWireframe(m_wireframe);
    m_geometryClipmap->setCutout(m_cutout);
    m_geometryClipmap->render();

    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = glm::rotate(m_modelMatrix, m_viewAngle, glm::vec3(0.0, 1.0, 0.0));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(200.0, 200.0, 200.0));
    m_sky->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_sky->render();

    m_modelMatrix = glm::mat4(1.0);
    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(5.0, 1.0, 5.0));
    //m_grass->draw(m_modelMatrix, m_cam, 100);

}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::mousePressEvent(QMouseEvent *_event){
    // this method is called when the mouse button is pressed in this case we
    // store the value where the maouse was clicked (x,y) and set the Rotate flag to true
    if(_event->button() == Qt::LeftButton)
    {
      m_origX = _event->x();
      m_origY = _event->y();
      m_rotate =true;
    }
    // right mouse translate mode
    else if(_event->button() == Qt::MiddleButton)
    {
      m_origXPos = _event->x();
      m_origYPos = _event->y();
      m_translate=true;
    }
    else if(_event->button() == Qt::RightButton){
        m_origYPos = _event->y();
        m_translate=true;
      }
}

void OpenGLWidget::mouseMoveEvent (QMouseEvent * _event)
{
    // note the method buttons() is the button state when event was called
    // this is different from button() which is used to check which button was
    // pressed when the mousePress/Release event is generated
    if(m_rotate && _event->buttons() == Qt::LeftButton)
    {
       int diffx=_event->x()-m_origX;
       int diffy=_event->y()-m_origY;
       m_spinXFace += (float) 0.05f * diffy;
       m_spinYFace += (float) 0.05f * diffx;
       m_origX = _event->x();
       m_origY = _event->y();

    }
    // right mouse translate code
    else if(m_translate && _event->buttons() == Qt::RightButton)
    {
       int diffX = (int)(_event->x() - m_origXPos);
       int diffY = (int)(_event->y() - m_origYPos);
       m_origXPos=_event->x();
       m_origYPos=_event->y();
       m_modelPos.x += INCREMENT * diffX;
       m_modelPos.y -= INCREMENT * diffY;

     }
}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::keyPressEvent(QKeyEvent *_event){

    float speed = 100.0;
    switch(_event->key()){
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;

    //calculate our camera rotations
    case Qt::Key_W:
        m_geometryClipmap->setUpdating(true);
        //m_geometryClipmap->moveCamera(glm::vec3(0.0, 0.0, -m_cameraSpeed));
        //m_modelPos.z+=ZOOM;
    break;
    case Qt::Key_S:
        m_geometryClipmap->setUpdating(true);
        //m_geometryClipmap->moveCamera(glm::vec3(0.0, 0.0, m_cameraSpeed));
    break;
    case Qt::Key_A:
        m_geometryClipmap->setUpdating(true);
        //m_geometryClipmap->moveCamera(glm::vec3(-m_cameraSpeed, 0.0, 0.0));
    break;
    case Qt::Key_D:
        m_geometryClipmap->setUpdating(true);
        //m_geometryClipmap->moveCamera(glm::vec3(m_cameraSpeed, 0.0, 0.0));
    break;
    case Qt::Key_4:
        m_wireframe = true;
    break;
    case Qt::Key_5:
        m_wireframe = false;
    break;
    case Qt::Key_C:
        if (m_cutout){
            m_cutout = false;
        }
        else{
            m_cutout = true;
        }
    break;
    default : break;
    }
}

void OpenGLWidget::wheelEvent(QWheelEvent *_event){
    // Sourced from Jon Macey's NGL library
    // check the diff of the wheel position (0 means no change)
    if(_event->delta() > 0)
    {
        m_modelPos.z+=ZOOM;
    }
    else if(_event->delta() <0 )
    {
        m_modelPos.z-=ZOOM;
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *_event){
    m_cameraSpeed = 100.0;
    m_geometryClipmap->setUpdating(false);
}
//---------------SLOTS----------------
void OpenGLWidget::setHeight(int _height){
    m_geometryClipmap->setHeight(_height);
}

