/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <myopenglwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actDrawRect;
    QAction *clearShape;
    QAction *wireframe;
    QWidget *centralwidget;
    MyOpenGLWidget *openGLWidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actDrawRect = new QAction(MainWindow);
        actDrawRect->setObjectName(QString::fromUtf8("actDrawRect"));
        clearShape = new QAction(MainWindow);
        clearShape->setObjectName(QString::fromUtf8("clearShape"));
        wireframe = new QAction(MainWindow);
        wireframe->setObjectName(QString::fromUtf8("wireframe"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        openGLWidget = new MyOpenGLWidget(centralwidget);
        openGLWidget->setObjectName(QString::fromUtf8("openGLWidget"));
        openGLWidget->setGeometry(QRect(210, 130, 300, 200));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menubar->setNativeMenuBar(true);
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(actDrawRect);
        toolBar->addAction(wireframe);
        toolBar->addAction(clearShape);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actDrawRect->setText(QCoreApplication::translate("MainWindow", "\347\273\230\345\210\266\347\237\251\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        actDrawRect->setToolTip(QCoreApplication::translate("MainWindow", "\347\273\230\345\210\266\344\270\200\344\270\252\347\237\251\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        clearShape->setText(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\345\233\276\345\275\242", nullptr));
#if QT_CONFIG(tooltip)
        clearShape->setToolTip(QCoreApplication::translate("MainWindow", "\346\270\205\351\231\244\347\273\230\345\210\266\347\232\204\345\233\276\345\275\242", nullptr));
#endif // QT_CONFIG(tooltip)
        wireframe->setText(QCoreApplication::translate("MainWindow", "\347\272\277\346\241\206\347\273\230\345\210\266", nullptr));
#if QT_CONFIG(tooltip)
        wireframe->setToolTip(QCoreApplication::translate("MainWindow", "\347\273\230\345\210\266\346\210\220\347\272\277\346\241\206\346\250\241\345\274\217", nullptr));
#endif // QT_CONFIG(tooltip)
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
