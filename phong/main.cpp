#include "mainwindow.h"
#include <QApplication>
#include <assimp/Importer.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Assimp::Importer importer;
    return a.exec();
}
