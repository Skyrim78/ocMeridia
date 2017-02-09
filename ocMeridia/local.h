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


public slots:
    void load_category();
    void clear_category();

    void load_attribute();
    void clear_attribute();

    void total_clear();
};

#endif // LOCAL_H
