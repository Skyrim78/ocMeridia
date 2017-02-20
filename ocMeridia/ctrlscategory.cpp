#include "ctrlscategory.h"

ctrlSCategory::ctrlSCategory(QSqlDatabase _db, QString _pre, int _st)
{
    DB = _db;
    PREFIX = _pre;
    STORE = _st;
}

void ctrlSCategory::set_current(int ID, int LANG)
{
    CAT.clear();
    QSqlQuery query(QString("SELECT `%0_category`.`parent_id`, `%0_category`.`top`, `%0_category`.`column`, "
                            "`%0_category`.`sort_order`, `%0_category`.`status`, `%0_category`.`date_modified`, "
                            "`%0_category`.`code`, `%0_category`.`image` "
                            "FROM `%0_category` "
                            "WHERE `%0_category`.`category_id` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(ID), DB);
    query.next();
   // qDebug() << "cat" << query.lastError();
    CAT.insert("parentId", query.value(0).toInt());
    CAT.insert("top", query.value(1).toInt());
    CAT.insert("column", query.value(2).toInt());
    CAT.insert("sortOrder", query.value(3).toInt());
    CAT.insert("status", query.value(4).toInt());
    CAT.insert("dateModified", query.value(5).toDateTime());
    CAT.insert("code", query.value(6).toString());
    CAT.insert("image", query.value(7).toString());

    QSqlQuery queryB(QString("SELECT %0_category_description.name, "
                            "%0_category_description.description, %0_category_description.meta_title, "
                            "%0_category_description.meta_h1, %0_category_description.meta_description, "
                            "%0_category_description.meta_keyword "
                            "FROM %0_category_description "
                            "WHERE (%0_category_description.category_id = \'%1\') "
                            "AND (%0_category_description.language_id = \'%2\') ")
                    .arg(PREFIX)
                    .arg(ID)
                    .arg(LANG), DB);
    queryB.next();
    //qDebug() << "cat_desc" << queryB.lastError();
    CAT.insert("name", queryB.value(0).toString());
    CAT.insert("desc", queryB.value(1).toString());
}

int ctrlSCategory::get_parentId()
{
    return CAT.value("parentId").toInt();
}

int ctrlSCategory::get_top()
{
    return CAT.value("top").toInt();
}

int ctrlSCategory::get_column()
{
    return CAT.value("column").toInt();
}

int ctrlSCategory::get_sortOrder()
{
    return CAT.value("sortOrder").toInt();
}

int ctrlSCategory::get_status()
{
    return CAT.value("status").toInt();
}

QDateTime ctrlSCategory::get_dateModified()
{
    return CAT.value("dateModified").toDateTime();
}

QString ctrlSCategory::get_code()
{
    return CAT.value("code").toString();
}

QString ctrlSCategory::get_name()
{
    return CAT.value("name").toString();
}

QString ctrlSCategory::get_description()
{
    return CAT.value("desc").toString();
}

QString ctrlSCategory::get_parentName(int ID, int LANG)
{
    set_current(ID, LANG);
    return get_name();
}

QString ctrlSCategory::get_road(int ID, int LANG)
{

    QString ROAD;

    set_current(ID, LANG);
    ROAD.append(get_name());

    int parent = get_parentId();
    if (parent > 0){
        set_current(parent, LANG);
        ROAD.prepend(QString("%0 - ").arg(get_name()));
        parent = get_parentId();
        if (parent > 0){
            set_current(parent, LANG);
            ROAD.prepend(QString("%0 - ").arg(get_name()));
            parent = get_parentId();
            if (parent > 0){
                set_current(parent, LANG);
                ROAD.prepend(QString("%0 - ").arg(get_name()));
                parent = get_parentId();
                if (parent > 0){
                    set_current(parent, LANG);
                    ROAD.prepend(QString("%0 - ").arg(get_name()));
                    parent = get_parentId();
                    if (parent > 0){
                        set_current(parent, LANG);
                        ROAD.prepend(QString("%0 - ").arg(get_name()));
                        parent = get_parentId();
                        if (parent > 0){
                            set_current(parent, LANG);
                            ROAD.prepend(QString("%0 - ").arg(get_name()));
                            parent = get_parentId();

                        }
                    }
                }
            }
        }
    }
    return ROAD;
}

int ctrlSCategory::getIdByCode(QString _code)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT `%0_category`.`category_id` "
                            "FROM `%0_category` "
                            "WHERE `%0_category`.`code` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(_code), DB);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    }
    return _id;
}

QMap<int, QMap<QString, QVariant> > ctrlSCategory::get_categories()
{
    QMap<int, QMap<QString, QVariant> > MAP;
    int x = 0;
    QSqlQuery query(QString("SELECT `%0_category`.`category_id`, `%0_category`.`parent_id`, "
                            "`%0_category`.`top`, `%0_category`.`column`, "
                            "`%0_category`.`sort_order`, `%0_category`.`status`, `%0_category`.`date_modified`, "
                            "`%0_category`.`code`, `%0_category`.`image` "
                            "FROM `%0_category` ")
                    .arg(PREFIX), DB);
    while (query.next()){
        QMap<QString, QVariant> _map;
        _map.insert("categoryId", query.value(0).toInt());
        _map.insert("parentId", query.value(1).toInt());
        _map.insert("top", query.value(2).toInt());
        _map.insert("column", query.value(3).toInt());
        _map.insert("sortOrder", query.value(4).toInt());
        _map.insert("status", query.value(5).toInt());
        _map.insert("dateModified", query.value(6).toDateTime());
        _map.insert("code", query.value(7).toString());
        _map.insert("image", query.value(8).toString());

        MAP.insert(x, _map);
        x++;
    }
    return MAP;
}

QMap<int, QMap<QString, QVariant> > ctrlSCategory::get_mainCategories()
{
    QMap<int, QMap<QString, QVariant> > MAP;
    int x = 0;
    QSqlQuery query(QString("SELECT `%0_category`.`category_id`, `%0_category`.`parent_id`, "
                            "`%0_category`.`top`, `%0_category`.`column`, "
                            "`%0_category`.`sort_order`, `%0_category`.`status`, `%0_category`.`date_modified`, "
                            "`%0_category`.`code`, `%0_category`.`image` "
                            "FROM `%0_category` "
                            "WHERE `%0_category`.`parent_id` = \'0\' ")
                    .arg(PREFIX), DB);
    while (query.next()){
        QMap<QString, QVariant> _map;
        _map.insert("categoryId", query.value(0).toInt());
        _map.insert("parentId", query.value(1).toInt());
        _map.insert("top", query.value(2).toInt());
        _map.insert("column", query.value(3).toInt());
        _map.insert("sortOrder", query.value(4).toInt());
        _map.insert("status", query.value(5).toInt());
        _map.insert("dateModified", query.value(6).toDateTime());
        _map.insert("code", query.value(7).toString());
        _map.insert("image", query.value(8).toString());

        MAP.insert(x, _map);
        x++;
    }
    return MAP;
}

QMap<int, QMap<QString, QVariant> > ctrlSCategory::get_descriptions(int ID)
{
    QMap<int, QMap<QString, QVariant> > MAP;
    QSqlQuery query(QString("SELECT %0_category_description.language_id, %0_category_description.name, "
                            "%0_category_description.description, %0_category_description.meta_title, "
                            "%0_category_description.meta_h1, %0_category_description.meta_description, "
                            "%0_category_description.meta_keyword "
                            "FROM %0_category_description "
                            "WHERE %0_category_description.category_id = \'%1\' ")
                    .arg(PREFIX)
                    .arg(ID), DB);
    int _x = 0;
    while (query.next()){
        QMap<QString, QVariant> DESC;
        DESC.insert("lang_id", query.value(0).toInt());
        DESC.insert("name", query.value(1).toString());
        DESC.insert("desc", query.value(1).toString());
        DESC.insert("meta_title", query.value(2).toString());
        DESC.insert("meta_h1", query.value(3).toString());
        DESC.insert("meta_desc", query.value(4).toString());
        DESC.insert("meta_key", query.value(5).toString());
        MAP.insert(_x, DESC);
        _x++;
    }
    return MAP;
}

QMap<int, QMap<QString, QVariant> > ctrlSCategory::get_descriptionsLang(int ID, int LANG)
{
    QMap<int, QMap<QString, QVariant> > MAP;
    QSqlQuery query(QString("SELECT %0_category_description.language_id, %0_category_description.name, "
                            "%0_category_description.description, %0_category_description.meta_title, "
                            "%0_category_description.meta_h1, %0_category_description.meta_description, "
                            "%0_category_description.meta_keyword "
                            "FROM %0_category_description "
                            "WHERE (%0_category_description.category_id = \'%1\') "
                            "AND (%0_category_description.language_id = \'%2\') ")
                    .arg(PREFIX)
                    .arg(ID)
                    .arg(LANG), DB);
    int _x = 0;
    while (query.next()){
        QMap<QString, QVariant> DESC;
        DESC.insert("lang_id", query.value(0).toInt());
        DESC.insert("name", query.value(1).toString());
        DESC.insert("desc", query.value(1).toString());
        DESC.insert("meta_title", query.value(2).toString());
        DESC.insert("meta_h1", query.value(3).toString());
        DESC.insert("meta_desc", query.value(4).toString());
        DESC.insert("meta_key", query.value(5).toString());
        MAP.insert(_x, DESC);
        _x++;
    }
    return MAP;
}

QMap<QString, QVariant> ctrlSCategory::saveMain(QMap<QString, QVariant> saveMap, int LANG)
{
    QString _error;
    int _lastId = 0;
    if (saveMap.value("id").toInt() == 0){
        QSqlQuery query(QString("INSERT INTO `%0_category` (`parent_id`, `top`, `column`, `sort_order`, "
                                "`status`, `code`, `date_added`, `date_modified`) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?, ?)").arg(PREFIX), DB);
        query.bindValue(0, saveMap.value("parentId").toInt());
        query.bindValue(1, saveMap.value("top").toInt());
        query.bindValue(2, saveMap.value("column").toInt());
        query.bindValue(3, saveMap.value("sortOrder").toInt());
        query.bindValue(4, saveMap.value("status").toInt());
        query.bindValue(5, saveMap.value("code").toString());
        query.bindValue(6, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.bindValue(7, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
        query.exec();
        if (query.lastError().isValid()){
            _error = query.lastError().text();
        } else {
            _lastId = query.lastInsertId().toInt();

            QSqlQuery queryD(QString("INSERT INTO `%0_category_description` (`category_id`, `language_id`, `name`, `description`) "
                                     "VALUES (?, ?, ?, ?)").arg(PREFIX), DB);
            queryD.bindValue(0, _lastId);
            queryD.bindValue(1, LANG);
            queryD.bindValue(2, saveMap.value("name").toString());
            queryD.bindValue(3, saveMap.value("desc").toString());
            queryD.exec();

            saveCategoryToStore(_lastId);
        }
    } else {
        QString _str = QString("UPDATE `%0_category` SET ").arg(PREFIX);
        if (!saveMap.value("top").toString().isEmpty()){
            _str.append(QString("`top` = \'%0\', ").arg(saveMap.value("top").toInt()));
        }
        if (!saveMap.value("column").toString().isEmpty()){
            _str.append(QString("`column` = \'%0\', ").arg(saveMap.value("column").toInt()));
        }
        if (!saveMap.value("sortOrder").toString().isEmpty()){
            _str.append(QString("`sort_order` = \'%0\', ").arg(saveMap.value("sortOrder").toInt()));
        }
        if (!saveMap.value("status").toString().isEmpty()){
            _str.append(QString("`status` = \'%0\', ").arg(saveMap.value("status").toInt()));
        }
        if (!saveMap.value("code").toString().isEmpty()){
            _str.append(QString("`code` = \'%0\', ").arg(saveMap.value("code").toString()));
        }
        _str.append(QString("`parent_id` = \'%1\' WHERE `%0_category`.`category_id` = \'%2\'")
                    .arg(PREFIX)
                    .arg(saveMap.value("parentId").toInt())
                    .arg(saveMap.value("id").toInt()));
        QSqlQuery query(_str, DB);
        query.exec();

        QString _strD = QString("UPDATE `%0_category_description` SET ").arg(PREFIX);
        if (!saveMap.value("desc").toString().isEmpty()){
            _strD.append(QString("`description` = \'%0\', ").arg(saveMap.value("desc").toString()));
        }
        _strD.append(QString("`name` = \'%1\' "
                            "WHERE (`%0_category_description`.`category_id` = \'%2\') "
                            "AND (`%0_category_description`.`language_id` = \'%3\') ")
                    .arg(PREFIX)
                    .arg(saveMap.value("name").toInt())
                    .arg(saveMap.value("id").toInt())
                    .arg(LANG));
        QSqlQuery queryD(_strD, DB);
        queryD.exec();


        if (query.lastError().isValid()){
            _error = query.lastError().text();
        } else {
            _lastId = saveMap.value("id").toInt();
        }
    }
    QMap<QString,QVariant> FEED;
    FEED.insert("error", _error);
    FEED.insert("lastId", _lastId);
    
    return FEED;
}


void ctrlSCategory::makeCategoryPath()
{
    QStringList pathList;

    QSqlQuery queryP_Del(QString("DELETE FROM `%0_category_path`").arg(PREFIX), DB);
    queryP_Del.exec();

    //выборка категорий
    QSqlQuery queryPS (QString("SELECT `%0_category`.`category_id`, `%0_category`.`parent_id` FROM `%0_category`")
                       .arg(PREFIX), DB);
    while (queryPS.next()){
        tempList.clear();
        int _id = queryPS.value(0).toInt();
        int _parent = queryPS.value(1).toInt();
        tempList.append(QString("%1|%1").arg(_id));
        if (_parent > 0){
            makeCategoryPathLevel(_id, _parent);
        }
        int level = tempList.size() - 1;
        for (int x = 0; x < tempList.size(); x++){
            pathList.append(QString("%1|%2").arg(tempList.at(x)).arg(level));
            level--;
        }
    }
    //путь категорий в таб oc_cayegory_padd
    if (pathList.size() > 0){
        for (int x = 0; x < pathList.size(); x++){
               QSqlQuery queryS6_b(QString("INSERT INTO `%0_category_path` (`category_id`, `path_id`, `level`) "
                                   "VALUES (?, ?, ?) ").arg(PREFIX), DB);
               queryS6_b.bindValue(0, pathList.at(x).split("|").at(0));
               queryS6_b.bindValue(1, pathList.at(x).split("|").at(1));
               queryS6_b.bindValue(2, pathList.at(x).split("|").at(2));
               queryS6_b.exec();
        }
    }
}

void ctrlSCategory::makeCategoryPathLevel(int _ID, int _PAR)
{
    tempList.append(QString("%1|%2").arg(_ID).arg(_PAR));
    QSqlQuery query(QString("SELECT `%0_category`.`parent_id` "
                            "FROM `%0_category` "
                            "WHERE `%0_category`.`category_id` = \'%1\' ")
                    .arg(PREFIX)
                    .arg(_PAR), DB);
    query.next();
    int id_parent = query.value(0).toInt();
    if (id_parent > 0){
        makeCategoryPathLevel(_ID, id_parent);
    }
}

void ctrlSCategory::saveCategoryToStore(int ID)
{
    QSqlQuery query(QString("INSERT INTO `%0_category_to_store` (`category_id`, `store_id`) "
                            "VALUES (?, ?)")
                    .arg(PREFIX), DB);
    query.bindValue(0, ID);
    query.bindValue(1, STORE);
    query.exec();
}

