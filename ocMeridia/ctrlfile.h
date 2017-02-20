#ifndef CTRLFILE_H
#define CTRLFILE_H

#include <QtXml>
#include <QMap>
#include <QSettings>
//--
#include "ctrlpreference.h"

class ctrlFile
{
public:
    ctrlFile();

    preference *cPref;

    QMap<int, QMap<QString, QVariant> > mapGroup;
    //ИД_группы : ИД_группы, Наименование, Родитель

    QMap<QString, QMap<QString, QVariant> > mapAttribute;
    //ИД_аттрибута : ИД_аттрибута, Наименование

    QMap<QString, QMap<QString, QVariant> > mapProduct;
    //ИД_товара :  ИД(артикул), Модель, Наименование, ИД Группы, Производитель,
    //Количество, Описание, Картинка, Цена, Наличие

    QMap<int, QMap<QString, QVariant> > mapProductAttribute;
    //ИД_товара : ИД_аттрибута, Значение

    QMap<int, QMap<QString, QVariant> > mapProductImage;
    //ИД_товара : ИД_аттрибута, Значение

    QMap<int, QMap<QString, QVariant> > mapProductOption;

    QStringList listOptions;
    //ИД_товара : Значение, Цена

public slots:
    void readXML(QString _fname);
    void readXMLGroups(QDomDocument *doc);
    void readXMLAttributes(QDomDocument *doc);


    void readCSV(QStringList _data, QString _maket);
    void readCSVGroups(QStringList _data, QString _maket);
    void readCSVAttribute(QStringList _data, QString _maket);
    void readCSVProduct(QStringList _data, QString _maket);
    void readCSVProductAttribute(QStringList _data, QString _maket);
    void readCSVProductImage(QStringList _data, QString _maket);


    QMap<int, QMap<QString, QVariant> > getGroups();
    QMap<QString, QMap<QString, QVariant> > getAttributes();
    QMap<QString, QMap<QString, QVariant> > getProducts();
    QMap<int, QMap<QString, QVariant> > getProductAttributes();
    QMap<int, QMap<QString, QVariant> > getProductImages();
    QMap<int, QMap<QString, QVariant> > getProductOptions();
    QStringList getOptionsList();


};



#endif // CTRLFILE_H
