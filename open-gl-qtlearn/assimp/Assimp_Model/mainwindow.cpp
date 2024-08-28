#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myopenglwidget.h"

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


void MainWindow::on_actDrawRect_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::Rect);
}

void MainWindow::on_clearShape_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::None);
}

void MainWindow::on_wireframe_triggered()
{
    ui->openGLWidget->drawShape(MyOpenGLWidget::RectWireframe);
}

