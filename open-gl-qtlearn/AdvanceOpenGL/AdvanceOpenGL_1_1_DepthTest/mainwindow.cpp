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

