#ifndef CTRLSPRODUCT_H
#define CTRLSPRODUCT_H

#include <QMap>
#include <QtSql>

//--
#include "ctrloptions.h"
#include "ctrlattribute.h"

class ctrlSProduct
{
public:
    ctrlSProduct(QSqlDatabase db_server, QString _pre, int _st);

    QSqlDatabase DB;
    QMap<QString, QVariant> PROD;

    QString PREFIX;
    int STORE;

    ctrlOptions *cOption;
    ctrlAttribute *cAttribute;

public slots:
    void set_current(int ID);
    QString get_model();
    QString get_sku();
    int get_quantity();
    int get_stockStatusId();
    int get_manufacturerId();
    double get_price();
    int get_taxClassId();
    int get_status();
    QDateTime get_dateModified();

    int test_hasProductOption(int productId, int optionId);


    int getIDforTestA(QString _sku, QString _name);
    int getIDforTestB(QString _sku);

    int getCountSpecial(int ID);

    QMap<QString, QVariant> saveProduct(QMap<QString, QVariant> saveMap,
                                        QMap<int, QMap<QString, QVariant> >saveMapOption, QMap<int, QMap<QString, QVariant> > saveMapAttribute, QMap<int, QMap<QString, QVariant> > saveMapImage);

    QString saveProductAttribute(QMap<QString, QVariant> saveMap);
    QString saveProductImage(QMap<QString, QVariant> saveMap);
    QString saveProductOption(QMap<QString, QVariant> saveMap);


    QMap<QString, QVariant> addProduct(QMap<QString, QVariant> saveMap);
    QString updateProduct(QMap<QString, QVariant> saveMap);

    QString addProductDescription(int productId, QMap<QString, QVariant> saveMap);
    QString updateProductDescription(QMap<QString, QVariant> saveMap);

    QString addProductToCategory(int productId, QMap<QString, QVariant> saveMap);
    QString updateProductToCategory(QMap<QString, QVariant> saveMap);

    QString addProductToStore(int productId);

    QString addProductSpecial(int productId, QMap<QString, QVariant> saveMap);
    QString updateProductSpecial(QMap<QString, QVariant> saveMap);
};

#endif // CTRLSPRODUCT_H
