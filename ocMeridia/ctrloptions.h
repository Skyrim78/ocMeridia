#ifndef CTRLOPTIONS_H
#define CTRLOPTIONS_H

#include <QtSql>
#include <QMap>

class ctrlOptions
{
public:
    ctrlOptions(QSqlDatabase _db, QString _pre);

    QSqlDatabase DB;
    QString PREFIX;


public slots:
    void remakeOptions(QStringList listOptions, int LANG);
    int get_optionID(QString _name);
    void add_option(QString _name, int LANG);

    void remakeOptionValue(QMap<int, QMap<QString,QVariant> >, int LANG);
    int get_optionValueID(int _optionId, QString _name);
    void add_optionValue(int _optionId, QString _name, int LANG);
};

#endif // CTRLOPTIONS_H
