#include <QApplication>
#include "mainwindow.h"
#include <QFile>

int main(int argc, char **argv)
{
    QApplication app(argc,argv);
    MainWindow w;
    QFile File("styleSheet/darkOrange");
    File.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(File.readAll());

    w.setStyleSheet(StyleSheet);
    w.setWindowTitle(QString("Geometry Clipmaps"));
    w.show();
    app.exec();
}
