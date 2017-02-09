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
#include "setting.h"
#include "local.h"

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
    double _QUAN;
    QMap<int,int> attributeMap;

    //-category
    int cat_activeID;
    QMap<QString,int> cat_mainMap; // код1С + category_id
    QStringList category_tempList;

    //-attributes
    QStringList attributes_list;
    QMap<QString, QString> attributeProductMap;
    int att_activeID;
    //manufacturer
    bool _test_manufacturer;

    //makets
    QSettings maket;

    //csv
    QStringList csvData;

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
    void setting_loadListFiles();
    void setting_selectDirPL();

    void file_Select();
    void file_Open_PL();
    void file_Open_Main();
    void file_loadMaket();
    void file_saveMaket();

    void csv_readData();


    void category_loadFromDB();
    void category_loadFromDB_parent(QTreeWidgetItem *par, QString id_parent);
    void category_selectServer();
    void category_checkState();

    void category_testServer();
    void category_save();
    void category_makePath(int _ID, int _PAR);

    void attribute_testServer();
    void attribute_loadFromDB();
    void attribute_checkState();
    void attribute_selectServer();
    void attribute_save();

    //void all_test();

    void product_testServer();
    void product_openForm();
    void product_loadOnServer();
    void product_manufacturer();

    //подмена категорий в новых товарах на установленную в настройках
    void product_change_group();

    //связи
    void edit_connection();

    void makeMessage(const QString str, bool x);

};

#endif // OCMERIDIA_H
