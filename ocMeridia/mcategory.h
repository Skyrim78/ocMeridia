#ifndef MCATEGORY_H
#define MCATEGORY_H

#include <QtXml/QtXml>

class mCategory
{


public:
    mCategory();

    QDomDocument *doc;

public slots:
    void category_openXML();
    QStringList category_readXML();
    void product_readXML();
};

#endif // MCATEGORY_H
