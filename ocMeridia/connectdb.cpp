#include "connectdb.h"

connectDB::connectDB()
{
    connectOC();
}

void connectDB::connectOC()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("rm");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("");
    db.open();

    qDebug() << db.isOpen();

}
