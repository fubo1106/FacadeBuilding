/********************************************************************************
** Form generated from reading UI file 'facbuilding.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACBUILDING_H
#define UI_FACBUILDING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FacBuildingClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FacBuildingClass)
    {
        if (FacBuildingClass->objectName().isEmpty())
            FacBuildingClass->setObjectName(QStringLiteral("FacBuildingClass"));
        FacBuildingClass->resize(600, 400);
        menuBar = new QMenuBar(FacBuildingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        FacBuildingClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FacBuildingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FacBuildingClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(FacBuildingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        FacBuildingClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(FacBuildingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FacBuildingClass->setStatusBar(statusBar);

        retranslateUi(FacBuildingClass);

        QMetaObject::connectSlotsByName(FacBuildingClass);
    } // setupUi

    void retranslateUi(QMainWindow *FacBuildingClass)
    {
        FacBuildingClass->setWindowTitle(QApplication::translate("FacBuildingClass", "FacBuilding", 0));
    } // retranslateUi

};

namespace Ui {
    class FacBuildingClass: public Ui_FacBuildingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACBUILDING_H
