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
    int _STORE;
    int _LANG;
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
    QMap<QString, QString> mapAttribute;//ArtProduct -- ID_attribute||Value

    QStringList category_tempList;
    int IDcolor;


private:
    Ui::ocMeridiaAuto *ui;

public slots:
    void read_setting();
    void connect_db_local();
    void connect_db_server();

    void main_file_get();
    void main_file_read();

    int group_get_id(const QString _c, const QString _n);
    int manufacturer_get_id(const QString _n);
    int attribute_get_id(const QString _n);


    void category_test();
    void category_productChange(QString prevID, QString realID);
    void category_productDelete(QString prevID);
    int category_add(QString cat_code, QString cat_name, QString cat_parent);
    void category_makePath(int _ID, int _PAR);

    //products
    void product_load();
    int product_get_id(const QString prod_code, const QString prod_name);
    void product_insert(const QString pcode, const QString pname, const QString pgroup, const QString pimage, const QString pdesc, const QString pattr, double pprice);

    //log
    void writeLog();

    //scenario
    void scenario_A();
};

#endif // OCMERIDIAAUTO_H
