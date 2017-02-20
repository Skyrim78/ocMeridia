#include "ctrlsproduct.h"

ctrlSProduct::ctrlSProduct(QSqlDatabase db_server, QString _pre, int _st)
{
    DB = db_server;
    PREFIX = _pre;
    STORE = _st;

    cOption = new ctrlOptions(DB, PREFIX);
    cAttribute = new ctrlAttribute(DB, PREFIX, STORE);
}

void ctrlSProduct::set_current(int ID)
{
    PROD.clear();
    QSqlQuery query(QString("SELECT %0_product.model, %0_product.sku, %0_product.quantity, %0_product.stock_status_id, "
                            "%0_product.manufacturer_id, %0_product.price, %0_product.tax_class_id, %0_product.status, "
                            "%0_product.date_modified "
                            "FROM %0_product "
                            "WHERE %0_product.product_id = \'%1\' ")
                    .arg(PREFIX)
                    .arg(ID), DB);
    query.next();
    //qDebug() << query.lastError();
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

int ctrlSProduct::test_hasProductOption(int productId, int optionId)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_product_option`.`product_option_id` "
                            "FROM `%0_product_option` "
                            "WHERE (`%0_product_option`.`product_id` = \'%1\') "
                            "AND (`%0_product_option`.`option_id` = \'%2\') ")
                    .arg(PREFIX)
                    .arg(productId)
                    .arg(optionId), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

int ctrlSProduct::getIDforTestB(QString _sku)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_product`.`product_id` "
                            "FROM `%0_product` "
                            "WHERE (`%0_product`.`sku` = \'%1\') ")
                    .arg(PREFIX)
                    .arg(_sku), DB);
    query.next();

    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

int ctrlSProduct::getCountSpecial(int ID)
{
    QSqlQuery query(QString("SELECT Count(`%0_product_special`.`product_special_id`) "
                            "FROM `%0_product_special` "
                            "WHERE `%0_product_special`.`product_id` = \'%1\' ").arg(PREFIX).arg(ID), DB);
    query.next();
    return query.value(0).toInt();
}

