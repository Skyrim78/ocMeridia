/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_setting
{
public:
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_1c;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_file1c;
    QToolButton *toolButton_file1c;
    QGroupBox *groupBox_pl;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_6;
    QVBoxLayout *verticalLayout;
    QToolButton *toolButton_pl_add;
    QToolButton *toolButton_pl_del_N;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_pl_form;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QLineEdit *lineEdit_pl_name;
    QToolButton *toolButton_pl_sel_name;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_2;
    QLineEdit *lineEdit_pl_schema;
    QToolButton *toolButton_pl_sel_schema;
    QHBoxLayout *horizontalLayout_3;
    QCheckBox *checkBox_pl_active;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_pl_del;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_pl_save;
    QTableWidget *tableWidget_pl;
    QGroupBox *groupBox_messa;
    QGridLayout *gridLayout_6;
    QLabel *l_message;

    void setupUi(QDialog *setting)
    {
        if (setting->objectName().isEmpty())
            setting->setObjectName(QStringLiteral("setting"));
        setting->resize(736, 613);
        QFont font;
        font.setPointSize(10);
        setting->setFont(font);
        gridLayout_3 = new QGridLayout(setting);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_1c = new QGroupBox(setting);
        groupBox_1c->setObjectName(QStringLiteral("groupBox_1c"));
        horizontalLayout = new QHBoxLayout(groupBox_1c);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        lineEdit_file1c = new QLineEdit(groupBox_1c);
        lineEdit_file1c->setObjectName(QStringLiteral("lineEdit_file1c"));
        lineEdit_file1c->setMinimumSize(QSize(0, 25));
        lineEdit_file1c->setMaximumSize(QSize(16777215, 25));

        horizontalLayout->addWidget(lineEdit_file1c);

        toolButton_file1c = new QToolButton(groupBox_1c);
        toolButton_file1c->setObjectName(QStringLiteral("toolButton_file1c"));
        toolButton_file1c->setMinimumSize(QSize(30, 30));
        toolButton_file1c->setMaximumSize(QSize(30, 30));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        toolButton_file1c->setFont(font1);

        horizontalLayout->addWidget(toolButton_file1c);


        verticalLayout_3->addWidget(groupBox_1c);

        groupBox_pl = new QGroupBox(setting);
        groupBox_pl->setObjectName(QStringLiteral("groupBox_pl"));
        gridLayout_2 = new QGridLayout(groupBox_pl);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        toolButton_pl_add = new QToolButton(groupBox_pl);
        toolButton_pl_add->setObjectName(QStringLiteral("toolButton_pl_add"));
        toolButton_pl_add->setMinimumSize(QSize(30, 30));
        toolButton_pl_add->setMaximumSize(QSize(30, 30));
        toolButton_pl_add->setFont(font1);

        verticalLayout->addWidget(toolButton_pl_add);

        toolButton_pl_del_N = new QToolButton(groupBox_pl);
        toolButton_pl_del_N->setObjectName(QStringLiteral("toolButton_pl_del_N"));
        toolButton_pl_del_N->setMinimumSize(QSize(30, 30));
        toolButton_pl_del_N->setMaximumSize(QSize(30, 30));
        toolButton_pl_del_N->setFont(font1);

        verticalLayout->addWidget(toolButton_pl_del_N);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_6->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_pl_form = new QGroupBox(groupBox_pl);
        groupBox_pl_form->setObjectName(QStringLiteral("groupBox_pl_form"));
        gridLayout = new QGridLayout(groupBox_pl_form);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label = new QLabel(groupBox_pl_form);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(155, 0));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label);

        lineEdit_pl_name = new QLineEdit(groupBox_pl_form);
        lineEdit_pl_name->setObjectName(QStringLiteral("lineEdit_pl_name"));
        lineEdit_pl_name->setMinimumSize(QSize(0, 25));
        lineEdit_pl_name->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_4->addWidget(lineEdit_pl_name);

        toolButton_pl_sel_name = new QToolButton(groupBox_pl_form);
        toolButton_pl_sel_name->setObjectName(QStringLiteral("toolButton_pl_sel_name"));
        toolButton_pl_sel_name->setMinimumSize(QSize(30, 30));
        toolButton_pl_sel_name->setMaximumSize(QSize(30, 30));
        toolButton_pl_sel_name->setFont(font1);

        horizontalLayout_4->addWidget(toolButton_pl_sel_name);


        gridLayout->addLayout(horizontalLayout_4, 0, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_2 = new QLabel(groupBox_pl_form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(155, 0));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_2);

        lineEdit_pl_schema = new QLineEdit(groupBox_pl_form);
        lineEdit_pl_schema->setObjectName(QStringLiteral("lineEdit_pl_schema"));
        lineEdit_pl_schema->setMinimumSize(QSize(0, 25));
        lineEdit_pl_schema->setMaximumSize(QSize(16777215, 25));

        horizontalLayout_5->addWidget(lineEdit_pl_schema);

        toolButton_pl_sel_schema = new QToolButton(groupBox_pl_form);
        toolButton_pl_sel_schema->setObjectName(QStringLiteral("toolButton_pl_sel_schema"));
        toolButton_pl_sel_schema->setMinimumSize(QSize(30, 30));
        toolButton_pl_sel_schema->setMaximumSize(QSize(30, 30));
        toolButton_pl_sel_schema->setFont(font1);

        horizontalLayout_5->addWidget(toolButton_pl_sel_schema);


        gridLayout->addLayout(horizontalLayout_5, 1, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        checkBox_pl_active = new QCheckBox(groupBox_pl_form);
        checkBox_pl_active->setObjectName(QStringLiteral("checkBox_pl_active"));

        horizontalLayout_3->addWidget(checkBox_pl_active);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_pl_del = new QPushButton(groupBox_pl_form);
        pushButton_pl_del->setObjectName(QStringLiteral("pushButton_pl_del"));
        pushButton_pl_del->setMinimumSize(QSize(110, 30));
        pushButton_pl_del->setMaximumSize(QSize(110, 30));
        pushButton_pl_del->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_pl_del);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_pl_save = new QPushButton(groupBox_pl_form);
        pushButton_pl_save->setObjectName(QStringLiteral("pushButton_pl_save"));
        pushButton_pl_save->setMinimumSize(QSize(110, 30));
        pushButton_pl_save->setMaximumSize(QSize(110, 30));
        pushButton_pl_save->setFont(font1);

        horizontalLayout_2->addWidget(pushButton_pl_save);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 1);


        verticalLayout_2->addWidget(groupBox_pl_form);

        tableWidget_pl = new QTableWidget(groupBox_pl);
        if (tableWidget_pl->columnCount() < 3)
            tableWidget_pl->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_pl->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_pl->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_pl->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget_pl->setObjectName(QStringLiteral("tableWidget_pl"));
        tableWidget_pl->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_pl->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_pl->verticalHeader()->setDefaultSectionSize(20);

        verticalLayout_2->addWidget(tableWidget_pl);


        horizontalLayout_6->addLayout(verticalLayout_2);


        gridLayout_2->addLayout(horizontalLayout_6, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_pl);

        groupBox_messa = new QGroupBox(setting);
        groupBox_messa->setObjectName(QStringLiteral("groupBox_messa"));
        gridLayout_6 = new QGridLayout(groupBox_messa);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        l_message = new QLabel(groupBox_messa);
        l_message->setObjectName(QStringLiteral("l_message"));
        l_message->setFont(font1);
        l_message->setScaledContents(true);
        l_message->setAlignment(Qt::AlignCenter);
        l_message->setWordWrap(true);

        gridLayout_6->addWidget(l_message, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_messa);


        gridLayout_3->addLayout(verticalLayout_3, 0, 0, 1, 1);


        retranslateUi(setting);

        QMetaObject::connectSlotsByName(setting);
    } // setupUi

    void retranslateUi(QDialog *setting)
    {
        setting->setWindowTitle(QApplication::translate("setting", "\320\244\320\260\320\271\320\273\321\213 \320\260\320\262\321\202\320\276\320\267\320\260\320\263\321\200\321\203\320\267\320\272\320\270", 0));
        groupBox_1c->setTitle(QApplication::translate("setting", "\320\222\321\213\320\263\321\200\321\203\320\267\320\272\320\260 1\320\241", 0));
        toolButton_file1c->setText(QApplication::translate("setting", "...", 0));
        groupBox_pl->setTitle(QApplication::translate("setting", "\320\237\321\200\320\260\320\271\321\201-\320\273\320\270\321\201\321\202\321\213", 0));
        toolButton_pl_add->setText(QApplication::translate("setting", "+", 0));
        toolButton_pl_del_N->setText(QApplication::translate("setting", "-", 0));
        groupBox_pl_form->setTitle(QString());
        label->setText(QApplication::translate("setting", "\320\240\320\260\321\201\320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265 \321\204\320\260\320\271\320\273\320\260:", 0));
        toolButton_pl_sel_name->setText(QApplication::translate("setting", "...", 0));
        label_2->setText(QApplication::translate("setting", "\320\241\321\205\320\265\320\274\320\260 \321\200\320\260\320\267\320\261\320\276\321\200\320\260:", 0));
        toolButton_pl_sel_schema->setText(QApplication::translate("setting", "...", 0));
        checkBox_pl_active->setText(QApplication::translate("setting", "\320\222 \321\201\320\277\320\270\321\201\320\272\320\265 \320\260\320\262\321\202\320\276\320\267\320\260\320\263\321\200\321\203\320\267\320\272\320\270", 0));
        pushButton_pl_del->setText(QApplication::translate("setting", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", 0));
        pushButton_pl_save->setText(QApplication::translate("setting", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_pl->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("setting", "\321\210\320\262", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_pl->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("setting", "\320\244\320\260\320\271\320\273", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_pl->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("setting", "\320\260\320\272\321\202", 0));
        groupBox_messa->setTitle(QString());
        l_message->setText(QApplication::translate("setting", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class setting: public Ui_setting {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
