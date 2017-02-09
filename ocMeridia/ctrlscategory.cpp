#include "ctrlscategory.h"

ctrlSCategory::ctrlSCategory(QSqlDatabase _db, QString _pre)
{
    DB = _db;
    PREFIX = _pre;
}

void ctrlSCategory::set_current(int ID)
{
    CAT.clear();
    QSqlQuery query(QString("SELECT `%0category`.`parent_id`, `%0category`.`top`, `%0category`.`column`, "
                            "`%0category`.`sort_order`, `%0category`.`status`, `%0category`.`date_moidified`, "
                            "`%0category`.`code`, `%0category`.`image` "
                            "FROM `%0category` "
                            "WHERE `%0category`.`category_id` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(ID), DB);
    query.next();
    CAT.insert("parentId", query.value(0).toInt());
    CAT.insert("top", query.value(1).toInt());
    CAT.insert("column", query.value(2).toInt());
    CAT.insert("sortOrder", query.value(3).toInt());
    CAT.insert("status", query.value(4).toInt());
    CAT.insert("dateModified", query.value(5).toDateTime());
    CAT.insert("code", query.value(6).toString());
    CAT.insert("image", query.value(7).toString());
}

int ctrlSCategory::get_parentId()
{

}

int ctrlSCategory::get_status()
{

}

QDateTime ctrlSCategory::get_dateModified()
{

}

QString ctrlSCategory::get_code()
{

}

QMap<int, QMap<QString, QVariant> > ctrlSCategory::get_descriptions(int prodID)
{

}

QMap<QString, QVariant> ctrlSCategory::save(QMap<QString, QVariant> saveMap)
{

}
