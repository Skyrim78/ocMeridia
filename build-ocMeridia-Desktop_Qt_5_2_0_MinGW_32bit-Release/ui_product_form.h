/********************************************************************************
** Form generated from reading UI file 'product_form.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRODUCT_FORM_H
#define UI_PRODUCT_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_product_form
{
public:
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout_7;
    QSplitter *splitter;
    QGroupBox *groupBox_file;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *tableWidget_f_product;
    QGroupBox *groupBox_shop;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_6;
    QCheckBox *checkBox_s_filter;
    QComboBox *comboBox_s_group;
    QSpacerItem *horizontalSpacer_7;
    QRadioButton *radioButton_s_all;
    QRadioButton *radioButton_s_cod;
    QTableWidget *tableWidget_s_product;
    QGroupBox *groupBox_form;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QRadioButton *radioButton_add;
    QSpacerItem *horizontalSpacer_2;
    QRadioButton *radioButton_equi;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox_form_group;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_name;
    QToolButton *toolButton_clear;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_save;
    QGroupBox *groupBox_info;
    QGridLayout *gridLayout_2;
    QTextEdit *textEdit_info;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_messa;
    QGridLayout *gridLayout_6;
    QLabel *l_message;
    QProgressBar *progressBar;

    void setupUi(QDialog *product_form)
    {
        if (product_form->objectName().isEmpty())
            product_form->setObjectName(QStringLiteral("product_form"));
        product_form->resize(907, 622);
        QFont font;
        font.setPointSize(10);
        product_form->setFont(font);
        gridLayout_5 = new QGridLayout(product_form);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        splitter = new QSplitter(product_form);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        groupBox_file = new QGroupBox(splitter);
        groupBox_file->setObjectName(QStringLiteral("groupBox_file"));
        gridLayout_3 = new QGridLayout(groupBox_file);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        tableWidget_f_product = new QTableWidget(groupBox_file);
        if (tableWidget_f_product->columnCount() < 2)
            tableWidget_f_product->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_f_product->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_f_product->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget_f_product->setObjectName(QStringLiteral("tableWidget_f_product"));
        tableWidget_f_product->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_f_product->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_f_product->verticalHeader()->setDefaultSectionSize(20);
        tableWidget_f_product->verticalHeader()->setMinimumSectionSize(20);

        verticalLayout_2->addWidget(tableWidget_f_product);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);

        splitter->addWidget(groupBox_file);
        groupBox_shop = new QGroupBox(splitter);
        groupBox_shop->setObjectName(QStringLiteral("groupBox_shop"));
        gridLayout_4 = new QGridLayout(groupBox_shop);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        checkBox_s_filter = new QCheckBox(groupBox_shop);
        checkBox_s_filter->setObjectName(QStringLiteral("checkBox_s_filter"));

        horizontalLayout_6->addWidget(checkBox_s_filter);

        comboBox_s_group = new QComboBox(groupBox_shop);
        comboBox_s_group->setObjectName(QStringLiteral("comboBox_s_group"));

        horizontalLayout_6->addWidget(comboBox_s_group);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_7);

        radioButton_s_all = new QRadioButton(groupBox_shop);
        radioButton_s_all->setObjectName(QStringLiteral("radioButton_s_all"));
        radioButton_s_all->setChecked(true);

        horizontalLayout_6->addWidget(radioButton_s_all);

        radioButton_s_cod = new QRadioButton(groupBox_shop);
        radioButton_s_cod->setObjectName(QStringLiteral("radioButton_s_cod"));

        horizontalLayout_6->addWidget(radioButton_s_cod);


        verticalLayout_3->addLayout(horizontalLayout_6);

        tableWidget_s_product = new QTableWidget(groupBox_shop);
        if (tableWidget_s_product->columnCount() < 3)
            tableWidget_s_product->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_s_product->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_s_product->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_s_product->setHorizontalHeaderItem(2, __qtablewidgetitem4);
        tableWidget_s_product->setObjectName(QStringLiteral("tableWidget_s_product"));
        tableWidget_s_product->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_s_product->setSelectionMode(QAbstractItemView::SingleSelection);
        tableWidget_s_product->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableWidget_s_product->verticalHeader()->setDefaultSectionSize(20);
        tableWidget_s_product->verticalHeader()->setMinimumSectionSize(20);

        verticalLayout_3->addWidget(tableWidget_s_product);


        gridLayout_4->addLayout(verticalLayout_3, 0, 0, 1, 1);

        splitter->addWidget(groupBox_shop);

        horizontalLayout_7->addWidget(splitter);

        groupBox_form = new QGroupBox(product_form);
        groupBox_form->setObjectName(QStringLiteral("groupBox_form"));
        groupBox_form->setMinimumSize(QSize(350, 0));
        groupBox_form->setMaximumSize(QSize(350, 16777215));
        gridLayout = new QGridLayout(groupBox_form);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        radioButton_add = new QRadioButton(groupBox_form);
        radioButton_add->setObjectName(QStringLiteral("radioButton_add"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        radioButton_add->setFont(font1);
        radioButton_add->setChecked(true);

        horizontalLayout->addWidget(radioButton_add);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        radioButton_equi = new QRadioButton(groupBox_form);
        radioButton_equi->setObjectName(QStringLiteral("radioButton_equi"));
        radioButton_equi->setFont(font1);

        horizontalLayout->addWidget(radioButton_equi);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(groupBox_form);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label);

        comboBox_form_group = new QComboBox(groupBox_form);
        comboBox_form_group->setObjectName(QStringLiteral("comboBox_form_group"));
        comboBox_form_group->setMinimumSize(QSize(0, 25));
        comboBox_form_group->setMaximumSize(QSize(16777215, 25));
        comboBox_form_group->setSizeAdjustPolicy(QComboBox::AdjustToContents);

        horizontalLayout_2->addWidget(comboBox_form_group);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(groupBox_form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_2);

        lineEdit_name = new QLineEdit(groupBox_form);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setMinimumSize(QSize(0, 25));
        lineEdit_name->setMaximumSize(QSize(16777215, 25));
        lineEdit_name->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEdit_name->setClearButtonEnabled(false);

        horizontalLayout_3->addWidget(lineEdit_name);

        toolButton_clear = new QToolButton(groupBox_form);
        toolButton_clear->setObjectName(QStringLiteral("toolButton_clear"));
        toolButton_clear->setMinimumSize(QSize(25, 25));
        toolButton_clear->setMaximumSize(QSize(25, 25));
        toolButton_clear->setArrowType(Qt::LeftArrow);

        horizontalLayout_3->addWidget(toolButton_clear);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_5);

        pushButton_save = new QPushButton(groupBox_form);
        pushButton_save->setObjectName(QStringLiteral("pushButton_save"));
        pushButton_save->setMinimumSize(QSize(110, 30));
        pushButton_save->setMaximumSize(QSize(110, 30));

        horizontalLayout_4->addWidget(pushButton_save);


        verticalLayout->addLayout(horizontalLayout_4);

        groupBox_info = new QGroupBox(groupBox_form);
        groupBox_info->setObjectName(QStringLiteral("groupBox_info"));
        groupBox_info->setFont(font1);
        groupBox_info->setAlignment(Qt::AlignCenter);
        gridLayout_2 = new QGridLayout(groupBox_info);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        textEdit_info = new QTextEdit(groupBox_info);
        textEdit_info->setObjectName(QStringLiteral("textEdit_info"));
        QFont font2;
        font2.setBold(false);
        font2.setWeight(50);
        textEdit_info->setFont(font2);

        gridLayout_2->addWidget(textEdit_info, 0, 0, 1, 1);


        verticalLayout->addWidget(groupBox_info);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        groupBox_messa = new QGroupBox(groupBox_form);
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


        verticalLayout->addWidget(groupBox_messa);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        horizontalLayout_7->addWidget(groupBox_form);


        gridLayout_5->addLayout(horizontalLayout_7, 0, 0, 1, 1);

        progressBar = new QProgressBar(product_form);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setMaximumSize(QSize(16777215, 10));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout_5->addWidget(progressBar, 1, 0, 1, 1);


        retranslateUi(product_form);

        QMetaObject::connectSlotsByName(product_form);
    } // setupUi

    void retranslateUi(QDialog *product_form)
    {
        product_form->setWindowTitle(QApplication::translate("product_form", "\320\242\320\276\320\262\320\260\321\200\321\213", 0));
        groupBox_file->setTitle(QApplication::translate("product_form", "\320\242\320\276\320\262\320\260\321\200\321\213 \320\275\320\260 \320\276\320\261\321\200\320\260\320\261\320\276\321\202\320\272\321\203", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_f_product->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("product_form", "\320\220\321\200\321\202\320\270\320\272\321\203\320\273", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_f_product->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("product_form", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        groupBox_shop->setTitle(QApplication::translate("product_form", "\320\242\320\276\320\262\320\260\321\200\321\213 \320\274\320\260\320\263\320\260\320\267\320\270\320\275\320\260", 0));
        checkBox_s_filter->setText(QApplication::translate("product_form", "\321\204\320\270\320\273\321\214\321\202\321\200\320\276\320\262\320\260\321\202\321\214 \320\277\320\276 \320\263\321\200\321\203\320\277\320\277\320\265", 0));
        radioButton_s_all->setText(QApplication::translate("product_form", "\320\262\321\201\320\265", 0));
        radioButton_s_cod->setText(QApplication::translate("product_form", "\320\261\320\265\320\267 \320\232\320\276\320\264\320\260 1\320\241", 0));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_s_product->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QApplication::translate("product_form", "id", 0));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_s_product->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QApplication::translate("product_form", "\320\223\321\200\321\203\320\277\320\277\320\260", 0));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_s_product->horizontalHeaderItem(2);
        ___qtablewidgetitem4->setText(QApplication::translate("product_form", "\320\235\320\260\320\270\320\274\320\265\320\275\320\276\320\262\320\260\320\275\320\270\320\265", 0));
        groupBox_form->setTitle(QString());
        radioButton_add->setText(QApplication::translate("product_form", "+", 0));
        radioButton_equi->setText(QApplication::translate("product_form", "=", 0));
        label->setText(QApplication::translate("product_form", "\320\223\321\200\321\203\320\277\320\277\320\260:", 0));
        label_2->setText(QApplication::translate("product_form", "\320\242\320\276\320\262\320\260\321\200:", 0));
        toolButton_clear->setText(QApplication::translate("product_form", "...", 0));
        pushButton_save->setText(QApplication::translate("product_form", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", 0));
        groupBox_info->setTitle(QApplication::translate("product_form", "info", 0));
        groupBox_messa->setTitle(QString());
        l_message->setText(QApplication::translate("product_form", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class product_form: public Ui_product_form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRODUCT_FORM_H
