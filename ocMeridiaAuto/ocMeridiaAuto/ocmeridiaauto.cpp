#include "ocmeridiaauto.h"
#include "ui_ocmeridiaauto.h"
#include <QtSql/QtSql>

ocMeridiaAuto::ocMeridiaAuto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ocMeridiaAuto)
{
    ui->setupUi(this);
    QSettings sett("setting.ini", QSettings::IniFormat);
    _LANG = sett.value("oc/lang").toInt();
    _STATUS = sett.value("oc/status").toInt();
    _STORE = 0;
    _DIR = "catalog/";
    _DIR.append(sett.value("oc/dir").toString());


    log.append(QString("Start at: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));

    connect_db_local();
    connect_db_server();
    getMainFile();
    readMainFile();

    category_test();
    product_load();


    qDebug() << log;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(close()));
    timer->start(3000);
}

ocMeridiaAuto::~ocMeridiaAuto()
{
    delete ui;
}

void ocMeridiaAuto::read_setting()
{

}

void ocMeridiaAuto::connect_db_local()
{
    db_local = QSqlDatabase::addDatabase("QSQLITE", "localDB");
    db_local.setDatabaseName("meridia.db3");
    db_local.open();
    if (db_local.isOpen()){
        log.append(QString("%1 - успешное соединение с локальной БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    } else {
        log.append(QString("%1 - неуспешное соединение с локальной БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    }
}

void ocMeridiaAuto::connect_db_server()
{
    QSettings sett("setting.ini", QSettings::IniFormat);
    db_server = QSqlDatabase::addDatabase("QMYSQL", "serverDB");
    db_server.setDatabaseName(sett.value("db/name").toString());
    db_server.setHostName(sett.value("db/host").toString());
    db_server.setPort(sett.value("db/port").toInt());
    db_server.setUserName(sett.value("db/user").toString());
    db_server.setPassword(sett.value("db/pass").toString());
    db_server.open();
    if (db_server.isOpen()){
        log.append(QString("%1 - успешное соединение с удаленным сервером БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    } else {
        log.append(QString("%1 - неуспешное соединение с удаленным сервером БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    }

}

void ocMeridiaAuto::getMainFile()
{
    QSqlQuery queryMainFile("SELECT files.fname FROM files WHERE files.vid = \'2\' ", db_local);
    queryMainFile.next();
    if (queryMainFile.isValid()){
        FILE_1C = queryMainFile.value(0).toString();
        log.append(QString("%1 - Файл выгрузки 1С определен *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    } else {
        log.append(QString("%1 - Отсутствует информация о файле выгрузки 1С *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    }
}

void ocMeridiaAuto::readMainFile()
{
    file_category_list.clear();

    QFile file(FILE_1C);
    if (file.open(QIODevice::ReadOnly)){
        doc = new QDomDocument();
        doc->setContent(&file);

        QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
        QDomNode nodeCl = nodeCom.childNodes().at(0); //классификатор

        //reading groups

        QDomNode nodeGroups = nodeCl.childNodes().at(2);

        for (int x = 0; x < nodeGroups.childNodes().count(); x++){
            QDomNode nodeGroup = nodeGroups.childNodes().at(x);
            file_category_list.append(QString("%1||%2||%3")
                                      .arg(nodeGroup.firstChildElement("Ид").text())
                                      .arg(nodeGroup.firstChildElement("Наименование").text())
                                      .arg(nodeGroup.firstChildElement("Родитель").text()));
        }

        //reading products
        QDomNode nodeAll = nodeCom.childNodes().at(1); //комерч предложения
        int idxPred = 0;
        int idxProd = 0;
        for (int x = 0; x < nodeAll.childNodes().count(); x++){
            if (nodeAll.childNodes().at(x).nodeName() == "Предложения"){
                idxPred = x;
            }
            if (nodeAll.childNodes().at(x).nodeName() == "Товары"){
                idxProd = x;
            }
        }

        QDomNode nodePredAll = nodeAll.childNodes().at(idxPred);

        QString line;
        QMap<QString, double> mapPrice; //ИД товаров + Цена

        // собираем ID товаров и цены из предложений
        for (int x = 0; x < nodePredAll.childNodes().count(); x++){
            QDomNode nodePred = nodePredAll.childNodes().at(x);
            QString _ID = nodePred.firstChildElement("Ид").text();
            QString _Price = nodePred.firstChildElement("Цены").firstChildElement("Цена")
                    .firstChildElement("ЦенаЗаЕдиницу").text();
            if (_Price.isEmpty()){
                mapPrice.insert(_ID, 0.00);
            } else {
                mapPrice.insert(_ID, _Price.toDouble());
            }
        }

        //формируем карту товаров
        mapProduct.clear();
        QDomNode nodeProducts = nodeAll.childNodes().at(idxProd);
        QString _line;
        for (int y = 0; y < 500; y++){; //*//*nodeProducts.childNodes().count(); y++){ //****
            QDomNode nodeProduct = nodeProducts.childNodes().at(y);
            QString _ID = nodeProduct.firstChildElement("Ид").text();
            if (_ID.size() == 36){
                line.clear();
                line.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
                line.append(QString("||%0").arg(QString::number(mapPrice.value(_ID), 'f', 2)));//Цена
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                QString line_att = "";
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                        line_att.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                    }
                }
                line.append(QString("||%0").arg(line_att));//Описание
                //qDebug() << line;
                mapProduct.insert(_ID, line);
            } else if (_ID.size() > 36){
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ИДРодителя"){
                        _ID =  nAtt.firstChildElement("Значение").text();
                    }
                }
                if (_ID.size() == 36){
                    QString line_post = mapProduct.value(_ID);
                    if (line_post.isEmpty()){
                        line_post.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
                        line_post.append(QString("||%0").arg(QString::number(mapPrice.value(_ID), 'f', 2)));//Цена
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        QString line_att = "";
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                line_att.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                            }
                        }
                        line_post.append(QString("||%0").arg(line_att));//Описание
                        mapProduct.insert(_ID, line_post);
                    } else {
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                line_post.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                            }
                        }
                        mapProduct.remove(_ID);
                        mapProduct.insert(_ID, line_post);
                    }
                } else {
                    qDebug() << _ID << "Нет родителя ";
                }
            }
        }
    }
    file.close();

    log.append(QString("%1 - получено категорий: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(file_category_list.size()));
    log.append(QString("%1 - получено товаров: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProduct.size()));

}

void ocMeridiaAuto::category_test()
{
    QList<int> del_list;
    for (int x = 0; x < file_category_list.size(); x++){
        QSqlQuery queryS(QString("SELECT rmrt_category.category_id "
                                "FROM rmrt_category "
                                "WHERE rmrt_category.code = \'%0\' ")
                        .arg(file_category_list.at(x).split("||").at(0)), db_server);
        queryS.next();
        if (queryS.isValid()){ // есть -> подставляем ID в табл. товары, удаляем строку
            category_productChange(file_category_list.at(x).split("||").at(0), queryS.value(0).toString());
            // cтроки для удаления
            del_list.append(x);
        } else {
            //test2: проверяем наличие записи по имени
            QSqlQuery queryS2(QString("SELECT rmrt_category_description.category_id "
                                      "FROM rmrt_category_description "
                                      "WHERE rmrt_category_description.name = \"%1\" ")
                              .arg(file_category_list.at(x).split("||").at(1)), db_server);
            queryS2.next();
            if (queryS2.isValid()){
                //если такая группа есть: добавляем код1С, подставляем ID в таблицу товары, удаляем строку
                QSqlQuery queryU(QString("UPDATE rmrt_category SET code = \'%0\' "
                                         "WHERE rmrt_category.category_id = \'%1\' ")
                                 .arg(file_category_list.at(x).split("||").at(0))
                                 .arg(queryS2.value(0).toString()), db_server);
                queryU.exec();
                // меняем в таблице товаров код1С на ID групп магазина
                category_productChange(file_category_list.at(x).split("||").at(0), queryS2.value(0).toString());
                // cтроки для удаления
                del_list.append(x);
            } else {
                // проверяем наличие категории в синонимах
                QSqlQuery queryL(QString("SELECT cat.id_db FROM cat WHERE cat.cod = \'%1\'")
                                 .arg(file_category_list.at(x).split("||").at(0))
                                 , db_local);
                queryL.next();
                if (queryL.isValid()){
                    // меняем в таблице товаров код1С на ID группы из справочника синонимов
                    category_productChange(file_category_list.at(x).split("||").at(0), queryL.value(0).toString());
                    // cтроки для удаления
                    del_list.append(x);
                } else {
                    QSettings sett("setting.ini", QSettings::IniFormat);
                    if (sett.value("load1c/cat_in").toBool()){ // если по настройкам новую категорию вносим
                        int newCatID = category_add(file_category_list.at(x).split("||").at(0),
                                                    file_category_list.at(x).split("||").at(1),
                                                    sett.value("load1c/cat_target").toString());
                        category_productChange(file_category_list.at(x).split("||").at(0), QString::number(newCatID));
                        del_list.append(x);
                    } else if (sett.value("load1c/cat_out").toBool()) { //если по настройками новую категорию не вносим
                        category_productDelete(file_category_list.at(x).split("||").at(0));
                        del_list.append(x);
                    }
                }
            }
        }
    }
    for (int x = del_list.size() - 1; x >= 0; x--){
        file_category_list.removeAt(del_list.at(x));
    }
    log.append(QString("%1 - осталось необработанных категорий: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(file_category_list.size()));
}

void ocMeridiaAuto::category_productChange(QString prevID, QString realID)
{
    for (int x = 0; x < mapProduct.keys().size(); x++){
        QString id_product = mapProduct.keys().at(x);
        QString product_line = mapProduct.value(id_product);
        QString product_group = product_line.split("||").at(1);

        if (prevID == product_group){
            QString new_product_line(QString("%1||%2||%3")
                                     .arg(product_line.section("||", 0, 0))
                                     .arg(realID)
                                     .arg(product_line.section("||", 2, product_line.split("||").size() - 1)));
            mapProduct.remove(id_product);
            mapProduct.insert(id_product, new_product_line);
        }
    }
}

void ocMeridiaAuto::category_productDelete(QString prevID)
{
    QStringList del_list;
    for (int x = 0; x < mapProduct.keys().size(); x++){
        QString id_product = mapProduct.keys().at(x);
        QString product_line = mapProduct.value(id_product);
        if (prevID == product_line.split("||").at(1)){
            del_list.append(id_product);
        }
    }
    if (del_list.size() > 0){
        for (int x = 0; x < del_list.size(); x++){
            mapProduct.remove(del_list.at(x));
        }
    }
}

int ocMeridiaAuto::category_add(QString cat_code, QString cat_name, QString cat_parent)
{
    QString error;
    int cat_ID = 0;

    QSqlQuery query_C ("INSERT INTO `rmrt_category` (`parent_id`, `top`, `column`, `status`, "
                       "`date_added`, `date_modified`, `code`) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?) ", db_server);
    query_C.bindValue(0, cat_parent);
    query_C.bindValue(1, 0);
    query_C.bindValue(2, 0);
    query_C.bindValue(3, _STATUS);
    query_C.bindValue(4, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query_C.bindValue(5, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query_C.bindValue(6, cat_code);
    query_C.exec();
    if (query_C.lastError().text().size() > 3){
        error.append(query_C.lastError().text());
    } else {
        cat_ID = query_C.lastInsertId().toInt();
        //description
        QSqlQuery query_CD("INSERT INTO `rmrt_category_description` (`category_id`, `language_id`, `name`, `description`) "
                           "VALUES (?, ?, ?, ?)", db_server);
        query_CD.bindValue(0, cat_ID);
        query_CD.bindValue(1, _LANG);
        query_CD.bindValue(2, cat_name);
        query_CD.bindValue(3, cat_name);
        query_CD.exec();
        if (query_CD.lastError().text().size() > 3){
            error.append(query_CD.lastError().text());
        }
        //store
        QSqlQuery query_CS("INSERT INTO `rmrt_category_to_store` (`category_id`, `store_id`) VALUES (?, ?)", db_server);
        query_CS.bindValue(0, cat_ID);
        query_CS.bindValue(1, _STORE);
        query_CS.exec();
        if (query_CD.lastError().text().size() > 3){
            error.append(query_CD.lastError().text());
        }

        //path--------------------------------------
        //формируем список oc_category_path
        QStringList pathList;

        QSqlQuery queryP_Del("DELETE FROM `rmrt_category_path`", db_server);
        queryP_Del.exec();

        //выборка категорий
        QSqlQuery queryPS ("SELECT `rmrt_category`.`category_id`, `rmrt_category`.`parent_id` FROM `rmrt_category`", db_server);
        while (queryPS.next()){
            category_tempList.clear();
            int _id = queryPS.value(0).toInt();
            int _parent = queryPS.value(1).toInt();
            category_tempList.append(QString("%1|%1").arg(_id));
            if (_parent > 0){
                category_makePath(_id, _parent);
            }

            int level = category_tempList.size() - 1;
            for (int x = 0; x < category_tempList.size(); x++){
                pathList.append(QString("%1|%2").arg(category_tempList.at(x)).arg(level));
                level--;
            }
        }
        //путь категорий в таб oc_cayegory_padd
        if (pathList.size() > 0){
            for (int x = 0; x < pathList.size(); x++){
                   QSqlQuery queryS6_b("INSERT INTO `rmrt_category_path` (`category_id`, `path_id`, `level`) "
                                       "VALUES (?, ?, ?) ", db_server);
                   queryS6_b.bindValue(0, pathList.at(x).split("|").at(0));
                   queryS6_b.bindValue(1, pathList.at(x).split("|").at(1));
                   queryS6_b.bindValue(2, pathList.at(x).split("|").at(2));
                   queryS6_b.exec();
            }
        }
    }
    if (error.size() > 0){
        log.append(QString("%1 - Ошибка создания новой категории: %2 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(error));
    }
    return cat_ID;
}

void ocMeridiaAuto::category_makePath(int _ID, int _PAR)
{
    category_tempList.append(QString("%1|%2").arg(_ID).arg(_PAR));
    QSqlQuery query(QString("SELECT `rmrt_category`.`parent_id` FROM `rmrt_category` WHERE `rmrt_category`.`category_id` = \'%1\' ")
                    .arg(_PAR), db_server);
    query.next();
    int id_parent = query.value(0).toInt();
    if (id_parent > 0){
        category_makePath(_ID, id_parent);
    }
}

void ocMeridiaAuto::product_load()
{

    if (mapProduct.keys().size() > 0){
        QSettings sett("setting.ini", QSettings::IniFormat);
        for (int x = 0; x < mapProduct.keys().size(); x++){
            QString _code = mapProduct.keys().at(x);
            QString _name = mapProduct.value(_code).split("||").at(0);
            QString _group = mapProduct.value(_code).split("||").at(1);
            double _price = mapProduct.value(_code).split("||").at(2).toDouble();
            QString _image = mapProduct.value(_code).split("||").at(3);
            QString _descr = mapProduct.value(_code).split("||").at(4);
            QString _attr = mapProduct.value(_code).split("||").at(5);

            int _id = product_get_id(_code, _name);

            bool idxIN;
            bool idxUP;
            QString strUp;
            if (_id == 0){//новый товар
                idxIN = true;
                idxUP = false;
                if (sett.value("load1c/new_prod_in").toBool()){ // если разрешено вносить новые товары
                    idxIN = true;
                    if (sett.value("load1c/prod_out").toBool()){ // разрешено только с ценой > 0
                        if (_price > 0.00){
                            idxIN = true;
                        } else if (_price == 0.00){
                            idxIN = false;
                        }
                    } else if (sett.value("load1c/prod_in").toBool()){ //разрешено вносить все
                        idxIN = true;
                    }
                } else if (sett.value("load1c/new_prod_out").toBool()) {
                    idxIN = false;
                }
            } else if (_id > 0){// старый товар
                idxIN = false;
                idxUP = true;
            }
            if (idxIN){ //ввод новых товаров
                // ID аттрибута "ЦВЕТ"
                QSqlQuery query_GA(QString("SELECT rmrt_attribute_description.attribute_id "
                                           "FROM rmrt_attribute_description "
                                           "WHERE rmrt_attribute_description.name = \'Цвет\' "
                                           "AND rmrt_attribute_description.language_id = \'%1\' ").arg(_LANG), db_server);
                query_GA.next();
                IDcolor = query_GA.value(0).toInt();

                product_insert(_code, _name, _group, _image, _descr, _attr, _price);
            } else if (idxUP){ //обновление старых товаров
                QString mainStr;
                if (sett.value("load1c/up_price").toInt() == 2){
                    mainStr.append(QString("UPDATE rmrt_product SET price = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(_price)
                                   .arg(_id));
                }
                if (sett.value("load1c/up_image").toInt() == 2){
                    if (!_DIR.isEmpty()){
                        mainStr.append(QString("UPDATE rmrt_product SET image = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                       .arg(_DIR.append("/").append(_image))
                                       .arg(_id));
                    }
                }
                if (sett.value("load1c/up_name").toInt() == 2){
                    mainStr.append(QString("UPDATE rmrt_product_description SET `name` = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(_name)
                                   .arg(_id)
                                   .arg(_LANG));
                }
                if (sett.value("load1c/up_desc").toInt() == 2){
                    mainStr.append(QString("UPDATE rmrt_product_description SET description = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(_descr)
                                   .arg(_id)
                                   .arg(_LANG));
                }
                if (sett.value("load1c/up_color").toInt() == 2){
                    if (_attr.size() > 1){
                        //clear;
                        QSqlQuery queryClearProdAttribute(QString("DELETE FROM rmrt_product_attribute WHERE rmrt_product_attribute.product_id = \'%1\' ")
                                                          .arg(_id), db_server);
                        queryClearProdAttribute.exec();
                        for (int x = 0; x < _attr.split(",").size(); x++){
                            if (!_attr.split(",").at(x).isEmpty()){
                                QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                                "VALUES (?, ?, ?, ?)", db_server);
                                queryAddProdAttribute.bindValue(0, _id);
                                queryAddProdAttribute.bindValue(1, IDcolor);
                                queryAddProdAttribute.bindValue(2, _LANG);
                                queryAddProdAttribute.bindValue(3, _attr.split(",").at(x));
                                queryAddProdAttribute.exec();
//                                if (queryAddProdAttribute.lastError().text().size() > 3){
//                                  error.append(queryAddProdAttribute.lastError().text());
//                                }
                            }
                        }
                    }
                }
                if (mainStr.size() > 0){
                    QSqlQuery queryUP(mainStr, db_server);
                    queryUP.exec();
                }
            }
        }
    }
}

int ocMeridiaAuto::product_get_id(const QString prod_code, const QString prod_name)
{
    //проверяем по коду 1с
    int prod_id = 0;
    QSqlQuery queryS(QString("SELECT rmrt_product.product_id "
                             "FROM rmrt_product "
                             "WHERE rmrt_product.sku = \'%1\' ")
                     .arg(prod_code), db_server);
    queryS.next();
    if (queryS.isValid()){
        prod_id = queryS.value(0).toInt();
    } else {
        //если нет - проверяем по наименованию
        QSqlQuery queryS2(QString("SELECT rmrt_product_description.product_id "
                                  "FROM rmrt_product_description "
                                  "WHERE rmrt_product_description.name = \"%1\" "
                                  "AND rmrt_product_description.language_id = \'%2\'  ")
                          .arg(prod_name)
                          .arg(_LANG), db_server);
        queryS2.next();
        if (queryS2.isValid()){
            //если есть - ставим код 1С + ставим ИД
            prod_id = queryS2.value(0).toInt();
            QSqlQuery queryU(QString("UPDATE rmrt_product "
                                     "SET sku = \'%0\' "
                                     "WHERE rmrt_product.product_id = \'%1\' ")
                             .arg(prod_code)
                             .arg(prod_id), db_server);
            queryU.exec();
        }
    }
    return prod_id;
}

void ocMeridiaAuto::product_insert(const QString pcode, const QString pname, const QString pgroup,
                                   const QString pimage, const QString pdesc, const QString pattr, double pprice)
{
    QSettings sett("setting.ini", QSettings::IniFormat);
    QSqlQuery queryAddProduct("INSERT INTO rmrt_product (sku, quantity, manufacturer_id, stock_status_id, "
                              "tax_class_id, status, price, image, date_added, date_modified, model) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", db_server);
    queryAddProduct.bindValue(0, pcode);
    queryAddProduct.bindValue(1, sett.value("oc/quan").toDouble());
    queryAddProduct.bindValue(2, 0); // производителя нет
    queryAddProduct.bindValue(3, sett.value("oc/stock").toInt()); //stock status  = 7 (в наличие)
    queryAddProduct.bindValue(4, sett.value("oc/tax").toInt()); // tax_class = 9 (облагаемый налогом)
    queryAddProduct.bindValue(5, _STATUS); // status = 1 (включено)
    queryAddProduct.bindValue(6, pprice); // price
    queryAddProduct.bindValue(7, _DIR.append("/").append(pimage)); // image
    queryAddProduct.bindValue(8, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    queryAddProduct.bindValue(9, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    queryAddProduct.bindValue(10, "model");
    queryAddProduct.exec();
    if (queryAddProduct.lastError().text().size() > 3){

    } else {
        int _ID = queryAddProduct.lastInsertId().toInt();
        //добавляем description
        QSqlQuery queryAddProdDesc("INSERT INTO rmrt_product_description (product_id, language_id, name, description) "
                                   "VALUES (?, ?, ?, ?)", db_server);
        queryAddProdDesc.bindValue(0, _ID);
        queryAddProdDesc.bindValue(1, _LANG);
        queryAddProdDesc.bindValue(2, pname);
        queryAddProdDesc.bindValue(3, pdesc);
        queryAddProdDesc.exec();
//        if (queryAddProdDesc.lastError().text().size() > 3){
//          error.append(queryAddProdDesc.lastError().text());
//        }

        //добавляем store
        QSqlQuery queryAddProdStore("INSERT INTO rmrt_product_to_store (product_id, store_id) "
                                   "VALUES (?, ?)", db_server);
        queryAddProdStore.bindValue(0, _ID);
        queryAddProdStore.bindValue(1, _STORE);
        queryAddProdStore.exec();
//        if (queryAddProdStore.lastError().text().size() > 3){
//          error.append(queryAddProdStore.lastError().text());
//        }
        //добавляем category
        QSqlQuery queryAddProdCat("INSERT INTO rmrt_product_to_category (product_id, category_id, main_category) "
                                   "VALUES (?, ?, ?)", db_server);
        queryAddProdCat.bindValue(0, _ID);
        queryAddProdCat.bindValue(1, pgroup);
        queryAddProdCat.bindValue(2, 1);
        queryAddProdCat.exec();
//        if (queryAddProdCat.lastError().text().size() > 3){
//          error.append(queryAddProdCat.lastError().text());
//        }

        //добавляем аттрибуты
        if (pattr.size() > 1){
            for (int x = 0; x < pattr.split(",").size(); x++){
                if (!pattr.split(",").at(x).isEmpty()){
                    QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                    "VALUES (?, ?, ?, ?)", db_server);
                    queryAddProdAttribute.bindValue(0, _ID);
                    queryAddProdAttribute.bindValue(1, IDcolor);
                    queryAddProdAttribute.bindValue(2, _LANG);
                    queryAddProdAttribute.bindValue(3, pattr.split(",").at(x));
                    queryAddProdAttribute.exec();
                }
            }
        }
    }
}


