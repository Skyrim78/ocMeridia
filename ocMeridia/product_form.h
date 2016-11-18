#ifndef PRODUCT_FORM_H
#define PRODUCT_FORM_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QTimer>

namespace Ui {
class product_form;
}

class product_form : public QDialog
{
    Q_OBJECT

public:
    explicit product_form(int l, int s, QSqlDatabase db, QMap<QString,QString> ll, QWidget *parent = 0);
    ~product_form();

    QMap<QString,QString> loadProductsMap;
    QSqlDatabase DB;
    int _LANG;
    int _STORE;

    QTimer *timer;

    QMap<int, int> groupsMap;
    int prodID;

private:
    Ui::product_form *ui;

public slots:
    void loadGroups();
    void loadFileProducts();
    void loadOCProducts();
    void selectProduct();
    void unselectProduct();
    void checkSelectionMode();
    void save();

    void makeMessage(const QString str, bool x);
};

#endif // PRODUCT_FORM_H
