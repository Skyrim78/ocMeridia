#include "local.h"
#include "ui_local.h"

local::local(QSqlDatabase db_local, QSqlDatabase db_server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::local)
{
    ui->setupUi(this);

    DB_LOCAL = db_local;
    DB_SERVER = db_server;

<<<<<<< HEAD
    load_category();
    load_attribute();

    connect(ui->toolButton_category_del, SIGNAL(clicked(bool)), this, SLOT(clear_category()));
    connect(ui->toolButton_attribute_del, SIGNAL(clicked(bool)), this, SLOT(clear_attribute()));
    connect(ui->pushButton_clear, SIGNAL(clicked(bool)), this, SLOT(total_clear()));
=======
    category_load();
    connect(ui->toolButton_category_del, SIGNAL(clicked(bool)), this, SLOT(category_reconnect()));

    attribute_load();
    connect(ui->toolButton_attribute_del, SIGNAL(clicked(bool)), this, SLOT(attribute_reconnect()));

    connect(ui->pushButton_reconnect, SIGNAL(clicked(bool)), this, SLOT(total_reconnect()));
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(close()));
}

local::~local()
{
    delete ui;
}

<<<<<<< HEAD
void local::load_category()
{
    for (int r = ui->tableWidget_category->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_category->removeRow(r);
    }

    QSqlQuery query("SELECT cat.id, cat.cod, cat.name, cat.id_db FROM cat ", DB_LOCAL);
=======
void local::category_load()
{
    for (int r = ui->tableWidget_category->rowCount(); r >= 0; r--){
        ui->tableWidget_category->removeRow(r);
    }

    QSqlQuery query("SELECT cat.id, cat.cod, cat.name, cat.id_db "
                    "FROM cat ", DB_LOCAL);
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364
    int row = 0;
    while (query.next()) {
        ui->tableWidget_category->insertRow(row);
        for (int col = 0; col < 3; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableWidget_category->setItem(row, col, item);
        }

        QSqlQuery queryA(QString("SELECT rmrt_category_description.name "
                                 "FROM rmrt_category_description "
                                 "WHERE rmrt_category_description.category_id = \'%1\' ")
                         .arg(query.value(3).toString()), DB_SERVER);
        queryA.next();
<<<<<<< HEAD
        QTableWidgetItem *itemX = new QTableWidgetItem();
        itemX->setText(QString("ID:%1 - %2")
                       .arg(query.value(3).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_category->setItem(row, 3, itemX);
=======
        QTableWidgetItem *itemA = new QTableWidgetItem();
        itemA->setText(QString("ID:%1 - %2")
                       .arg(query.value(3).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_category->setItem(row, 3, itemA);
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364

        row++;
    }
    ui->tableWidget_category->setColumnHidden(0, true);
    ui->tableWidget_category->resizeColumnsToContents();
    ui->tableWidget_category->horizontalHeader()->setStretchLastSection(true);
}

<<<<<<< HEAD
void local::clear_category()
{
    if (ui->tableWidget_category->rowCount() > 0){
        for (int row = 0; row < ui->tableWidget_category->rowCount(); row++){
            if (ui->tableWidget_category->item(row, 0)->isSelected()){
                QSqlQuery query(QString("DELETE FROM cat WHERE cat.id = \'%1\' ").arg(ui->tableWidget_category->item(row, 0)->text()), DB_LOCAL);
                query.exec();
            }
        }
    }
    load_category();
}

void local::load_attribute()
{
    for (int r = ui->tableWidget_attribute->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_attribute->removeRow(r);
    }

    QSqlQuery query("SELECT att.id, att.name, att.id_db FROM att ", DB_LOCAL);
=======
void local::category_reconnect()
{
    for (int row = 0; row < ui->tableWidget_category->rowCount(); row++){
        if (ui->tableWidget_category->item(row, 0)->isSelected()){
            QSqlQuery query(QString("DELETE FROM cat WHERE cat.id = \'%1\' ")
                            .arg(ui->tableWidget_category->item(row, 0)->text()), DB_LOCAL);
            query.exec();
        }
    }
    category_load();
}

void local::attribute_load()
{
    for (int r = ui->tableWidget_attribute->rowCount(); r >= 0; r--){
        ui->tableWidget_attribute->removeRow(r);
    }

    QSqlQuery query("SELECT att.id, att.name, att.id_db "
                    "FROM att ", DB_LOCAL);
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364
    int row = 0;
    while (query.next()) {
        ui->tableWidget_attribute->insertRow(row);
        for (int col = 0; col < 2; col++){
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableWidget_attribute->setItem(row, col, item);
        }

        QSqlQuery queryA(QString("SELECT rmrt_attribute_description.name "
                                 "FROM rmrt_attribute_description "
                                 "WHERE rmrt_attribute_description.attribute_id = \'%1\' ")
                         .arg(query.value(2).toString()), DB_SERVER);
        queryA.next();
<<<<<<< HEAD
        QTableWidgetItem *itemX = new QTableWidgetItem();
        itemX->setText(QString("ID:%1 - %2")
                       .arg(query.value(2).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_attribute->setItem(row, 2, itemX);
=======
        QTableWidgetItem *itemA = new QTableWidgetItem();
        itemA->setText(QString("ID:%1 - %2")
                       .arg(query.value(2).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_attribute->setItem(row, 2, itemA);
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364

        row++;
    }
    ui->tableWidget_attribute->setColumnHidden(0, true);
    ui->tableWidget_attribute->resizeColumnsToContents();
    ui->tableWidget_attribute->horizontalHeader()->setStretchLastSection(true);
}

<<<<<<< HEAD
void local::clear_attribute()
{
    if (ui->tableWidget_attribute->rowCount() > 0){
        for (int row = 0; row < ui->tableWidget_attribute->rowCount(); row++){
            if (ui->tableWidget_attribute->item(row, 0)->isSelected()){
                QSqlQuery query(QString("DELETE FROM att WHERE att.id = \'%1\' ").arg(ui->tableWidget_attribute->item(row, 0)->text()), DB_LOCAL);
                query.exec();
            }
        }
    }
    load_attribute();
}

void local::total_clear()
{
    QSqlQuery queryA("DELETE FROM cat WHERE cat.id > \'0\' ", DB_LOCAL);
    queryA.exec();
    QSqlQuery queryB("DELETE FROM att WHERE att.id > \'0\' ", DB_LOCAL);
    queryB.exec();

    load_attribute();
    load_category();
=======
void local::attribute_reconnect()
{
    for (int row = 0; row < ui->tableWidget_attribute->rowCount(); row++){
        if (ui->tableWidget_attribute->item(row, 0)->isSelected()){
            QSqlQuery query(QString("DELETE FROM att WHERE att.id = \'%1\' ")
                            .arg(ui->tableWidget_attribute->item(row, 0)->text()), DB_LOCAL);
            query.exec();
        }
    }
    attribute_load();
}

void local::total_reconnect()
{
    QSqlQuery queryA("DELETE FROM att WHERE att.id > \'0\' ", DB_LOCAL);
    queryA.exec();
    QSqlQuery queryB("DELETE FROM cat WHERE cat.id > \'0\' ", DB_LOCAL);
    queryB.exec();

    category_load();
    attribute_load();
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364
}
