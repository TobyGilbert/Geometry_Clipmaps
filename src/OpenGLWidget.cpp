#include "OpenGLWidget.h"

#include <QGuiApplication>
#include <iostream>
#include <QCursor>
#include <sys/time.h>
#include <OpenGL/glu.h>
#include <cmath>

#include "Bmp.h"

const static float INCREMENT=0.01;
const static float ZOOM=0.5;

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
    delete m_water;
    delete m_mesoTerrain;
}
//----------------------------------------------------------------------------------------------------------------------
void OpenGLWidget::genFBOs(){
    // Gen framebuffer
    glGenFramebuffers(1, &m_reflectFB);
    glBindFramebuffer(GL_FRAMEBUFFER, m_reflectFB);

    // Bind texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(m_water->getReflectTex()), 0);

    // Set the targets for the fragment shader output
    GLenum drawBufs[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBufs);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Gen framebuffer
    glGenFramebuffers(1, &m_refractFB);
    glBindFramebuffer(GL_FRAMEBUFFER, m_refractFB);

    // Bind texture to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,*(m_water->getRefractTex()), 0);

    // Set the targets for the fragment shader output
    glDrawBuffers(1, drawBufs);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLWidget::initializeGL(){
    glClearDepth(1.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);
    glDepthFunc(GL_LESS);

    // as re-size is not explicitly called we need to do this.
    glViewport(0,0,width(),height());

    // Initialize the camera
    m_cam = new Camera(glm::vec3(0.0, 10.0, 10.0), width(), height(), glm::vec3(0.0, 1.0, 0.0));

    // Create Geometry
    m_geometryClipmap = new GeometryClipmap(5);

    // Create plane for water
    m_water = new Water();

    //Create Skybox
    m_sky = new Skybox();

    // Create meso level terrain
    m_mesoTerrain = new MesoTerrain();

    genFBOs();

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
void OpenGLWidget::renderReflections(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 512, 512);

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

    // Draw environment map
    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(200.0, -200.0, 200.0));
    m_sky->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_sky->render();

    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(10.0, -1.0, 10.0));
    m_geometryClipmap->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    m_geometryClipmap->setWireframe(m_wireframe);
    m_geometryClipmap->setCutout(m_cutout);
    m_geometryClipmap->render();

}

void OpenGLWidget::renderRefractions(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 512, 512);

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

    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(10.0, 1.0, 10.0));
    m_geometryClipmap->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    m_geometryClipmap->setWireframe(m_wireframe);
    m_geometryClipmap->setCutout(m_cutout);
    m_geometryClipmap->render();

}

void OpenGLWidget::paintGL(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_reflectFB);
    renderReflections();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_refractFB);
    renderRefractions();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, width()*2, height()*2);

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

    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(10.0, 1.0, 10.0));
    m_geometryClipmap->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    m_geometryClipmap->setWireframe(m_wireframe);
    m_geometryClipmap->setCutout(m_cutout);
    m_geometryClipmap->render();

    // Draw environment map
    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(200.0, 200.0, 200.0));
    m_sky->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    m_sky->render();

    // Draw water plane
    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.0, 1.5, 0.0));
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(80.0, 80.0, 80.0));
    m_water->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
    m_water->render();

    // Draw declan's meso terrain
    m_modelMatrix = m_mouseGlobalTX;
    m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(5.0, 5.0, 5.0));
    m_mesoTerrain->loadMatricesToShader(m_modelMatrix, m_cam->getViewMatrix(), m_cam->getProjectionMatrix());
//    m_mesoTerrain->render();

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
    switch(_event->key()){
    case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
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
}
//---------------SLOTS----------------
void OpenGLWidget::setHeight(int _height){
    m_geometryClipmap->setHeight(_height);
}

void OpenGLWidget::movingCamera(bool _moving){
    m_geometryClipmap->setUpdating(_moving);
}

