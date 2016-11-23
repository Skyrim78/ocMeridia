#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include <QtSql/QtSql>
#include <QSettings>
#include <QTimer>
#include <QFileDialog>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QSqlDatabase db, QWidget *parent = 0);
    ~setting();

private:
    Ui::setting *ui;

    QTimer *timer;

    QSqlDatabase DB;
    int _ID;

public slots:
    void oneC_load();
    void oneC_select();

    void pl_load();
    void pl_add();
    void pl_edit();
    void pl_save();
    void pl_del();
    void pl_delN();
    void pl_selectFile();
    void pl_selectSchema();


    void makeMessage(const QString str, bool x);
};

#endif // SETTING_H
