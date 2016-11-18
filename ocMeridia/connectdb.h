#ifndef CONNECTDB_H
#define CONNECTDB_H

#include <QtSql/QtSql>
#include <QDebug>

class connectDB
{
public:
    connectDB();

    QSqlDatabase db;

    QStringList category;
    QStringList category_desc;

public slots:
    void connectOC();

};

#endif // CONNECTDB_H
