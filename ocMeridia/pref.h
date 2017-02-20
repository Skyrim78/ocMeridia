#ifndef PREF_H
#define PREF_H

#include <QDialog>
#include <QtSql>
#include <QTimer>
//--
#include "setting.h"
//--
#include "ctrlpreference.h"
#include "ctrlscategory.h"

namespace Ui {
class Pref;
}

class Pref : public QDialog
{
    Q_OBJECT

public:
    explicit Pref(QSqlDatabase db_server, QSqlDatabase db_local, QWidget *parent = 0);
    ~Pref();

    QSqlDatabase sDB;
    QSqlDatabase lDB;
    QSqlDatabase tDB;

    preference *cPref;
    ctrlSCategory *cCategory;

    int LANG;

    QMap<int,int> langMap;
    QMap<int,int> taxMap;
    QMap<int,int> groupsMap;
    QMap<int,int> attributeMap;
    QMap<int,int> stockMap;

    QTimer *timer;

private:
    Ui::Pref *ui;

public slots:
    void loadLang();
    void loadTax();
    void loadGroup();
    void loadAttribute();
    void loadStock();

    void load();
    void save();

    void loadAutoFile();
    void changeFile();

    void test_connection();
    void message(QString str, bool x);
};

#endif // PREF_H
