/********************************************************************************
** Form generated from reading UI file 'ocmeridiaauto.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OCMERIDIAAUTO_H
#define UI_OCMERIDIAAUTO_H

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

class Ui_ocMeridiaAuto
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ocMeridiaAuto)
    {
        if (ocMeridiaAuto->objectName().isEmpty())
            ocMeridiaAuto->setObjectName(QStringLiteral("ocMeridiaAuto"));
        ocMeridiaAuto->resize(400, 300);
        menuBar = new QMenuBar(ocMeridiaAuto);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        ocMeridiaAuto->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ocMeridiaAuto);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ocMeridiaAuto->addToolBar(mainToolBar);
        centralWidget = new QWidget(ocMeridiaAuto);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ocMeridiaAuto->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(ocMeridiaAuto);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ocMeridiaAuto->setStatusBar(statusBar);

        retranslateUi(ocMeridiaAuto);

        QMetaObject::connectSlotsByName(ocMeridiaAuto);
    } // setupUi

    void retranslateUi(QMainWindow *ocMeridiaAuto)
    {
        ocMeridiaAuto->setWindowTitle(QApplication::translate("ocMeridiaAuto", "ocMeridiaAuto", 0));
    } // retranslateUi

};

namespace Ui {
    class ocMeridiaAuto: public Ui_ocMeridiaAuto {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OCMERIDIAAUTO_H
