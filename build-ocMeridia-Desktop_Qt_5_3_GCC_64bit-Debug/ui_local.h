/********************************************************************************
** Form generated from reading UI file 'local.ui'
**
** Created by: Qt User Interface Compiler version 5.3.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCAL_H
#define UI_LOCAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_local
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_4;
    QSplitter *splitter;
    QGroupBox *groupBox_category;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton_category_del;
    QSpacerItem *verticalSpacer;
    QTableWidget *tableWidget_category;
    QGroupBox *groupBox_attribute;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QToolButton *toolButton_attribute_del;
    QSpacerItem *verticalSpacer_3;
    QTableWidget *tableWidget_attribute;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_reconnect;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_close;

    void setupUi(QDialog *local)
    {
        if (local->objectName().isEmpty())
            local->setObjectName(QStringLiteral("local"));
        local->resize(789, 564);
        QFont font;
        font.setPointSize(10);
        local->setFont(font);
        gridLayout_3 = new QGridLayout(local);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        splitter = new QSplitter(local);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        groupBox_category = new QGroupBox(splitter);
        groupBox_category->setObjectName(QStringLiteral("groupBox_category"));
        gridLayout = new QGridLayout(groupBox_category);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        toolButton_category_del = new QToolButton(groupBox_category);
        toolButton_category_del->setObjectName(QStringLiteral("toolButton_category_del"));
        toolButton_category_del->setMinimumSize(QSize(30, 30));
        toolButton_category_del->setMaximumSize(QSize(30, 30));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        toolButton_category_del->setFont(font1);

        verticalLayout->addWidget(toolButton_category_del);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        tableWidget_category = new QTableWidget(groupBox_category);
        if (tableWidget_category->columnCount() < 4)
            tableWidget_category->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_category->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_category->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_category->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_category->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget_category->setObjectName(QStringLiteral("tableWidget_category"));
        tableWidget_category->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_category->setAlternatingRowColors(true);
        tableWidget_category->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_category->setShowGrid(false);
        tableWidget_category->setWordWrap(false);
        tableWidget_category->horizontalHeader()->setStretchLastSection(true);
        tableWidget_category->verticalHeader()->setDefaultSectionSize(25);

        horizontalLayout->addWidget(tableWidget_category);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        splitter->addWidget(groupBox_category);
        groupBox_attribute = new QGroupBox(splitter);
        groupBox_attribute->setObjectName(QStringLiteral("groupBox_attribute"));
        gridLayout_2 = new QGridLayout(groupBox_attribute);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        toolButton_attribute_del = new QToolButton(groupBox_attribute);
        toolButton_attribute_del->setObjectName(QStringLiteral("toolButton_attribute_del"));
        toolButton_attribute_del->setMinimumSize(QSize(30, 30));
        toolButton_attribute_del->setMaximumSize(QSize(30, 30));
        toolButton_attribute_del->setFont(font1);

        verticalLayout_3->addWidget(toolButton_attribute_del);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        horizontalLayout_3->addLayout(verticalLayout_3);

        tableWidget_attribute = new QTableWidget(groupBox_attribute);
        if (tableWidget_attribute->columnCount() < 3)
            tableWidget_attribute->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_attribute->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_attribute->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget_attribute->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        tableWidget_attribute->setObjectName(QStringLiteral("tableWidget_attribute"));
        tableWidget_attribute->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_attribute->setAlternatingRowColors(true);
        tableWidget_attribute->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_attribute->setShowGrid(false);
        tableWidget_attribute->setWordWrap(false);
        tableWidget_attribute->horizontalHeader()->setStretchLastSection(true);
        tableWidget_attribute->verticalHeader()->setDefaultSectionSize(25);

        horizontalLayout_3->addWidget(tableWidget_attribute);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        splitter->addWidget(groupBox_attribute);

        verticalLayout_4->addWidget(splitter);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_reconnect = new QPushButton(local);
        pushButton_reconnect->setObjectName(QStringLiteral("pushButton_reconnect"));
        pushButton_reconnect->setMinimumSize(QSize(140, 30));
        pushButton_reconnect->setMaximumSize(QSize(140, 30));
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        pushButton_reconnect->setFont(font2);

        horizontalLayout_4->addWidget(pushButton_reconnect);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_close = new QPushButton(local);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));
        pushButton_close->setMinimumSize(QSize(110, 30));
        pushButton_close->setMaximumSize(QSize(110, 30));
        pushButton_close->setFont(font2);

        horizontalLayout_4->addWidget(pushButton_close);


        verticalLayout_4->addLayout(horizontalLayout_4);


        gridLayout_3->addLayout(verticalLayout_4, 0, 0, 1, 1);


        retranslateUi(local);

        QMetaObject::connectSlotsByName(local);
    } // setupUi

    void retranslateUi(QDialog *local)
    {
        local->setWindowTitle(QApplication::translate("local", "\320\241\320\262\321\217\320\267\320\270", 0));
        groupBox_category->setTitle(QApplication::translate("local", "\320\241\320\262\321\217\320\267\320\260\320\275\320\275\321\213\320\265 \320\272\320\260\321\202\320\265\320\263\320\276\321\200\320\270\320\270", 0));
        toolButton_category_del->setText(QApplication::translate("local", "-", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_category->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("local", "id", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_category->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("local", "\320\232\320\276\320\264", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_category->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("local", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_category->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("local", "\320\241\320\262\321\217\320\267\320\260\320\275\320\276 \321\201", 0));
        groupBox_attribute->setTitle(QApplication::translate("local", "\320\241\320\262\321\217\320\267\320\260\320\275\320\275\321\213\320\265 \320\260\321\202\321\200\320\270\320\261\321\203\321\202\321\213", 0));
        toolButton_attribute_del->setText(QApplication::translate("local", "-", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_attribute->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QApplication::translate("local", "id", 0));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_attribute->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QApplication::translate("local", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget_attribute->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QApplication::translate("local", "\320\241\320\262\321\217\320\267\320\260\320\275\320\276 \321\201", 0));
        pushButton_reconnect->setText(QApplication::translate("local", "\320\236\321\207\320\270\321\201\321\202\320\270\321\202\321\214 \321\201\320\262\321\217\320\267\320\270", 0));
        pushButton_close->setText(QApplication::translate("local", "\320\227\320\260\320\272\321\200\321\213\321\202\321\214", 0));
    } // retranslateUi

};

namespace Ui {
    class local: public Ui_local {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCAL_H
