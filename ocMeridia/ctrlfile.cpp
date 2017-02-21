#include "ctrlfile.h"

ctrlFile::ctrlFile()
{
    cPref = new preference();
}

void ctrlFile::readXML(QString _fname)
{
    //опция только одна - Цвет
    listOptions.clear();
    listOptions << "Цвет";

    mapGroup.clear();

    QFile file(_fname);
    if (file.open(QIODevice::ReadOnly)){
        QDomDocument *doc = new QDomDocument();
        doc->setContent(&file);

        int key_option = 0;


        //reading groups
        readXMLGroups(doc);
        //attributes
        readXMLAttributes(doc);
        //reading products
        QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
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
        QMap<QString, QVariant> product;
        QDomNode nodeProducts = nodeAll.childNodes().at(idxProd);


        for (int y = 0; y < nodeProducts.childNodes().count(); y++){ //****

            QDomNode nodeProduct = nodeProducts.childNodes().at(y);
            QString _ID = nodeProduct.firstChildElement("Ид").text();
            product.insert("articul", _ID);

            //переменные, которых нет в файле
            // производители
            product.insert("manufacturerId", "no");
            //количество
            product.insert("quantity", cPref->get_XML_newQuan());

            if (_ID.size() == 36){ // обычный товар

                QString fullName = nodeProduct.firstChildElement("ПолноеНаименование").text();
                fullName.remove("\"");
                fullName.remove("|");
                fullName.remove("\'");

                QString smallName = nodeProduct.firstChildElement("Наименование").text();
                smallName.remove("\"");
                smallName.remove("|");
                smallName.remove("\'");

                product.insert("model", smallName);
                product.insert("name", fullName);
                product.insert("price", mapPrice.value(_ID));
                product.insert("image", nodeProduct.firstChildElement("Картинка").text());
                product.insert("desc", nodeProduct.firstChildElement("Описание").text());
                product.insert("groupId", nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text());


                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ИД-Наличие"){ //Наличие
                        QString _ss = nAtt.firstChildElement("Значение").text();
                        if (_ss == "true"){
                            //TODO StockStatus получение ИД по Названию
                            product.insert("stockStatus", "В наличие");
                            product.insert("stockStatusID", "7");
                        } else if (_ss == "false"){
                            product.insert("stockStatus", "Нет в наличии");
                            product.insert("stockStatusID", "5");
                        }
                    }
                }

                mapProduct.insert(_ID, product);
            } else if (_ID.size() > 36){
                // находим правильный ИД товара
                QString oldID = _ID;
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ИДРодителя"){
                        _ID =  nAtt.firstChildElement("Значение").text();
                    }
                }
                //проверяем есть ли такой товар в карте
                if (mapProduct.contains(_ID)){
                    //если есть - собираем опции                    
                    for (int x = 0; x < elAttributes.childNodes().count(); x++){
                        QDomNode nAtt = elAttributes.childNodes().at(x);
                        if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){
                            QMap<QString, QVariant> _prodOpt;
                            _prodOpt.insert("productId", _ID);
                            _prodOpt.insert("name", "Цвет");
                            _prodOpt.insert("value", nAtt.firstChildElement("Значение").text());
                            _prodOpt.insert("price", mapPrice.value(oldID));
                            _prodOpt.insert("quan", 0);
                            mapProductOption.insert(key_option, _prodOpt);
                            key_option++;
                        }
                    }
                } else {
                    //если нет - добавляем товар
                    product.insert("articul", _ID);

                    QString fullName = nodeProduct.firstChildElement("ПолноеНаименование").text();
                    fullName.remove("\"");
                    fullName.remove("|");
                    fullName.remove("\'");

                    QString smallName = nodeProduct.firstChildElement("Наименование").text();
                    smallName.remove("\"");
                    smallName.remove("|");
                    smallName.remove("\'");

                    product.insert("model", smallName);
                    product.insert("name", fullName);
                    product.insert("price", mapPrice.value(oldID));
                    product.insert("image", nodeProduct.firstChildElement("Картинка").text());
                    product.insert("desc", nodeProduct.firstChildElement("Описание").text());
                    product.insert("groupId", nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text());
                    QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                    for (int x = 0; x < elAttributes.childNodes().count(); x++){
                        QDomNode nAtt = elAttributes.childNodes().at(x);
                        //Опции
                        if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ ////атрибут товара Цвет
                            QMap<QString, QVariant> _prodOpt;
                            _prodOpt.insert("productId", _ID);
                            _prodOpt.insert("name", "Цвет");
                            _prodOpt.insert("value", nAtt.firstChildElement("Значение").text());
                            _prodOpt.insert("price", mapPrice.value(oldID));
                            _prodOpt.insert("quan", 0);
                            mapProductOption.insert(key_option, _prodOpt);
                            key_option++;
                        } else if (nAtt.firstChildElement("Ид").text() == "ИД-Наличие"){ //Наличие
                            QString _ss = nAtt.firstChildElement("Значение").text();
                            if (_ss == "true"){
                                //TODO StockStatus получение ИД по Названию
                                product.insert("stockStatus", "В наличие");
                                product.insert("stockStatusID", "7");
                            } else if (_ss == "false"){
                                product.insert("stockStatus", "Нет в наличии");
                                product.insert("stockStatusID", "5");
                            }
                        }
                    }
                    mapProduct.insert(_ID, product);
                }
            }
        }
    }
}

