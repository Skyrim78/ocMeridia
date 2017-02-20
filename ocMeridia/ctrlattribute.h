#ifndef CTRLATTRIBUTE_H
#define CTRLATTRIBUTE_H

#include <QMap>
#include <QtSql>

class ctrlAttribute
{
public:
    ctrlAttribute(QSqlDatabase _db, QString _pre, int _st);

    QSqlDatabase DB;
    QString PREFIX;
    int STORE;

public slots:
    int get_attributeID(QString _name);
};

#endif // CTRLATTRIBUTE_H
