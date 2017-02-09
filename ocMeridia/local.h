#ifndef LOCAL_H
#define LOCAL_H

#include <QDialog>
#include <QtSql/QtSql>

namespace Ui {
class local;
}

class local : public QDialog
{
    Q_OBJECT

public:
    explicit local(QSqlDatabase db_local, QSqlDatabase db_server, QWidget *parent = 0);
    ~local();

private:
    Ui::local *ui;

    QSqlDatabase DB_LOCAL;
    QSqlDatabase DB_SERVER;

<<<<<<< HEAD

public slots:
    void load_category();
    void clear_category();

    void load_attribute();
    void clear_attribute();

    void total_clear();
=======
public slots:
    void category_load();
    void category_reconnect();

    void attribute_load();
    void attribute_reconnect();

    void total_reconnect();
>>>>>>> 3c9ad353c05f73199b5c0d19ebfcdb275c143364
};

#endif // LOCAL_H