void ctrlFile::readXMLGroups(QDomDocument *doc)
{
    mapGroup.clear();
    QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
    QDomNode nodeCl = nodeCom.childNodes().at(0); //классификатор

    //reading groups

    QDomNode nodeGroups = nodeCl.childNodes().at(2);

    for (int x = 0; x < nodeGroups.childNodes().count(); x++){
        QDomNode nodeGroup = nodeGroups.childNodes().at(x);

        QMap<QString,QVariant> group;
        group.insert("code", nodeGroup.firstChildElement("Ид").text());
        group.insert("name", nodeGroup.firstChildElement("Наименование").text());
        group.insert("parent", nodeGroup.firstChildElement("Родитель").text());
        mapGroup.insert(x, group);
    }

}

void ctrlFile::readXMLAttributes(QDomDocument *doc)
{
    if (doc->hasChildNodes()){
        mapAttribute.clear();
        QMap<QString,QVariant> attribute;
        attribute.insert("name", "Цвет");
        mapAttribute.insert(0, attribute);
    }
}

void ctrlFile::readCSV(QStringList _data, QString _maket)
{
    readCSVGroups(_data, _maket);
    readCSVAttribute(_data, _maket);
    readCSVProduct(_data, _maket);
    readCSVProductAttribute(_data, _maket);
    readCSVProductImage(_data, _maket);
}

void ctrlFile::readCSVGroups(QStringList _data, QString _maket)
{
    mapGroup.clear();

    QSettings *maket = new QSettings(_maket, QSettings::IniFormat);

    QString SPL = maket->value("split").toString();
    QString PRE = maket->value("prefix").toString();
    int FROW = maket->value("firstRow").toInt();


    for (int x = FROW + 1; x >= 0; x--){
        _data.removeAt(x);
    }

    QStringList _list;

    for (int x = 0; x < _data.size(); x++){
        QStringList _line = _data.at(x).split(SPL);

        QStringList _groupList = maket->value("cat").toString().split("|");

        QString catCode = PRE;
        QString catName;

        for (int a = 0; a < _groupList.count(); a++){
            catCode.append(QString("-%0").arg(_line.at(_groupList.at(a).toInt() - 1)));
            if (a == _groupList.size() - 1){
                catName.append(QString("%0").arg(_line.at(_groupList.at(a).toInt() - 1)));
            } else {
                catName.append(QString("%0-").arg(_line.at(_groupList.at(a).toInt() - 1)));
            }

        }
        _list << QString("%0|%1").arg(catCode).arg(catName);
    }

    _list.removeDuplicates();


    for (int x = 0; x < _list.size(); x++){
        QMap<QString,QVariant> group;
        group.insert("code", _list.at(x).split("|").at(0));
        group.insert("name", _list.at(x).split("|").at(1));
        group.insert("parent", "");
        mapGroup.insert(x, group);
    }
}

