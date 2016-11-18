#include "mcategory.h"

mCategory::mCategory()
{

}

void mCategory::category_openXML()
{
    QString _fname = QDir::currentPath().append("/Data/products_character.cml");
    qDebug() << "address: " << _fname;

    QFile file(_fname);
    if (file.open(QIODevice::ReadOnly)){
        doc = new QDomDocument();
        doc->setContent(&file);
        //qDebug() << category_readXML();
        product_readXML();
    } else {
        qDebug() << "error reading!";
        qDebug() << "address: " << _fname;
    }
    file.close();
}

QStringList mCategory::category_readXML()
{
    QString _fname = QDir::currentPath().append("/Data/products_character.cml");
    qDebug() << "address: " << _fname;

    QStringList result;

    QFile file(_fname);
    if (file.open(QIODevice::ReadOnly)){
        doc = new QDomDocument();
        doc->setContent(&file);

        QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
        QDomNode nodeCl = nodeCom.childNodes().at(0); //классификатор

        QDomNode nodeGroups = nodeCl.childNodes().at(2);

        for (int x = 0; x < nodeGroups.childNodes().count(); x++){
            QDomNode nodeGroup = nodeGroups.childNodes().at(x);

            QString _ID = nodeGroup.firstChildElement("Ид").text();
            QString _NAME = nodeGroup.firstChildElement("Наименование").text();
            QString _PARENT = nodeGroup.firstChildElement("Родитель").text();

            result.append(QString("%1||%2||%3").arg(_ID).arg(_NAME).arg(_PARENT));

        }
    } else {
        qDebug() << "error reading!";
        qDebug() << "address: " << _fname;
    }
    file.close();

    return result;
}

void mCategory::product_readXML()
{
   QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
    qDebug() << nodeCom.nodeName();
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

    //qDebug() << idxProd;

    QDomNode nodePredAll = nodeAll.childNodes().at(idxPred);

    QString line;
    QMap<QString, QString> mapPrice; //ИД товаров + Цена

    // собираем ID товаров и цены из предложений
    for (int x = 0; x < nodePredAll.childNodes().count(); x++){


        QDomNode nodePred = nodePredAll.childNodes().at(x);

        QString _ID = nodePred.firstChildElement("Ид").text();
        QString _Price = nodePred.firstChildElement("Цены").firstChildElement("Цена").firstChildElement("ЦенаЗаЕдиницу").text();

        mapPrice.insert(_ID, _Price);
    }
    //qDebug() << mapPrice;

    //формируем карту товаров
    QMap<QString, QString> mapProduct;
    QDomNode nodeProducts = nodeAll.childNodes().at(idxProd);
    QString _line;
    for (int y = 0; y < 3000; y++){            //nodeProducts.childNodes().count(); y++){
        QDomNode nodeProduct = nodeProducts.childNodes().at(y);
        QString _ID = nodeProduct.firstChildElement("Ид").text();
        if (_ID.size() == 36){
            line.clear();
            line.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
            line.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
            line.append(QString("||%0").arg(mapPrice.value(_ID)));//Цена
            line.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
            line.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
            QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
            for (int x = 0; x < elAttributes.childNodes().count(); x++){
                QDomNode nAtt = elAttributes.childNodes().at(x);
                if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                    line.append(QString("||%0").arg(nAtt.firstChildElement("Значение").text()));//Описание
                }
            }
            qDebug() << line;
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
                    line_post.append(QString("||%0").arg(mapPrice.value(_ID)));//Цена
                    line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                    line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                    QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                    for (int x = 0; x < elAttributes.childNodes().count(); x++){
                        QDomNode nAtt = elAttributes.childNodes().at(x);
                        if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                            line_post.append(QString("||%0").arg(nAtt.firstChildElement("Значение").text()));
                        }
                    }
                    mapProduct.insert(_ID, line_post);
                } else {
                    QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                    for (int x = 0; x < elAttributes.childNodes().count(); x++){
                        QDomNode nAtt = elAttributes.childNodes().at(x);
                        if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                            line_post.append(QString("||%0").arg(nAtt.firstChildElement("Значение").text()));
                        }
                    }
                    mapProduct.remove(_ID);
                    mapProduct.insert(_ID, line_post);
                }
            }
        }
    }
    qDebug() << mapProduct;
}
