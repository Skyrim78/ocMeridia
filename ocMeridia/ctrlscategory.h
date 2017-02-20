#ifndef CTRLSCATEGORY_H
#define CTRLSCATEGORY_H

#include <QMap>
#include <QtSql>


class ctrlSCategory
{
public:
    ctrlSCategory(QSqlDatabase _db, QString _pre, int _st);

    QMap<QString,QVariant> CAT;
    QSqlDatabase DB;
    QString PREFIX;
    int STORE;

    QStringList tempList;

public slots:
    void set_current(int ID, int LANG);
    int get_parentId();
    int get_top();
    int get_column();
    int get_sortOrder();
    int get_status();
    QDateTime get_dateModified();
    QString get_code();
    QString get_name();
    QString get_description();

    QString get_parentName(int ID, int LANG);
    QString get_road(int ID, int LANG);
    int getIdByCode(QString _code);

    QMap<int, QMap<QString, QVariant> > get_categories();
    QMap<int, QMap<QString, QVariant> > get_mainCategories();
    QMap<int, QMap<QString, QVariant> > get_descriptions(int ID);
    QMap<int, QMap<QString, QVariant> > get_descriptionsLang(int ID, int LANG);


    QMap<QString,QVariant> saveMain(QMap<QString,QVariant> saveMap, int LANG);
    void makeCategoryPath();
    void makeCategoryPathLevel(int _ID, int _PAR);
    void saveCategoryToStore(int ID);

    
};

#endif // CTRLSCATEGORY_H
