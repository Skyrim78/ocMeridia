#include "ctrlsproduct.h"

ctrlSProduct::ctrlSProduct()
{

}

void ctrlSProduct::set_current(QSqlDatabase DB, QString PREFIX, int ID)
{
    PROD.clear();
    QSqlQuery query(QString("SELECT %0_product.model, %0_product.sku, %0_product.quantity, %0_product.stock_status_id, "
                            "%0_product.manufacturer_id, %0_product.price, %0_product.tax_class.id, %0_product.status, "
                            "%0_product.date_modified"
                            "FROM %0_product "
                            "WHERE %0_product.product_id = \'%1\' ")
                    .arg(PREFIX)
                    .arg(ID), DB);
    query.next();
    PROD.insert("model", query.value(0).toString());
    PROD.insert("sku", query.value(1).toString());
    PROD.insert("quantity", query.value(2).toInt());
    PROD.insert("stockStatusId", query.value(3).toInt());
    PROD.insert("manufacturerId", query.value(4).toInt());
    PROD.insert("price", query.value(5).toDouble());
    PROD.insert("taxClassId", query.value(6).toInt());
    PROD.insert("status", query.value(7).toInt());
    PROD.insert("dateModified", query.value(8).toDateTime());
}

QString ctrlSProduct::get_model()
{
    return PROD.value("model").toString();
}

QString ctrlSProduct::get_sku()
{
    return PROD.value("sku").toString();
}

int ctrlSProduct::get_quantity()
{
    return PROD.value("quantity").toInt();
}

int ctrlSProduct::get_stockStatusId()
{
    return PROD.value("stockStatusId").toInt();
}

int ctrlSProduct::get_manufacturerId()
{
    return PROD.value("manufacturerId").toInt();
}

double ctrlSProduct::get_price()
{
    return PROD.value("price").toDouble();
}

int ctrlSProduct::get_taxClassId()
{
    return PROD.value("taxClassId").toInt();
}

int ctrlSProduct::get_status()
{
    return PROD.value("status").toInt();
}

QDateTime ctrlSProduct::get_dateModified()
{
    return PROD.value("dateModified").toDateTime();
}
