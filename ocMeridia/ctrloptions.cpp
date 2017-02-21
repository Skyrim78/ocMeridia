#include "ctrloptions.h"

ctrlOptions::ctrlOptions(QSqlDatabase _db, QString _pre)
{
    DB = _db;
    PREFIX = _pre;
}

void ctrlOptions::remakeOptions(QStringList listOptions, int LANG)
{
    if (listOptions.size() > 0){
        for (int x = 0; x < listOptions.size(); x++){
            int ID = get_optionID(listOptions.at(x));
            if (ID == 0){
               add_option(listOptions.at(x), LANG);
            }
        }
    }
}

int ctrlOptions::get_optionID(QString _name)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_option_description`.`option_id` "
                            "FROM `%0_option_description` "
                            "WHERE (`%0_option_description`.`name` = \'%1\') ")
                    .arg(PREFIX)
                    .arg(_name), DB);
    //qDebug() << "get_optionID -"<<query.lastError();
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

void ctrlOptions::add_option(QString _name, int LANG)
{
    QSqlQuery queryA(QString("INSERT INTO `%0_option` (type, sort_order) "
                            "VALUES (?, ?) ")
                    .arg(PREFIX), DB);
    queryA.bindValue(0, "radio"); //добавляем только радио опции
    queryA.bindValue(1, 0);
    queryA.exec();
    if (queryA.lastError().isValid()){
        qDebug() << queryA.lastError().text();
    } else {
        int _id = queryA.lastInsertId().toInt();

        QSqlQuery queryB(QString("INSERT INTO `%0_option_description` (`option_id`, `language_id`, `name`) "
                                 "VALUES (?, ?, ?) ")
                         .arg(PREFIX), DB);
        queryB.bindValue(0, _id);
        queryB.bindValue(1, LANG);
        queryB.bindValue(2, _name);
        queryB.exec();
        if (queryB.lastError().isValid()){
            qDebug() << queryB.lastError().text();
        }
    }
}

void ctrlOptions::remakeOptionValue(QMap<int, QMap<QString, QVariant> > mapOptionValue, int LANG)
{
    if (mapOptionValue.count() > 0){
        for (int x = 0; x < mapOptionValue.count(); x++){
            QMap<QString,QVariant> map = mapOptionValue.value(x);
            int _optionId = get_optionID(map.value("name").toString());
            QString _name = map.value("value").toString();
            int ID = get_optionValueID(_optionId, _name);
            if (ID == 0){
                add_optionValue(_optionId, _name, LANG);
            }
        }
    }
}

int ctrlOptions::get_optionValueID(int _optionId, QString _name)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_option_value_description`.`option_value_id` "
                            "FROM `%0_option_value_description` "
                            "WHERE (`%0_option_value_description`.`name` = \'%1\') "
                            "AND (`%0_option_value_description`.`option_id` = \'%2\') ")
                    .arg(PREFIX)
                    .arg(_name)
                    .arg(_optionId), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

void ctrlOptions::add_optionValue(int _optionId, QString _name, int LANG)
{
    QSqlQuery queryA(QString("INSERT INTO `%0_option_value` (option_id, image, sort_order) "
                            "VALUES (?, ?, ?) ")
                    .arg(PREFIX), DB);
    queryA.bindValue(0, _optionId);
    queryA.bindValue(1, "");
    queryA.bindValue(2, 0);
    queryA.exec();
    if (queryA.lastError().isValid()){
        qDebug() << queryA.lastError().text();
    } else {
        int _id = queryA.lastInsertId().toInt();

        QSqlQuery queryB(QString("INSERT INTO `%0_option_value_description` (`option_value_id`, `language_id`, `option_id`, `name`) "
                                 "VALUES (?, ?, ?, ?) ")
                         .arg(PREFIX), DB);
        queryB.bindValue(0, _id);
        queryB.bindValue(1, LANG);
        queryB.bindValue(2, _optionId);
        queryB.bindValue(3, _name);
        queryB.exec();
        if (queryB.lastError().isValid()){
            qDebug() << queryB.lastError().text();
        }
    }
}
