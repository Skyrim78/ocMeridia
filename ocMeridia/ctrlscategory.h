#ifndef CTRLSCATEGORY_H
#define CTRLSCATEGORY_H

#include <QMap>
#include <QtSql>


class ctrlSCategory
{
public:
    ctrlSCategory(QSqlDatabase _db, QString _pre);

    QMap<QString,QVariant> CAT;
    QSqlDatabase DB;
    QString PREFIX;
public slots:
    void set_current(int ID,);
    int get_parentId();
    int get_top();
    int get_column();
    int get_status();
    QDateTime get_dateModified();
    QString get_code();
    QMap<int, QMap<QString, QVariant> > get_descriptions(int prodID);
    QMap<QString,QVariant> save(QMap<QString,QVariant> saveMap);
};

#endif // CTRLSCATEGORY_H