int ctrlSProduct::getIDforTestA(QString _sku, QString _name)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_product`.`product_id` "
                            "FROM `%0_product` "
                            "INNER JOIN `%0_product_description` ON (`%0_product_description`.`product_id` = `%0_product`.`product_id`) "
                            "WHERE (`%0_product`.`sku` = \'%1\')  AND (`%0_product_description`.`name` = \'%2\') ")
                    .arg(PREFIX)
                    .arg(_sku)
                    .arg(_name), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

QMap<QString, QVariant> ctrlSProduct::saveProduct(QMap<QString, QVariant> saveMap,
                                                  QMap<int, QMap<QString, QVariant> > saveMapOption,
                                                  QMap<int, QMap<QString, QVariant> > saveMapAttribute,
                                                  QMap<int, QMap<QString, QVariant> > saveMapImage)
{
    QString error;
    int productId = saveMap.value("productId").toInt();
    if (productId == 0){
        QMap<QString, QVariant> feedA = addProduct(saveMap);
        if (!feedA.value("error").toString().isEmpty()){
            error.append(feedA.value("error").toString());
        } else {
            productId = feedA.value("lastId").toInt();

            //add description
            QString eDesc = addProductDescription(productId, saveMap);
            if (!eDesc.isEmpty()){
                error.append(eDesc);
            }
            //add special
            int sprice = saveMap.value("specPrice").toDouble();
            if (sprice > 0){
                QString eSPrice = addProductSpecial(productId, saveMap);
                if (!eSPrice.isEmpty()){
                    error.append(eSPrice);
                }
            }
            //add to_category
            QString eGroup = addProductToCategory(productId, saveMap);
            if (!eGroup.isEmpty()){
                error.append(eGroup);
            }
            //add to_store
            QString eStore = addProductToStore(productId);
            if (!eStore.isEmpty()){
                error.append(eStore);
            }

            //add option + option value
            if (saveMapOption.size() > 0){
                for (int x = 0; x < saveMapOption.count(); x++){
                    QMap<QString,QVariant> _mapData = saveMapOption.value(x);

                    int prodId = getIDforTestB(_mapData.value("productId").toString());
                    int optId = cOption->get_optionID(_mapData.value("optionId").toString());
                    int optValueId = cOption->get_optionValueID(optId, _mapData.value("optionValueId").toString());


                    QMap<QString,QVariant> _mapTemp;
                    _mapTemp.insert("productId", prodId);
                    _mapTemp.insert("optionId", optId);
                    _mapTemp.insert("optionValueId", optValueId);
                    _mapTemp.insert("quantity", _mapData.value("quantity").toInt());

                    QString eOption = saveProductOption(_mapTemp);
                    if (!eOption.isEmpty()){
                        error.append(eOption);
                    }
                }
            }
            //add attribute
            if (saveMapAttribute.size() > 0){
                for (int x = 0; x < saveMapAttribute.count(); x++){
                    QMap<QString,QVariant> _mapData = saveMapAttribute.value(x);

                    int prodId = getIDforTestB(_mapData.value("productId").toString());
                    int attrId = cAttribute->get_attributeID(_mapData.value("attributeId").toString());


                    QMap<QString,QVariant> _mapTemp;
                    _mapTemp.insert("productId", prodId);
                    _mapTemp.insert("attributeId", attrId);
                    _mapTemp.insert("languageId", _mapData.value("languageId").toInt());
                    _mapTemp.insert("text", _mapData.value("text").toString());

                    QString eAttribute = saveProductAttribute(_mapTemp);
                    if (!eAttribute.isEmpty()){
                        error.append(eAttribute);
                    }
                }
            }
            //add image
            if (saveMapImage.size() > 0){
                for (int x = 0; x < saveMapImage.count(); x++){
                    QMap<QString,QVariant> _mapData = saveMapImage.value(x);

                    int prodId = getIDforTestB(_mapData.value("productId").toString());

                    QMap<QString,QVariant> _mapTemp;
                    _mapTemp.insert("productId", prodId);
                    _mapTemp.insert("image", _mapData.value("image").toString());



                    QString eImage = saveProductImage(_mapTemp);
                    if (!eImage.isEmpty()){
                        error.append(eImage);
                    }
                }
            }
        }

    } else {
        QString errorA = updateProduct(saveMap);
        if (!errorA.isEmpty()){
            error.append(QString("Ошибка обновления %0_product: %1").arg(PREFIX).arg(errorA));
        }
        //update description
        QString errorB = updateProductDescription(saveMap);
        if (!errorB.isEmpty()){
            error.append(QString("Ошибка обновления %0_product_description: %1").arg(PREFIX).arg(errorB));
        }
        //update special
        int sprice = saveMap.value("specPrice").toDouble();
        if (sprice > 0){
            QString eSPrice;
            if (getCountSpecial(productId) == 0){
                eSPrice = addProductSpecial(productId, saveMap);
            } else {
                eSPrice = updateProductSpecial(saveMap);
            }
            if (!eSPrice.isEmpty()){
                error.append(eSPrice);
            }
        }

        //update to_category
        int groupId = saveMap.value("groupId").toInt();
        if (groupId > 0){
            QString errorC = updateProductToCategory(saveMap);
            if (!errorC.isEmpty()){
                error.append(QString("Ошибка обновления %0_product_to_category: %1").arg(PREFIX).arg(errorC));
            }
        }

    }
    QMap<QString, QVariant> FEED;
    FEED.insert("error", error);
    FEED.insert("productId", productId);

    return FEED;
}


QString ctrlSProduct::saveProductAttribute(QMap<QString, QVariant> saveMap)
{
    QString _error;
    if (saveMap.value("save").toInt() == 0){
        QSqlQuery query(QString("INSERT INTO `%0_product_attribute` (`product_id`, `language_id`, `attribute_id`, `text`) "
                                  "VALUES (?, ?, ?, ?)").arg(PREFIX), DB);
        query.bindValue(0, saveMap.value("productId").toInt());
        query.bindValue(1, saveMap.value("languageId").toInt());
        query.bindValue(2, saveMap.value("attributeId").toInt());
        query.bindValue(3, saveMap.value("text").toString());
        query.exec();
        if (query.lastError().isValid()){
            qDebug() << "save product attribute - "<< query.lastError();
            //_error = query.lastError().text();
        }
    }

    return _error;
}

QString ctrlSProduct::saveProductImage(QMap<QString, QVariant> saveMap)
{
    QString _error;
    if (saveMap.value("id").toInt() == 0){
        QSqlQuery query(QString("INSERT INTO `%0_product_image` (`product_id`, `image`) "
                                  "VALUES (?, ?)").arg(PREFIX), DB);
        query.bindValue(0, saveMap.value("productId").toInt());
        query.bindValue(1, saveMap.value("image").toString());
        query.exec();
        if (query.lastError().isValid()){
            qDebug() << "save product image - "<< query.lastError();
            _error = query.lastError().text();
        }
    }
    return _error;
}

QString ctrlSProduct::saveProductOption(QMap<QString, QVariant> saveMap)
{
    QString _error;
    if (saveMap.value("").toInt() == 0){

        int productOptionId = test_hasProductOption(saveMap.value("productId").toInt(), saveMap.value("optionId").toInt());

        if (productOptionId == 0){
            QSqlQuery query(QString("INSERT INTO `%0_product_option` (`product_id`, `option_id`, `required`) "
                                      "VALUES (?, ?, ?)").arg(PREFIX), DB);
            query.bindValue(0, saveMap.value("productId").toInt());
            query.bindValue(1, saveMap.value("optionId").toInt());
            query.bindValue(2, 1);
            query.exec();
            if (query.lastError().isValid()){
                qDebug() << "save product option - "<< query.lastError();
                _error = query.lastError().text();
            } else {
                productOptionId = query.lastInsertId().toInt();
            }
        }

        if (productOptionId > 0){
            QSqlQuery queryB(QString("INSERT INTO `%0_product_option_value` (`product_option_id`, `product_id`, "
                                    "`option_id`, `option_value_id`, `quantity`, `subtract`, `price`) "
                                      "VALUES (?, ?, ?, ?, ?, ?, ?)").arg(PREFIX), DB);
            queryB.bindValue(0, productOptionId);
            queryB.bindValue(1, saveMap.value("productId").toInt());
            queryB.bindValue(2, saveMap.value("optionId").toInt());
            queryB.bindValue(3, saveMap.value("optionValueId").toInt());
            queryB.bindValue(4, saveMap.value("quantity").toInt());
            queryB.bindValue(5, 1);
            queryB.bindValue(6, 0);
            queryB.exec();
            if (queryB.lastError().isValid()){
                qDebug() << "save product optionB - "<< queryB.lastError();
                _error = queryB.lastError().text();
            }
        }
    }

    return _error;
}


QMap<QString, QVariant> ctrlSProduct::addProduct(QMap<QString, QVariant> saveMap)
{
    QMap<QString, QVariant> FEED;
    QSqlQuery query(QString("INSERT INTO `%0_product` (`sku`, `model`, `manufacturer_id`, `price`, `quantity`, `image`, "
                              "`stock_status_id`, `tax_class_id`, `status`, `date_added`, `date_modified`, `date_available`, "
                            "`minimum`) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)").arg(PREFIX), DB);
    query.bindValue(0, saveMap.value("sku").toString());
    query.bindValue(1, saveMap.value("model").toString());
    query.bindValue(2, saveMap.value("manufacturerId").toInt());
    query.bindValue(3, saveMap.value("price").toDouble());
    query.bindValue(4, saveMap.value("quantity").toInt());
    query.bindValue(5, saveMap.value("image").toString());
    query.bindValue(6, saveMap.value("stockStatusId").toInt());
    query.bindValue(7, saveMap.value("taxClassId").toInt());
    query.bindValue(8, saveMap.value("status").toInt());
    query.bindValue(9, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    query.bindValue(10, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    query.bindValue(11, QDate::currentDate().toString("yyyy.MM.dd"));
    query.bindValue(12, 1);
    query.exec();
    if (query.lastError().isValid()){
        qDebug() << "add product - "<< query.lastError();
        FEED.insert("error", query.lastError().text());
    } else {
        FEED.insert("lastId", query.lastInsertId().toInt());
    }
    return FEED;
}

QString ctrlSProduct::updateProduct(QMap<QString, QVariant> saveMap)
{
    QString error;
    QString _str = QString("UPDATE `%0_product` SET ").arg(PREFIX);
    if (!saveMap.value("articul").toString().isEmpty()){
        _str.append(QString("`sku` = \'%0\', ").arg(saveMap.value("articul").toString()));
    }
    if (!saveMap.value("model").toString().isEmpty()){
        _str.append(QString("`model` = \'%0\', ").arg(saveMap.value("model").toString()));
    }
    if (!saveMap.value("manufacturerId").toString().isEmpty()){
        _str.append(QString("`manufacturer_id` = \'%0\', ").arg(saveMap.value("manufacturerId").toInt()));
    }
    if (!saveMap.value("price").toString().isEmpty()){
        _str.append(QString("`price` = \'%0\', ").arg(saveMap.value("price").toDouble()));
    }
    if (!saveMap.value("quantity").toString().isEmpty()){
        _str.append(QString("`quantity` = \'%0\', ").arg(saveMap.value("quantity").toInt()));
    }
    if (!saveMap.value("image").toString().isEmpty()){
        _str.append(QString("`image` = \'%0\', ").arg(saveMap.value("image").toString()));
    }
    if (!saveMap.value("stockStatusId").toString().isEmpty()){
        _str.append(QString("`stock_status_id` = \'%0\', ").arg(saveMap.value("stockStatusId").toInt()));
    }
    if (!saveMap.value("taxClassId").toString().isEmpty()){
        _str.append(QString("`tax_class_id` = \'%0\', ").arg(saveMap.value("taxClassId").toInt()));
    }
    if (!saveMap.value("status").toString().isEmpty()){
        _str.append(QString("`status` = \'%0\', ").arg(saveMap.value("status").toInt()));
    }
    _str.append(QString("`date_modified` = \'%0\' WHERE `%1_product`.`product_id` = \'%2\' ")
                .arg(QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"))
                .arg(PREFIX)
                .arg(saveMap.value("productId").toInt()));
    QSqlQuery query(_str, DB);
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::addProductDescription(int productId, QMap<QString, QVariant> saveMap)
{
    QString error;
    QSqlQuery query(QString("INSERT INTO `%0_product_description` (`product_id`, `language_id`, `name`, `description`) "
                              "VALUES (?, ?, ?, ?)").arg(PREFIX), DB);
    query.bindValue(0, productId);
    query.bindValue(1, saveMap.value("languageId").toInt());
    query.bindValue(2, saveMap.value("name").toString());
    query.bindValue(3, saveMap.value("description").toString());
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::updateProductDescription(QMap<QString, QVariant> saveMap)
{
    QString error;
    QString _str = QString("UPDATE `%0_product_description` SET ").arg(PREFIX);
    if (!saveMap.value("description").toString().isEmpty()){
        _str.append(QString("`description` = \'%0\', ").arg(saveMap.value("description").toString()));
    }
    _str.append(QString("`name` = \'%0\' "
                        "WHERE (`%1_product_description`.`product_id` = \'%2\') "
                        "AND  (`%1_product_description`.`language_id` = \'%3\') ")
                .arg(saveMap.value("name").toString())
                .arg(PREFIX)
                .arg(saveMap.value("productId").toInt())
                .arg(saveMap.value("languageId").toInt()));
    QSqlQuery query(_str, DB);
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::addProductToCategory(int productId, QMap<QString, QVariant> saveMap)
{
    QString error;
    QSqlQuery query(QString("INSERT INTO `%0_product_to_category` (`product_id`, `category_id`, `main_category`) "
                               "VALUES (?, ?, ?)").arg(PREFIX), DB);
    query.bindValue(0, productId);
    query.bindValue(1, saveMap.value("groupId"));
    query.bindValue(2, 1);
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::updateProductToCategory(QMap<QString, QVariant> saveMap)
{
    QString error;
    QSqlQuery query(QString("UPDATE `%0_product_to_category` SET `category_id` = \'%1\' "
                            "WHERE `%0_product_to_category`.`product_id` = \'%2\' ")
                    .arg(PREFIX)
                    .arg(saveMap.value("groupId").toInt())
                    .arg(saveMap.value("productId").toInt()), DB);
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::addProductToStore(int productId)
{
    QString error;
    QSqlQuery query(QString("INSERT INTO `%0_product_to_store` (`product_id`, `store_id`) "
                               "VALUES (?, ?)").arg(PREFIX), DB);
    query.bindValue(0, productId);
    query.bindValue(1, STORE);
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::addProductSpecial(int productId, QMap<QString, QVariant> saveMap)
{
    QString error;

    QDate sDate = QDate::currentDate();
    QDate eDate = QDate::fromJulianDay(sDate.toJulianDay() + 10);

    QSqlQuery query(QString("INSERT INTO `%0_product_special` (`product_id`, `customer_group_id`, `priority`, `price`, `date_start`, `date_end`) "
                               "VALUES (?, ?, ?, ?, ?, ?)").arg(PREFIX), DB);
    query.bindValue(0, productId);
    query.bindValue(1, 1);
    query.bindValue(2, 1);
    query.bindValue(3, saveMap.value("specPrice").toDouble());
    query.bindValue(4, sDate.toString("yyyy-MM-dd"));
    query.bindValue(5, eDate.toString("yyyy-MM-dd"));
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}

QString ctrlSProduct::updateProductSpecial(QMap<QString, QVariant> saveMap)
{
    QString error;

    QDate sDate = QDate::currentDate();
    QDate eDate = QDate::fromJulianDay(sDate.toJulianDay() + 10);

    QSqlQuery query(QString("UPDATE `%0_product_special` SET `price` = \'%1\', `date_start` = \'%2\', `date_end` = \'%3\' "
                            "WHERE `%0_product_special`.`product_id` = \'%4\' ")
                    .arg(PREFIX)
                    .arg(saveMap.value("specPrice").toDouble())
                    .arg(sDate.toString("yyyy-MM-dd"))
                    .arg(eDate.toString("yyyy-MM-dd"))
                    .arg(saveMap.value("productId").toInt()), DB);
    query.exec();
    if (query.lastError().isValid()){
        error = query.lastError().text();
    }
    return error;
}
