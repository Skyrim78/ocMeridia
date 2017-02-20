#ifndef CTRLMANUFACTURER_H
#define CTRLMANUFACTURER_H

#include <QMap>
#include <QtSql>

class ctrlManufacturer
{
public:
    ctrlManufacturer(QSqlDatabase _db, QString _pre, int _st);

    QSqlDatabase DB;
    QString PREFIX;
    int STORE;

public slots:
    int getID_byName(QString _name);

    QMap<QString,QVariant> save(QMap<QString,QVariant> saveMap, int LANG);
};

#endif // CTRLMANUFACTURER_H
