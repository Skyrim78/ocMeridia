#include "ctrlmanufacturer.h"

ctrlManufacturer::ctrlManufacturer(QSqlDatabase _db, QString _pre, int _st)
{
    DB = _db;
    PREFIX = _pre;
    STORE = _st;
}

int ctrlManufacturer::getID_byName(QString _name)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_manufacturer_description`.`manufacturer_id` "
                            "FROM `%0_manufacturer_description` "
                            "WHERE `%0_manufacturer_description`.`name` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(_name), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

QMap<QString, QVariant> ctrlManufacturer::save(QMap<QString, QVariant> saveMap, int LANG)
{
    QString _error;
    int _lastId = 0;
    if (saveMap.value("manufacturerId").toInt() == 0){
        //основная таблица
        QSqlQuery queryA(QString("INSERT INTO `%0_manufacturer` (`name`, `sort_order`) "
                                 "VALUES (?, ?)").arg(PREFIX), DB);
        queryA.bindValue(0, saveMap.value("name").toString());
        queryA.bindValue(1, 0);
        queryA.exec();
        if (queryA.lastError().isValid()){
            _error = queryA.lastError().text();
        } else {
            _lastId = queryA.lastInsertId().toInt();

            //description
            QSqlQuery queryB(QString("INSERT INTO `%0_manufacturer_description` "
                                     "(`manufacturer_id`, `language_id`, `name`, `description`, `sort_order`) "
                                     "VALUES (?, ?, ?, ?, ?)").arg(PREFIX), DB);
            queryB.bindValue(0, _lastId);
            queryB.bindValue(1, LANG);
            queryB.bindValue(2, saveMap.value("name").toString());
            queryB.bindValue(3, saveMap.value("name").toString());
            queryB.bindValue(4, 0);
            queryB.exec();
            if (queryB.lastError().isValid()){
                _error.append(queryB.lastError().text());
            }

            //to store
            QSqlQuery queryC(QString("INSERT INTO `%0_manufacturer_to_store` "
                                     "(`manufacturer_id`, `store_id`) "
                                     "VALUES (?, ?)").arg(PREFIX), DB);
            queryC.bindValue(0, _lastId);
            queryC.bindValue(1, STORE);
            queryC.exec();
            if (queryC.lastError().isValid()){
                _error.append(queryC.lastError().text());
            }
        }
    }
    QMap<QString, QVariant> feed;
    feed.insert("error", _error);
    feed.insert("lastId", _lastId);

    return feed;
}


