#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myopenglwidget.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->openGLWidget);
    connect(ui->openGLWidget,(void (MyOpenGLWidget::*)(QVector3D))&MyOpenGLWidget::mousePickingPos,this,(void (MainWindow::*)(QVector3D))&MainWindow::getMousePickingPos);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actLoadModel_triggered()
{
    QString filePath=QFileDialog::getOpenFileName(this,"选择模型文件","",
                   "OBJ (*.obj);;FBX(*.fbx);;ALL FILES( *.* ) ");
    ui->openGLWidget->loadModel(filePath);
}

void MainWindow::getMousePickingPos(QVector3D pos)
{
    ui->statusbar->setStyleSheet("font: 14pt ");
    ui->statusbar->showMessage(" 世界坐标:    X:" + QString::number(pos.x(),'f', 2) +
                               " Y:"+QString::number(pos.y(),'f', 2) +
                               " Z:"+QString::number(pos.z(),'f', 2));
}
