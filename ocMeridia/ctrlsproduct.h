#ifndef CTRLSPRODUCT_H
#define CTRLSPRODUCT_H

#include <QMap>
#include <QtSql>

class ctrlSProduct
{
public:
    ctrlSProduct();

    QSqlDatabase db_server;
    QMap<QString, QVariant> PROD;

public slots:
    void set_current(QSqlDatabase DB, QString PREFIX, int ID);
    QString get_model();
    QString get_sku();
    int get_quantity();
    int get_stockStatusId();
    int get_manufacturerId();
    double get_price();
    int get_taxClassId();
    int get_status();
    QDateTime get_dateModified();
};

#endif // CTRLSPRODUCT_H
