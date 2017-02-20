#include "ctrlattribute.h"

ctrlAttribute::ctrlAttribute(QSqlDatabase _db, QString _pre, int _st)
{
    DB = _db;
    PREFIX = _pre;
    STORE = _st;
}

int ctrlAttribute::get_attributeID(QString _name)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_attribute_description`.`attribute_id` "
                            "FROM `%0_attribute_description` "
                            "WHERE `%0_attribute_description`.`name` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(_name), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}
