#ifndef OCMERIDIA_H
#define OCMERIDIA_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#include <QSettings>
#include <QTimer>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include "mcategory.h"
#include "product_form.h"

namespace Ui {
class ocMeridia;
}

class ocMeridia : public QMainWindow
{
    Q_OBJECT

public:
    explicit ocMeridia(QWidget *parent = 0);
    ~ocMeridia();

    QStringList cat_list;

    QSqlDatabase db_server;
    QSqlDatabase db_local;


    //******************
    QTimer *timer;
    QDomDocument *doc;

    //-setting
    QSettings sett;
    int _STORE;
    int _LANG;
    QMap<int,int> langMap;
    int _TAX;
    QMap<int,int> taxMap;
    int _STOCK;
    QMap<int,int> stockMap;
    int _STATUS;
    QMap<int,int> statusMap;
    QString _DIR;
    QMap<int,int> categoryMap;

    //-category
    int cat_activeID;
    QMap<QString,int> cat_mainMap; // код1С + category_id
    QStringList category_tempList;

    //-attributes
    QStringList attributes_list;

private:
    Ui::ocMeridia *ui;

public slots:
    void readSetting();
    void writeSetting();
    void loadMaps();

    void setting_connect_netDB();
    void setting_connect_locDB();
    void setting_testConnect();
    void setting_changeLangStore(int idx);
    void setting_selectDir1C();
    void setting_selectDirPL();

    void file_Open();

    void category_loadFromDB();
    void category_loadFromDB_parent(QTreeWidgetItem *par, QString id_parent);
    void category_selectServer();
    void category_checkState();

    void category_testServer();
    void category_save();
    void category_makePath(int _ID, int _PAR);

    void product_testServer();
    void product_openForm();
    void product_loadOnServer();



    void makeMessage(const QString str, bool x);

};

#endif // OCMERIDIA_H