void ctrlFile::readCSVAttribute(QStringList _data, QString _maket)
{
    mapAttribute.clear();

    QSettings *maket = new QSettings(_maket, QSettings::IniFormat);

    QString SPL = maket->value("split").toString();
    QString PRE = maket->value("prefix").toString();
    int FROW = maket->value("firstRow").toInt();


    QString header = _data.at(FROW - 2); //строка данных c заголовками
    QStringList attList = maket->value("attr").toString().split("|");

    for (int a = 0; a < attList.size(); a++){
        QString _name = header.split(SPL).at(attList.at(a).toInt()-1);

        QMap<QString, QVariant> attributes;
        attributes.insert("name", _name);
        mapAttribute.insert(QString("%0_%1").arg(PRE).arg(_name), attributes);
    }
}

void ctrlFile::readCSVProduct(QStringList _data, QString _maket)
{
    mapProduct.clear();

    QSettings *maket = new QSettings(_maket, QSettings::IniFormat);

    QString SPL = maket->value("split").toString();
    QString PRE = maket->value("prefix").toString();
    int FROW = maket->value("firstRow").toInt();


    for (int x = FROW + 1; x >= 0; x--){
        _data.removeAt(x);
    }

    for (int x = 0; x < _data.size(); x++){
        QStringList _line = _data.at(x).split(SPL);
        QMap<QString, QVariant> map;

        QString SKU;
        //category code
        QStringList _groupList = maket->value("cat").toString().split("|");
        QString catCode = PRE;
        for (int a = 0; a < _groupList.count(); a++){
            catCode.append(QString("-%0").arg(_line.at(_groupList.at(a).toInt() - 1)));
        }
        map.insert("groupId", catCode);

        if (maket->value("art_check").toBool()){
            int col = maket->value("art").toInt()-1;
            if (col < _line.size()){
                SKU = QString("%0_%1").arg(PRE).arg(_line.at(col));
                map.insert("articul", SKU);
            }
        }

        if (maket->value("model_check").toBool()){
            int col = maket->value("model").toInt()-1;
            if (col < _line.size()){
                map.insert("model", _line.at(col));
            }
        }

        if (maket->value("name_check").toBool()){
            int col = maket->value("name").toInt()-1;
            if (col < _line.size()){
                map.insert("name", _line.at(col));
            }
        }

        if (maket->value("manuf_check").toBool()){
            int col = maket->value("manuf").toInt()-1;
            if (col < _line.size()){
                map.insert("manufacturerId", _line.at(col));
            }
        } else {
            map.insert("manufacturerId", "no");
        }

        if (maket->value("price_check").toBool()){
            int col = maket->value("price").toInt()-1;
            if (col < _line.size()){
                map.insert("price", _line.at(col));
            }
        }

        if (maket->value("specPrice_check").toBool()){
            int col = maket->value("specPrice").toInt()-1;
            if (col < _line.size()){
                map.insert("specPrice", _line.at(col));
            }
        }

        if (maket->value("quan_check").toBool()){
            int col = maket->value("quan").toInt()-1;
            if (col < _line.size()){
                map.insert("quantity", _line.at(col));
            }
        } else {
            map.insert("quantity", cPref->get_CSV_newQuan());
        }

        if (maket->value("desc_check").toBool()){
            int col = maket->value("desc").toInt()-1;
            if (col < _line.size()){
                map.insert("desc", _line.at(col));
            }
        }
        if (maket->value("image_check").toBool()){
            int col = maket->value("image").toString().split("|").at(0).toInt() - 1;
            if (col < _line.size()){
                map.insert("image", _line.at(col));
            }
        }
        //TODO stock status
        int stockId = cPref->get_CSV_newStockStatusId();
        if (stockId == 7){
            map.insert("stockStatus", "В наличие");
        } else {
            map.insert("stockStatus", "Нет в наличие");
        }
        map.insert("stockStatusId", stockId);

        map.insert("statusId", stockId);

        mapProduct.insert(SKU, map);
    }
}

