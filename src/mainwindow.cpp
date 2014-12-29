#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    QGLFormat format;
    format.setVersion(4,1);
    format.setProfile(QGLFormat::CoreProfile);

    m_openGLWidget = new OpenGLWidget(format,this);
    ui->gridLayout->addWidget(m_openGLWidget,0,0,1,1);

    ui->horizontalSlider->setMinimum(20);
    ui->horizontalSlider->setMaximum(30);

    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), m_openGLWidget, SLOT(setHeight(int)));
    connect(ui->checkBox, SIGNAL(clicked(bool)), m_openGLWidget, SLOT(movingCamera(bool)));

}

MainWindow::~MainWindow(){
    delete ui;
    delete m_openGLWidget;

}
