#ifndef OCMERIDIAAUTO_H
#define OCMERIDIAAUTO_H

#include <QMainWindow>
#include <QtSql/QtSql>
#include <QtXml/QtXml>
#include <QSettings>
#include <QTimer>
#include <QFileDialog>

namespace Ui {
class ocMeridiaAuto;
}

class ocMeridiaAuto : public QMainWindow
{
    Q_OBJECT

public:
    explicit ocMeridiaAuto(QWidget *parent = 0);
    ~ocMeridiaAuto();

    QString FILE_1C;

    bool _AUTO;
    int _STORE;
    int _LANG;
    int _STOCK;
    int _TAX;
    int _STATUS;
    QString _DIR;

    QSqlDatabase db_server;
    QSqlDatabase db_local;
    QSettings sett;
    QTimer *timer;

    QStringList log;

    QDomDocument *doc;

    QStringList file_category_list; // line = ID||categoryName||ID parent
    QMap<QString, QString> mapProduct; // ArtProduct -- ID||Art||Model||Name||IDGroup||Manufacturer||Price||Quan||Desc||Image
    QMap<QString, int> mapGroup; //codeGroup -- ID_group
    QMap<QString, int> mapAttributes; //nameAttribute -- ID_attribute
    QMap<QString, QString> mapProductAttribute;//ArtProduct -- ID_attribute||Value
    QMap<QString, QString> mapProductImage; //ArtProduct -- Image

    QStringList category_tempList;

    QStringList csvData;


private:
    Ui::ocMeridiaAuto *ui;

public slots:
    void read_setting();

    void connect_db_local();
    void connect_db_server();

    void main_file_get();
    void main_file_read();
    void main_file_upload();

    void pl_file_get();
    void pl_file_read(const QString schema);
    void pl_file_upload();



    int group_get_id(const QString _c, const QString _n, int _v); // v = 1(main file) -- 2(pl)
    int manufacturer_get_id(const QString _n);
    int attribute_get_id(const QString _n, int _v); //v = 1(main file) -- 2(pl)
    int product_get_id(const QString prod_code, const QString prod_name);

    int category_add(QString cat_code, QString cat_name, QString cat_parent);
    void category_makePath(int _ID, int _PAR);

    void product_insert(const QString key);
    void product_update(const QString key, int _v); //v = 1(main file) -- 2(pl)

    //-----------------



    //log
    void writeLog();

    //scenario
    void scenario_A(); //for 1C
    void scenario_B(); //for price_lists
};

#endif // OCMERIDIAAUTO_H
