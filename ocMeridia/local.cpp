#include "local.h"
#include "ui_local.h"

local::local(QSqlDatabase db_local, QSqlDatabase db_server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::local)
{
    ui->setupUi(this);

    DB_LOCAL = db_local;
    DB_SERVER = db_server;

    category_load();
    connect(ui->toolButton_category_del, SIGNAL(clicked(bool)), this, SLOT(category_reconnect()));

    attribute_load();
    connect(ui->toolButton_attribute_del, SIGNAL(clicked(bool)), this, SLOT(attribute_reconnect()));

    connect(ui->pushButton_reconnect, SIGNAL(clicked(bool)), this, SLOT(total_reconnect()));
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(close()));
}

local::~local()
{
    delete ui;
}

void local::category_load()
{
    for (int r = ui->tableWidget_category->rowCount(); r >= 0; r--){
        ui->tableWidget_category->removeRow(r);
    }

    QSqlQuery query("SELECT cat.id, cat.cod, cat.name, cat.id_db "
                    "FROM cat ", DB_LOCAL);
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
        QTableWidgetItem *itemA = new QTableWidgetItem();
        itemA->setText(QString("ID:%1 - %2")
                       .arg(query.value(3).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_category->setItem(row, 3, itemA);

        row++;
    }
    ui->tableWidget_category->setColumnHidden(0, true);
    ui->tableWidget_category->resizeColumnsToContents();
    ui->tableWidget_category->horizontalHeader()->setStretchLastSection(true);
}

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
        QTableWidgetItem *itemA = new QTableWidgetItem();
        itemA->setText(QString("ID:%1 - %2")
                       .arg(query.value(2).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_attribute->setItem(row, 2, itemA);

        row++;
    }
    ui->tableWidget_attribute->setColumnHidden(0, true);
    ui->tableWidget_attribute->resizeColumnsToContents();
    ui->tableWidget_attribute->horizontalHeader()->setStretchLastSection(true);
}

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
}
