#include "ocmeridiaauto.h"
#include "ui_ocmeridiaauto.h"
#include <QtSql/QtSql>

ocMeridiaAuto::ocMeridiaAuto(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ocMeridiaAuto)
{
    ui->setupUi(this);
    read_setting();
    if (_AUTO){
        log.append(QString("Start at: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));

        connect_db_local();
        connect_db_server();

        scenario_A();

        scenario_B();

        log.append(QString("Finish at: %1").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
        writeLog();
    }

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(close()));
    timer->start(10);
}

ocMeridiaAuto::~ocMeridiaAuto()
{
    delete ui;
}

void ocMeridiaAuto::read_setting()
{
    QSettings sett("setting.ini", QSettings::IniFormat);
    _AUTO = sett.value("auto/active").toBool();
    _LANG = sett.value("oc/lang").toInt();
    _STOCK = sett.value("oc/stock").toInt();
    _STATUS = sett.value("oc/status").toInt();
    _TAX = sett.value("oc/tax").toInt();
    _STORE = 0;
    _DIR = "catalog/";
    _DIR.append(sett.value("oc/dir").toString());

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
    db_server.setConnectOptions("MYSQL_OPT_RECONNECT = TRUE;");
    db_server.open();
    if (db_server.isOpen()){
        log.append(QString("%1 - успешное соединение с удаленным сервером БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    } else {
        log.append(QString("%1 - неуспешное соединение с удаленным сервером БД *").arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy")));
    }

}

void ocMeridiaAuto::main_file_get()
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

void ocMeridiaAuto::main_file_read()
{
    mapGroup.clear();
    mapAttributes.clear();
    mapProduct.clear();
    mapProductAttribute.clear();
    mapProductImage.clear();


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
            QString _code = nodeGroup.firstChildElement("Ид").text();
            QString _name = nodeGroup.firstChildElement("Наименование").text();
            mapGroup.insert(_code, group_get_id(_code, _name, 1));
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

        //QString line;
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

        QDomNode nodeProducts = nodeAll.childNodes().at(idxProd);
        //---
        int _id;
        QString _art;
        QString _model;
        QString _name;
        int _id_group;
        int _manufacturer;
        double _price;
        double _quan;
        QString _desc;
        QString _image;
        //*********

        for (int y = 0; y < 4000; y++){; //*/nodeProducts.childNodes().count(); y++){ //****
            QDomNode nodeProduct = nodeProducts.childNodes().at(y);
            _art = nodeProduct.firstChildElement("Ид").text();

            if (_art.size() > 36){
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ИДРодителя"){
                        _art =  nAtt.firstChildElement("Значение").text();
                    }
                }
            }

            _model = "model";
            _name = nodeProduct.firstChildElement("ПолноеНаименование").text();
            _id_group = mapGroup.value(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text());
            _manufacturer = manufacturer_get_id("no");
            _price = mapPrice.value(_art);
            _quan = sett.value("oc/quan").toDouble();
            _desc = nodeProduct.firstChildElement("Описание").text();
            _image = "";
            if (!nodeProduct.firstChildElement("Картинка").text().isEmpty()){
                _image = QString("%1/%2").arg(_DIR).arg(nodeProduct.firstChildElement("Картинка").text());
            }
            _id = product_get_id(_art, _name);

            mapProduct.insert(_art, QString("%1||%2||%3||%4||%5||%6||%7||%8||%9||%10")
                              .arg(_id)
                              .arg(_art)
                              .arg(_model)
                              .arg(_name)
                              .arg(_id_group)
                              .arg(_manufacturer)
                              .arg(_price)
                              .arg(_quan)
                              .arg(_desc)
                              .arg(_image));

            QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
            for (int x = 0; x < elAttributes.childNodes().count(); x++){
                QDomNode nAtt = elAttributes.childNodes().at(x);
                if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                    mapProductAttribute.insertMulti(_art, QString("%1||%2")
                                        .arg(attribute_get_id("Цвет", 1)).arg(nAtt.firstChildElement("Значение").text()));
                }
            }
        }
    }
    file.close();

    log.append(QString("%1 - получено категорий: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapGroup.size()));
    log.append(QString("%1 - получено атрибутов: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProductAttribute.size()));
    log.append(QString("%1 - получено товаров: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProduct.size()));

}

void ocMeridiaAuto::main_file_upload()
{
    if (mapProduct.count() > 0){
        QSettings sett("setting.ini", QSettings::IniFormat);
        int _id;
        QString _art;
        double _price;
        for (int x = 0; x < mapProduct.count(); x++){
            _art = mapProduct.keys().at(x);
            _id = mapProduct.value(_art).split("||").at(0).toInt();
            _price = mapProduct.value(_art).split("||").at(6).toDouble();

            bool idxIN = false;
            bool idxUP = false;
            if (_id == 0){
                idxIN = true;
                idxUP = false;
                if (sett.value("load1c/new_product").toBool()){
                    idxIN = true;
                    if (_price == 0){
                        if (sett.value("load1c/not_price").toBool()){
                            idxIN = true;
                        } else {
                            idxIN = true;
                        }
                    }
                } else {
                    idxIN = false;
                }
            } else if (_id > 0){
                idxIN = false;
                idxUP = true;
                if (_price == 0){
                    if (sett.value("load1c/not_price").toBool()){
                        idxUP = true;
                    } else {
                        idxUP = false;
                    }
                }
            }
            if (idxIN){
                product_insert(_art);
            } else if (idxUP){
                product_update(_art, 1);
            }
        }
    }
}

void ocMeridiaAuto::pl_file_get()
{
    QSqlQuery queryF("SELECT files.fname, files.schema "
                     "FROM files "
                     "WHERE files.vid = \'0\' AND files.active = \'1\' ", db_local);
    while (queryF.next()){
        QString fname = queryF.value(0).toString();
        QString schema = queryF.value(1).toString();
        QFile file(fname);
        if (file.open(QIODevice::ReadOnly)){
            csvData.clear();
            while (!file.atEnd()){
                QByteArray ar = file.readLine();
                csvData.append(ar);
            }
        }
        file.close();
        if (csvData.size() > 0){
            pl_file_read(schema);
        }
    }
}

void ocMeridiaAuto::pl_file_read(const QString schema)
{
    mapGroup.clear();
    mapAttributes.clear();
    mapProduct.clear();
    mapProductAttribute.clear();
    mapProductImage.clear();

    QSettings maket(schema, QSettings::IniFormat);
    QSettings sett("setting.ini", QSettings::IniFormat);

    int first_row = maket.value("firstRow").toInt();
    QString splA = maket.value("split").toString();
    int col_art = maket.value("art").toInt() - 1;
    int col_model = maket.value("model").toInt() - 1;
    int col_name = maket.value("name").toInt() - 1;
    int col_manufacturer = maket.value("manuf").toInt() - 1;
    int col_price = maket.value("price").toInt() - 1;
    int col_quan = maket.value("quan").toInt() - 1;

    QList<int> column_list_desc;
    for (int z = 0; z < maket.value("desc").toString().split("|").size(); z++){
        column_list_desc.append(maket.value("desc").toString().split("|").at(z).toInt() - 1);
    }

    QList<int> column_list_group;
    for (int z = 0; z < maket.value("cat").toString().split("|").size(); z++){
        column_list_group.append(maket.value("cat").toString().split("|").at(z).toInt() - 1);
    }

    QList<int> column_list_image; // номера колонок изображений
    for (int z = 0; z < maket.value("image").toString().split("|").size(); z++){
        column_list_image.append(maket.value("image").toString().split("|").at(z).toInt() - 1);
    }
    QList<int> column_list_attr; // номера колонок аттрибутов
    for (int z = 0; z < maket.value("attr").toString().split("|").size(); z++){
        column_list_attr.append(maket.value("attr").toString().split("|").at(z).toInt() - 1);
    }


    // --category
    QStringList _categoryList;
    QString group_line;
    for (int x = 0; x < csvData.size(); x++){
        group_line.clear();
        group_line.append(csvData.at(x).split(splA).at(column_list_group.at(0)));
        if (column_list_group.size() > 1){
            for (int a = 1; a < column_list_group.size(); a++){
                group_line.append(QString("|%1").arg(csvData.at(x).split(splA).at(column_list_group.at(a))));
            }
        }
        _categoryList.append(group_line);

    }
    _categoryList.removeDuplicates();
    for (int x = 0; x < _categoryList.size(); x++){
        int numRow = x - first_row + 1;
        if (numRow >= 0){
            mapGroup.insert(_categoryList.at(x), group_get_id(_categoryList.at(x),
                                                              _categoryList.at(x).split("|").at(_categoryList.at(x).split("|").size() - 1),
                                                              2));
        }
    }
    //удаление первых строк
    for (int r = first_row - 2; r >= 0; r--){
        mapGroup.remove(mapGroup.keys().at(r));
    }

    //****************************************************
    //------------attributes-----------------
    QStringList _attributeList;
    if ( maket.value("attr_check").toBool()){
        QString lineAttributeName = csvData.at(first_row - 2); //строка данных c заголовками

        for (int a = 0; a < column_list_attr.size(); a++){
            _attributeList.append(lineAttributeName.split(splA).at(column_list_attr.at(a)));
        }
        for (int row = 0; row < _attributeList.size(); row++){
            mapAttributes.insert(_attributeList.at(row), attribute_get_id(_attributeList.at(row), 2));
        }
    }
    //*****************************************************
    //------------products---------------------------------
    //собираем данные
    for (int row = 0; row < 100/*csvData.size()*/; row++){

        //variable
        int _id = 0;
        QString _art = "";
        QString _model = "";
        QString _name = "";
        int _id_group = 0;
        int _manufacturer = 0;
        double _price = 0.00;
        double _quan = sett.value("oc/quan").toDouble();
        QString _desc = "";
        QString _image = "";

        if (maket.value("art_check").toBool()){
            _art.append(csvData.at(row).split(splA).at(col_art));
        }
        if (maket.value("model_check").toBool()){
            _model.append(csvData.at(row).split(splA).at(col_model));
        }
        if (maket.value("name_check").toBool()){
            _name.append(csvData.at(row).split(splA).at(col_name));
        }
        if (maket.value("cat_check").toBool()){
            QString product_group = csvData.at(row).split(splA).at(column_list_group.at(0));
            if (column_list_group.size() > 1){
                for (int a = 1; a < column_list_group.size(); a++){
                    product_group.append(QString("|%1").arg(csvData.at(row).split(splA).at(column_list_group.at(a))));
                }
            }
            _id_group = mapGroup.value(product_group);
        }
        if (maket.value("manuf_check").toBool()){
            _manufacturer = manufacturer_get_id(csvData.at(row).split(splA).at(col_manufacturer));
        } else {
            _manufacturer = manufacturer_get_id("no");
        }

        if (maket.value("price_check").toBool()){
            _price = csvData.at(row).split(splA).at(col_price).toDouble();
        }

        if (maket.value("quan_check").toBool()){
            _quan = csvData.at(row).split(splA).at(col_quan).toDouble();
        }

        if (maket.value("desc_check").toBool()){
            for (int a = 0; a < column_list_desc.size(); a++){
                _desc.append(QString("%1 ").arg(csvData.at(row).split(splA).at(column_list_desc.at(a))));
            }
        }

        if (maket.value("image_check").toBool()){
            // первую позицию в таблицу товаров
            _image = csvData.at(row).split(splA).at(column_list_image.at(0));
            for (int a = 1; a < column_list_image.size(); a++){
                int numRow = row - first_row + 1; //номер строки связанного товара
                if (numRow >= 0){
                    mapProductImage.insertMulti(QString::number(numRow), csvData.at(row).split(splA).at(column_list_image.at(a)));
                }

            }
        }

        if (maket.value("attr_check").toBool()){
            for (int a = 0; a < column_list_attr.size(); a++){
                int numRow = row - first_row + 1; //номер строки связанного товара
                if (numRow >= 0){
                    mapProductAttribute.insertMulti(QString::number(numRow), QString("%1||%2")
                                                    .arg(mapAttributes.value(_attributeList.at(a)))
                                                    .arg(csvData.at(row).split(splA).at(column_list_attr.at(a))));
                }
            }
        }

        _id = product_get_id(_art, _name);
        int numRow = row - first_row + 1; //номер строки связанного товара
        if (numRow >= 0){
            mapProduct.insert(QString::number(row - 1), QString("%1||%2||%3||%4||%5||%6||%7||%8||%9||%10")
                              .arg(_id)
                              .arg(_art)
                              .arg(_model)
                              .arg(_name)
                              .arg(_id_group)
                              .arg(_manufacturer)
                              .arg(_price)
                              .arg(_quan)
                              .arg(_desc)
                              .arg(_image));
        }
    }
    log.append(QString("%1 - pl - получено категорий: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapGroup.size()));
    log.append(QString("%1 - pl - получено атрибутов: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapAttributes.size()));
    log.append(QString("%1 - pl - получено товаров: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProduct.size()));
    log.append(QString("%1 - pl - получено атрибутов товаров: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProductAttribute.size()));
    log.append(QString("%1 - pl - получено изображений товаров: %2")
               .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
               .arg(mapProductImage.size()));
}

void ocMeridiaAuto::pl_file_upload()
{
    if (mapProduct.count() > 0){
        QSettings sett("setting.ini", QSettings::IniFormat);
        int _id;
        QString row;
        double _price;
        for (int x = 0; x < mapProduct.count(); x++){
            row = mapProduct.keys().at(x);
            _id = mapProduct.value(row).split("||").at(0).toInt();
            _price = mapProduct.value(row).split("||").at(6).toDouble();

            bool idxIN = false;
            bool idxUP = false;
            if (_id == 0){
                idxIN = true;
                idxUP = false;
                if (sett.value("loadpl/new_product").toBool()){
                    idxIN = true;
                    if (_price == 0){
                        if (sett.value("loadpl/not_price").toBool()){
                            idxIN = true;
                        } else {
                            idxIN = false;
                        }
                    }
                } else {
                    idxIN = false;
                }
            } else if (_id > 0){
                idxIN = false;
                idxUP = true;
                if (_price == 0){
                    if (sett.value("loadpl/not_price").toBool()){
                        idxUP = true;
                    } else {
                        idxUP = false;
                    }
                }
            }
            if (idxIN){
                product_insert(row);
            } else if (idxUP){
                product_update(row, 2);
            }
        }
    }
}

int ocMeridiaAuto::group_get_id(const QString _c, const QString _n, int _v)
{
    int _id = 0;
    QSqlQuery queryA(QString("SELECT rmrt_category.category_id "
                            "FROM rmrt_category "
                            "WHERE rmrt_category.code = \'%0\' ")
                    .arg(_c), db_server);
    queryA.next();
    if (queryA.isValid()){
        _id = queryA.value(0).toInt();
    } else {
        //test2: проверяем наличие записи по имени
        QSqlQuery queryB(QString("SELECT rmrt_category_description.category_id "
                                  "FROM rmrt_category_description "
                                  "WHERE rmrt_category_description.name = \"%1\" ")
                          .arg(_n), db_server);
        queryB.next();
        if (queryB.isValid()){
            _id = queryB.value(0).toInt();
            //если такая группа есть: добавляем код1С, подставляем ID в таблицу товары, удаляем строку
            QSqlQuery queryU(QString("UPDATE rmrt_category SET code = \'%0\' "
                                     "WHERE rmrt_category.category_id = \'%1\' ")
                             .arg(_c)
                             .arg(_n), db_server);
            queryU.exec();
        } else {
            // проверяем наличие категории в синонимах
            QSqlQuery queryL(QString("SELECT cat.id_db FROM cat WHERE cat.cod = \'%1\'")
                             .arg(_c)
                             , db_local);
            queryL.next();
            if (queryL.isValid()){
                _id = queryL.value(0).toInt();
            } else {
                QSettings sett("setting.ini", QSettings::IniFormat);
                if (_v == 1){ //main file
                    _id = category_add(_c, _n, sett.value("load1c/group").toString());
                } else if (_v == 2){ //pl file
                    _id = category_add(_c, _n, sett.value("loadpl/group").toString());
                }

            }
        }
    }
    return _id;
}

int ocMeridiaAuto::manufacturer_get_id(const QString _n)
{
    int _id = 0;
    QSqlQuery query_test(QString("SELECT rmrt_manufacturer_description.manufacturer_id "
                                 "FROM rmrt_manufacturer_description "
                                 "WHERE rmrt_manufacturer_description.language_id = \'%1\' "
                                 "AND rmrt_manufacturer_description.name = \'%2\' ")
                         .arg(_LANG)
                         .arg(_n), db_server);
    query_test.next();
    if (query_test.isValid()){
        _id = query_test.value(0).toInt();
    } else {
        QSqlQuery query_add("INSERT INTO rmrt_manufacturer (name, image, sort_order) "
                                    "VALUES (?, ?, ?)", db_server);
        query_add.bindValue(0, _n);
        query_add.bindValue(1, "");
        query_add.bindValue(2, 0);
        query_add.exec();
        if (query_add.lastError().text().size() > 3){
            //error.append(query_add.lastError().text());
        } else {
            _id = query_add.lastInsertId().toInt();
            // desc
            QSqlQuery queryDesc("INSERT INTO rmrt_manufacturer_description (manufacturer_id, language_id, name, description) "
                                "VALUES (?, ?, ?, ?)", db_server);
            queryDesc.bindValue(0, _id);
            queryDesc.bindValue(1, _LANG);
            queryDesc.bindValue(2, _n);
            queryDesc.bindValue(3, _n);
            queryDesc.exec();
            if (queryDesc.lastError().text().size() > 3){
                //error.append(queryDesc.lastError().text());
            }
            //store
            QSqlQuery queryStore("INSERT INTO rmrt_manufacturer_to_store (manufacturer_id, store_id) "
                                                    "VALUES (?, ?)", db_server);
            queryStore.bindValue(0, _id);
            queryStore.bindValue(1, _STORE);
            queryStore.exec();
            if (queryStore.lastError().text().size() > 3){
                //error.append(queryStore.lastError().text());
            }
        }
    }
    return _id;
}

int ocMeridiaAuto::attribute_get_id(const QString _n, int _v)
{
    int _id = 0;
    QSqlQuery query(QString("SELECT rmrt_attribute_description.attribute_id "
                            "FROM rmrt_attribute_description "
                            "WHERE rmrt_attribute_description.name = \'%1\' ")
                    .arg(_n), db_server);
    query.next();
    if (query.isValid()){
        _id = query.value(0).toInt();
    } else {
        QSqlQuery queryL(QString("SELECT att.id_db FROM att WHERE att.name = \'%1\' ")
                         .arg(_n), db_local);
        queryL.next();
        if (queryL.isValid()){
            _id = queryL.value(0).toInt();
        } else {
            QSettings sett("setting.ini", QSettings::IniFormat);
            QSqlQuery query_add("INSERT INTO rmrt_attribute (attribute_group_id, sort_order) "
                            "VALUES (?, ?)", db_server);
            if (_v == 1){
                query_add.bindValue(0, sett.value("load1c/attribute").toInt());
            } else if (_v == 2){
                query_add.bindValue(0, sett.value("loadpl/attribute").toInt());
            }
            query_add.bindValue(1, 0);
            query_add.exec();
            if (query_add.lastError().text().size() > 3){
                //error.append(query_add.lastError().text());
            } else {
                _id = query_add.lastInsertId().toInt();
                QSqlQuery queryB("INSERT INTO rmrt_attribute_description (attribute_id, language_id, name) "
                                 "VALUES (?, ?, ?)", db_server);
                queryB.bindValue(0, _id);
                queryB.bindValue(1, _LANG);
                queryB.bindValue(2, _n);
                queryB.exec();
                if (queryB.lastError().text().size() > 3){
                 //   error.append(queryB.lastError().text());
                }
            }
        }
    }
    return _id;
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
        error.append(QString("category_add | query C -- error: %1").arg(query_C.lastError().text()));
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
            error.append(QString("category_add | query CD -- error: %1").arg(query_CD.lastError().text()));
        }
        //store
        QSqlQuery query_CS("INSERT INTO `rmrt_category_to_store` (`category_id`, `store_id`) VALUES (?, ?)", db_server);
        query_CS.bindValue(0, cat_ID);
        query_CS.bindValue(1, _STORE);
        query_CS.exec();
        if (query_CS.lastError().text().size() > 3){
            error.append(QString("category_add | query CS -- error: %1").arg(query_CS.lastError().text()));
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
    } else {
        log.append(QString("%1 - Создания категория -- %2 | %3 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(cat_ID)
                   .arg(cat_name));
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

int ocMeridiaAuto::product_get_id(const QString prod_code, const QString prod_name)
{
    //проверяем по коду 1с
    int _id = 0;
    QSqlQuery queryS(QString("SELECT rmrt_product.product_id "
                             "FROM rmrt_product "
                             "WHERE rmrt_product.sku = \'%1\' ")
                     .arg(prod_code), db_server);
    queryS.next();
    if (queryS.isValid()){
        _id = queryS.value(0).toInt();
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
            _id = queryS2.value(0).toInt();
            QSqlQuery queryU(QString("UPDATE rmrt_product "
                                     "SET sku = \'%0\' "
                                     "WHERE rmrt_product.product_id = \'%1\' ")
                             .arg(prod_code)
                             .arg(_id), db_server);
            queryU.exec();
        }
    }
    return _id;
}

void ocMeridiaAuto::product_insert(const QString key)
{
    QString error;
    QSettings sett("setting.ini", QSettings::IniFormat);

    int _id = 0;
    QString _art = mapProduct.value(key).split("||").at(1);
    QString _model = mapProduct.value(key).split("||").at(2);
    QString _name = mapProduct.value(key).split("||").at(3);
    int _id_group = mapProduct.value(key).split("||").at(4).toInt();
    int _manufacturer = mapProduct.value(key).split("||").at(5).toInt();
    double _price = mapProduct.value(key).split("||").at(6).toDouble();
    double _quan = mapProduct.value(key).split("||").at(7).toDouble();
    QString _desc = mapProduct.value(key).split("||").at(8);
    QString _image = mapProduct.value(key).split("||").at(9);

    QSqlQuery queryAddProduct("INSERT INTO rmrt_product (sku, model, quantity, manufacturer_id, price, image, "
                              "stock_status_id, tax_class_id, status,  date_added, date_modified) "
                              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", db_server);
    queryAddProduct.bindValue(0, _art);
    queryAddProduct.bindValue(1, _model);
    queryAddProduct.bindValue(2, _quan);
    queryAddProduct.bindValue(3, _manufacturer);
    queryAddProduct.bindValue(4, _price);
    queryAddProduct.bindValue(5, _image);
    queryAddProduct.bindValue(6, _STOCK);
    queryAddProduct.bindValue(7, _TAX);
    queryAddProduct.bindValue(8, _STATUS);
    queryAddProduct.bindValue(9, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    queryAddProduct.bindValue(10, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
    queryAddProduct.exec();
    if (queryAddProduct.lastError().text().size() > 3){
        error.append(QString("product_insert | queryAddProduct - error: %1").arg(queryAddProduct.lastError().text()));
    } else {
        _id = queryAddProduct.lastInsertId().toInt();
        //добавляем description
        QSqlQuery queryAddProdDesc("INSERT INTO rmrt_product_description (product_id, language_id, name, description) "
                                   "VALUES (?, ?, ?, ?)", db_server);
        queryAddProdDesc.bindValue(0, _id);
        queryAddProdDesc.bindValue(1, _LANG);
        queryAddProdDesc.bindValue(2, _name);
        queryAddProdDesc.bindValue(3, _desc);
        queryAddProdDesc.exec();
        if (queryAddProdDesc.lastError().text().size() > 3){
          error.append(QString("product_insert | queryAddProdDesc - error: %1").arg(queryAddProdDesc.lastError().text()));
        }

        //добавляем store
        QSqlQuery queryAddProdStore("INSERT INTO rmrt_product_to_store (product_id, store_id) "
                                   "VALUES (?, ?)", db_server);
        queryAddProdStore.bindValue(0, _id);
        queryAddProdStore.bindValue(1, _STORE);
        queryAddProdStore.exec();
        if (queryAddProdStore.lastError().text().size() > 3){
          error.append(QString("product_insert | queryAddProdStore - error: %1").arg(queryAddProdStore.lastError().text()));
        }
        //добавляем category
        QSqlQuery queryAddProdCat("INSERT INTO rmrt_product_to_category (product_id, category_id, main_category) "
                                   "VALUES (?, ?, ?)", db_server);
        queryAddProdCat.bindValue(0, _id);
        queryAddProdCat.bindValue(1, _id_group);
        queryAddProdCat.bindValue(2, _id_group);
        queryAddProdCat.exec();
        if (queryAddProdCat.lastError().text().size() > 3){
          error.append(QString("product_insert | queryAddProdCat - error: %1").arg(queryAddProdCat.lastError().text()));
        }

        //добавляем аттрибуты
        if (mapProductAttribute.count() > 0){
            QStringList values = mapProductAttribute.values(key);
            if (values.size() > 0){
                for (int x = 0; x < values.size(); x++){
                    int attribute_id = values.at(x).split("||").at(0).toInt();
                    QString attribute_value = values.at(x).split("||").at(1);
                    QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                    "VALUES (?, ?, ?, ?)", db_server);
                    queryAddProdAttribute.bindValue(0, _id);
                    queryAddProdAttribute.bindValue(1, attribute_id);
                    queryAddProdAttribute.bindValue(2, _LANG);
                    queryAddProdAttribute.bindValue(3, attribute_value);
                    queryAddProdAttribute.exec();
                    if (queryAddProdAttribute.lastError().text().size() > 3){
                      error.append(QString("product_insert | queryAddProdAttribute - error: %1").arg(queryAddProdAttribute.lastError().text()));
                    }
                }
            }
        }
        //добавляем изображения
        if (mapProductImage.count() > 0){
            QStringList values = mapProductImage.values(key);
            if (values.size() > 0){
                for (int x = 0; x < values.size(); x++){
                    QString image_value = values.at(x);
                    QSqlQuery queryAddProdImage("INSERT INTO rmrt_product_image (product_id, image, sort_order) "
                                                    "VALUES (?, ?, ?)", db_server);
                    queryAddProdImage.bindValue(0, _id);
                    queryAddProdImage.bindValue(1, image_value);
                    queryAddProdImage.bindValue(2, 0);
                    queryAddProdImage.exec();
                    if (queryAddProdImage.lastError().text().size() > 3){
                      error.append(QString("product_insert | queryAddProdImage - error: %1").arg(queryAddProdImage.lastError().text()));
                    }
                }
            }
        }
    }
    if (error.size() > 0){
        log.append(QString("%1 - Ошибка загрузки товаров: %2 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(error));
    } else {
        log.append(QString("%1 - Добавлен товар: %2 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(_name));
    }
}

void ocMeridiaAuto::product_update(const QString key, int _v)
{
    QString error;
    QSettings sett("setting.ini", QSettings::IniFormat);

    int _id = mapProduct.value(key).split("||").at(0).toInt();
    QString _art = mapProduct.value(key).split("||").at(1);//sku
    QString _model = mapProduct.value(key).split("||").at(2);//model
    QString _name = mapProduct.value(key).split("||").at(3); //name
    int _group = mapProduct.value(key).split("||").at(4).toInt();
    int _manufacturer = mapProduct.value(key).split("||").at(5).toInt();//manufacturer_id
    double _price = mapProduct.value(key).split("||").at(6).toDouble();//price
    double _quan = mapProduct.value(key).split("||").at(7).toDouble();//quantity
    QString _desc = mapProduct.value(key).split("||").at(8); //desc
    QString _image = mapProduct.value(key).split("||").at(9);//image

    if (_v == 1){
        QString mainStr;
        if (sett.value("load1c/up_name").toBool()){
            mainStr.append(QString("UPDATE rmrt_product_description SET `name` = \'%1\' "
                                   "WHERE rmrt_product_description.product_id = \'%2\' "
                                   "AND rmrt_product_description.language_id = \'%3\'; ")
                           .arg(_name)
                           .arg(_id)
                           .arg(_LANG));
        }
        if (sett.value("load1c/up_manuf").toBool()){
            mainStr.append(QString("UPDATE rmrt_product SET manufacturer_id = \'%1\' "
                                   "WHERE rmrt_product.product_id = \'%2\'; ")
                           .arg(_manufacturer)
                           .arg(_id));
        }

        if (sett.value("load1c/up_price").toBool()){
            mainStr.append(QString("UPDATE rmrt_product SET price = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                           .arg(_price)
                           .arg(_id));
        }
        if (sett.value("load1c/up_quan").toBool()){
            mainStr.append(QString("UPDATE rmrt_product SET quantity = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                           .arg(_quan)
                           .arg(_id));
        }
        if (sett.value("load1c/up_desc").toBool()){
            mainStr.append(QString("UPDATE rmrt_product_description SET description = \'%1\' "
                                   "WHERE rmrt_product_description.product_id = \'%2\' "
                                   "AND rmrt_product_description.language_id = \'%3\'; ")
                           .arg(_desc)
                           .arg(_id)
                           .arg(_LANG));
        }
        if (sett.value("load1c/up_image").toBool()){
            mainStr.append(QString("UPDATE rmrt_product SET image = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                           .arg(_image)
                           .arg(_id));
        }
        if (!mainStr.isEmpty()){
            QSqlQuery queryUp(mainStr, db_server);
            queryUp.exec();
            if (queryUp.lastError().text().size() > 3){
              error.append(QString("product_update | queryUp - error: %1").arg(queryUp.lastError().text()));
            }
        }


        if (sett.value("load1c/up_attribute").toBool()){
            // удаляем старые аттрибуты
            QSqlQuery queryD(QString("DELETE FROM rmrt_product_attribute "
                                     "WHERE rmrt_product_attribute.product_id = \'%1\' ").arg(_id), db_server);
            queryD.exec();
            //добавляем аттрибуты
            if (mapProductAttribute.count() > 0){
                QStringList values = mapProductAttribute.values(key);
                if (values.size() > 0){
                    for (int x = 0; x < values.size(); x++){
                        int attribute_id = values.at(x).split("||").at(0).toInt();
                        QString attribute_value = values.at(x).split("||").at(1);
                        QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                        "VALUES (?, ?, ?, ?)", db_server);
                        queryAddProdAttribute.bindValue(0, _id);
                        queryAddProdAttribute.bindValue(1, attribute_id);
                        queryAddProdAttribute.bindValue(2, _LANG);
                        queryAddProdAttribute.bindValue(3, attribute_value);
                        queryAddProdAttribute.exec();
                        if (queryAddProdAttribute.lastError().text().size() > 3){
                          error.append(QString("product_update | queryAddProdAttribute - error: %1").arg(queryAddProdAttribute.lastError().text()));
                        }
                    }
                }
            }
        }
    } else if (_v == 2){
        if(sett.value("loadpl/up_all").toBool()){
            QSqlQuery queryUpMain(QString("UPDATE rmrt_product SET sku = \'%1\', model = \'%2\', manufacturer_id = \'%3\', price = \'%4\', "
                                          "quantity = \'%5\', image = \'%6\'"
                                          "WHERE rmrt_product.product_id = \'%7\' ")
                                  .arg(_art)
                                  .arg(_model)
                                  .arg(_manufacturer)
                                  .arg(_price)
                                  .arg(_quan)
                                  .arg(_image)
                                  .arg(_id), db_server);
            queryUpMain.exec();
            if (queryUpMain.lastError().text().size() > 3){
              error.append(QString("product_update | queryUpMain - error: %1").arg(queryUpMain.lastError().text()));
            }
            QSqlQuery queryUpDesc(QString("UPDATE rmrt_product_description SET name = \'%1\', description = \'%2\' "
                                          "WHERE rmrt_product_description.product_id = \'%2\' "
                                          "AND rmrt_product_description.language_id = \'%3\' ")
                                  .arg(_name)
                                  .arg(_desc)
                                  .arg(_id)
                                  .arg(_LANG), db_server);
            queryUpDesc.exec();
            if (queryUpDesc.lastError().text().size() > 3){
              error.append(QString("product_update | queryUpDesc - error: %1").arg(queryUpDesc.lastError().text()));
            }
            //clear product_to_category, product_image, product_attribute
            QSqlQuery queryClear(QString("DELETE FROM rmrt_product_to_category WHERE rmrt_product_to_category.product_id = \'%1\'; "
                                         "DELETE FROM rmrt_product_attribute WHERE rmrt_product_attribute.product_id = \'%1\'; "
                                         "DELETE FROM rmrt_product_image WHERE rmrt_product_image.product_id = \'%1\'; ")
                                 .arg(_id), db_server);
            queryClear.exec();
            if (queryClear.lastError().text().size() > 3){
              error.append(QString("product_update | queryClear - error: %1").arg(queryClear.lastError().text()));
            }
            //добавляем category
            QSqlQuery queryAddProdCat("INSERT INTO rmrt_product_to_category (product_id, category_id, main_category) "
                                       "VALUES (?, ?, ?)", db_server);
            queryAddProdCat.bindValue(0, _id);
            queryAddProdCat.bindValue(1, _group);
            queryAddProdCat.bindValue(2, _group);
            queryAddProdCat.exec();
            if (queryAddProdCat.lastError().text().size() > 3){
              error.append(QString("product_update | queryAddProdCat - error: %1").arg(queryAddProdCat.lastError().text()));
            }
            //добавляем аттрибуты
            if (mapProductAttribute.count() > 0){
                QStringList values = mapProductAttribute.values(key);
                if (values.size() > 0){
                    for (int x = 0; x < values.size(); x++){
                        int attribute_id = values.at(x).split("||").at(0).toInt();
                        QString attribute_value = values.at(x).split("||").at(1);
                        QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                        "VALUES (?, ?, ?, ?)", db_server);
                        queryAddProdAttribute.bindValue(0, _id);
                        queryAddProdAttribute.bindValue(1, attribute_id);
                        queryAddProdAttribute.bindValue(2, _LANG);
                        queryAddProdAttribute.bindValue(3, attribute_value);
                        queryAddProdAttribute.exec();
                        if (queryAddProdAttribute.lastError().text().size() > 3){
                          error.append(QString("product_update | queryAddProdAttribute - error: %1").arg(queryAddProdAttribute.lastError().text()));
                        }
                    }
                }
            }
            //добавляем изображения
            if (mapProductImage.count() > 0){
                QStringList values = mapProductImage.values(key);
                if (values.size() > 0){
                    for (int x = 0; x < values.size(); x++){
                        QString image_value = values.at(x);
                        QSqlQuery queryAddProdImage("INSERT INTO rmrt_product_image (product_id, image, sort_order) "
                                                        "VALUES (?, ?, ?)", db_server);
                        queryAddProdImage.bindValue(0, _id);
                        queryAddProdImage.bindValue(1, image_value);
                        queryAddProdImage.bindValue(2, 0);
                        queryAddProdImage.exec();
                        if (queryAddProdImage.lastError().text().size() > 3){
                          error.append(QString("product_update | queryAddProdImage - error: %1").arg(queryAddProdImage.lastError().text()));
                        }
                    }
                }
            }
        } else if (sett.value("loadpl/up_price").toBool()){
            QSqlQuery queryUpMain(QString("UPDATE rmrt_product SET price = \'%1\' WHERE rmrt_product.product_id = \'%2\' ")
                                  .arg(_price)
                                  .arg(_id), db_server);
            queryUpMain.exec();
            if (queryUpMain.lastError().text().size() > 3){
              error.append(QString("product_update | queryUpMain - error: %1").arg(queryUpMain.lastError().text()));
            }
        }
    }

    if (error.size() > 0){
        log.append(QString("%1 - Ошибка обновления товаров: %2 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(error));
    } else {
        log.append(QString("%1 - Обновлен: %2 *")
                   .arg(QDateTime::currentDateTime().toString("hh:mm:ss dd.MM.yyyy"))
                   .arg(_name));
    }
}

void ocMeridiaAuto::writeLog()
{
    QFile file(QString("log_%1.txt").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm")));
    if (file.open(QIODevice::WriteOnly | QFile::Truncate)){
        QTextStream out(&file);
        for (int x = 0; x < log.size(); x++){
            out << log.at(x) << '\r' <<'\n';
        }
    }
}

void ocMeridiaAuto::scenario_A()
{
    main_file_get();
    main_file_read();
    main_file_upload();
}

void ocMeridiaAuto::scenario_B()
{
    pl_file_get();
    pl_file_upload();
}