void ctrlFile::readCSVProductAttribute(QStringList _data, QString _maket)
{
    mapProductAttribute.clear();

    QSettings *maket = new QSettings(_maket, QSettings::IniFormat);

    QString SPL = maket->value("split").toString();
    QString PRE = maket->value("prefix").toString();
    int FROW = maket->value("firstRow").toInt();

    QString header = _data.at(FROW - 2); //строка данных c заголовками

    //удаляем первые строки
    int z = 0;
    for (int x = FROW + 1; x >= 0; x--){
        _data.removeAt(x);
    }

    for (int x = 0; x < _data.size(); x++){
        QStringList _line = _data.at(x).split(SPL);
        QMap<QString, QVariant> map;

        QString SKU;

        if (maket->value("art_check").toBool()){
            int col = maket->value("art").toInt()-1;
            if (col < _line.size()){
                SKU = QString("%0_%1").arg(PRE).arg(_line.at(col));
                map.insert("articul", SKU);
            }
        }
        map.insert("productId", SKU);

        QStringList listA = maket->value("attr").toString().split("|");
        for (int a = 0; a < listA.count(); a++){
            QString _nameAttribute = header.split(SPL).at(listA.at(a).toInt()-1);
            map.insert("attributeId", _nameAttribute);
            map.insert("text", _line.at(listA.at(a).toInt()-1));
            mapProductAttribute.insert(z, map);
            z++;

        }


    }
}

void ctrlFile::readCSVProductImage(QStringList _data, QString _maket)
{
    mapProductImage.clear();

    QSettings *maket = new QSettings(_maket, QSettings::IniFormat);

    QString SPL = maket->value("split").toString();
    QString PRE = maket->value("prefix").toString();
    int FROW = maket->value("firstRow").toInt();

    //удаляем первые строки
    int z = 0;
    for (int x = FROW + 1; x >= 0; x--){
        _data.removeAt(x);
    }

    for (int x = 0; x < _data.size(); x++){
        QStringList _line = _data.at(x).split(SPL); // целая строка данных
        QMap<QString, QVariant> map;

        QString SKU; // идентификатор товара

        if (maket->value("art_check").toBool()){
            int col = maket->value("art").toInt()-1;
            if (col < _line.size()){
                SKU = QString("%0_%1").arg(PRE).arg(_line.at(col));
                map.insert("articul", SKU);
            }
        }
        map.insert("productId", SKU);

        QStringList listA = maket->value("image").toString().split("|");
        if (listA.count() > 1){ // первая запись идет - главная картинка (идет с товаром)
            for (int a = 1; a < listA.count(); a++){
                map.insert("image", _line.at(listA.at(a).toInt()-1));
                mapProductImage.insert(z, map);
                z++;
            }
        }
    }
}

QMap<int, QMap<QString, QVariant> > ctrlFile::getGroups()
{
    return mapGroup;
}

QMap<QString, QMap<QString, QVariant> > ctrlFile::getAttributes()
{
    return mapAttribute;
}

QMap<QString, QMap<QString, QVariant> > ctrlFile::getProducts()
{
    return mapProduct;
}

QMap<int, QMap<QString, QVariant> > ctrlFile::getProductAttributes()
{
    return mapProductAttribute;
}

QMap<int, QMap<QString, QVariant> > ctrlFile::getProductImages()
{
    return mapProductImage;
}

QMap<int, QMap<QString, QVariant> > ctrlFile::getProductOptions()
{
    return mapProductOption;
}

QStringList ctrlFile::getOptionsList()
{
    return listOptions;
}

