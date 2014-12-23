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

    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setMaximum(10);

    connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), m_openGLWidget, SLOT(setHeight(int)));
}

MainWindow::~MainWindow(){
    delete ui;
    delete m_openGLWidget;

}
