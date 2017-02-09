#include "local.h"
#include "ui_local.h"

local::local(QSqlDatabase db_local, QSqlDatabase db_server, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::local)
{
    ui->setupUi(this);

    DB_LOCAL = db_local;
    DB_SERVER = db_server;

    load_category();
    load_attribute();

    connect(ui->toolButton_category_del, SIGNAL(clicked(bool)), this, SLOT(clear_category()));
    connect(ui->toolButton_attribute_del, SIGNAL(clicked(bool)), this, SLOT(clear_attribute()));
    connect(ui->pushButton_clear, SIGNAL(clicked(bool)), this, SLOT(total_clear()));
    connect(ui->pushButton_close, SIGNAL(clicked(bool)), this, SLOT(close()));
}

local::~local()
{
    delete ui;
}

void local::load_category()
{
    for (int r = ui->tableWidget_category->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_category->removeRow(r);
    }

    QSqlQuery query("SELECT cat.id, cat.cod, cat.name, cat.id_db FROM cat ", DB_LOCAL);
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
        QTableWidgetItem *itemX = new QTableWidgetItem();
        itemX->setText(QString("ID:%1 - %2")
                       .arg(query.value(3).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_category->setItem(row, 3, itemX);

        row++;
    }
    ui->tableWidget_category->setColumnHidden(0, true);
    ui->tableWidget_category->resizeColumnsToContents();
    ui->tableWidget_category->horizontalHeader()->setStretchLastSection(true);
}

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
        QTableWidgetItem *itemX = new QTableWidgetItem();
        itemX->setText(QString("ID:%1 - %2")
                       .arg(query.value(2).toString())
                       .arg(queryA.value(0).toString()));
        ui->tableWidget_attribute->setItem(row, 2, itemX);

        row++;
    }
    ui->tableWidget_attribute->setColumnHidden(0, true);
    ui->tableWidget_attribute->resizeColumnsToContents();
    ui->tableWidget_attribute->horizontalHeader()->setStretchLastSection(true);
}

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
}
