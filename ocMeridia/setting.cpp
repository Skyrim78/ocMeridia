#include "setting.h"
#include "ui_setting.h"

setting::setting(QSqlDatabase db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    DB = db;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->groupBox_messa, SLOT(hide()));
    ui->groupBox_messa->hide();

    oneC_load();
    connect(ui->toolButton_file1c, SIGNAL(clicked(bool)), this, SLOT(oneC_select()));

    pl_load();
    ui->groupBox_pl_form->hide();
    ui->tableWidget_pl->setColumnHidden(0, true);
    ui->tableWidget_pl->setColumnHidden(2, true);
    connect(ui->toolButton_pl_add, SIGNAL(clicked(bool)), this, SLOT(pl_add()));
    connect(ui->toolButton_pl_del_N, SIGNAL(clicked(bool)), this, SLOT(pl_delN()));
    connect(ui->pushButton_pl_save, SIGNAL(clicked(bool)), this, SLOT(pl_save()));
    connect(ui->pushButton_pl_del, SIGNAL(clicked(bool)), this, SLOT(pl_del()));
    connect(ui->toolButton_pl_sel_name, SIGNAL(clicked(bool)), this, SLOT(pl_selectFile()));
    connect(ui->toolButton_pl_sel_schema, SIGNAL(clicked(bool)), this, SLOT(pl_selectSchema()));
    connect(ui->tableWidget_pl, SIGNAL(clicked(QModelIndex)), ui->groupBox_pl_form, SLOT(hide()));
    connect(ui->tableWidget_pl, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(pl_edit()));
}

setting::~setting()
{
    delete ui;
}

void setting::oneC_load()
{
    ui->lineEdit_file1c->clear();
    QSqlQuery queryMainFile("SELECT files.fname FROM files WHERE files.vid = \'2\' ", DB);
    queryMainFile.next();
    ui->lineEdit_file1c->setText(queryMainFile.value(0).toString());
}

void setting::oneC_select()
{
    QString fname = QFileDialog::getOpenFileName(this, "Select file...", "HOME", "XML 1C (*.cml)");
    if (fname.size() > 0){
        ui->lineEdit_file1c->setText(fname);
        QSqlQuery query(QString("UPDATE files SET fname = \'%1\' WHERE files.vid = \'2\' ").arg(fname), DB);
        query.exec();
    }
}

void setting::pl_load()
{
    for (int row = ui->tableWidget_pl->rowCount() - 1; row >= 0; row--){
        ui->tableWidget_pl->removeRow(row);
    }
    QSqlQuery queryPriceFile("SELECT files.id, files.fname, files.active FROM files WHERE files.vid = \'0\' ", DB);
    int row = 0;
    while (queryPriceFile.next()){
        ui->tableWidget_pl->insertRow(row);
        for (int col = 0; col < 3; col++){
            QColor colBG;
            QColor colFG;
            if (queryPriceFile.value(2).toInt() == 1) {
                colBG.setNamedColor("#FAFAFA");
                colFG.setNamedColor("#006400");
            } else {
                colBG.setNamedColor("#FAFAFA");
                colFG.setNamedColor("#CD0000");
            }
            QTableWidgetItem *item = new QTableWidgetItem(queryPriceFile.value(col).toString());
            item->setForeground(colFG);
            item->setBackground(colBG);
            ui->tableWidget_pl->setItem(row, col, item);
        }
        row++;
    }
}

void setting::pl_add()
{
    _ID = 0;
    ui->groupBox_pl_form->setVisible(true);
    ui->lineEdit_pl_name->clear();
    ui->lineEdit_pl_schema->clear();
    ui->checkBox_pl_active->setChecked(false);
    ui->pushButton_pl_del->setEnabled(false);
}

void setting::pl_edit()
{
    _ID = ui->tableWidget_pl->item(ui->tableWidget_pl->currentRow(), 0)->text().toInt();
    QSqlQuery query(QString("SELECT files.fname, files.schema, files.active "
                            "FROM files "
                            "WHERE files.vid = \'0\' AND files.id = \'%1\' ").arg(_ID), DB);
    query.next();
    ui->lineEdit_pl_name->setText(query.value(0).toString());
    ui->lineEdit_pl_schema->setText(query.value(1).toString());
    ui->checkBox_pl_active->setChecked(query.value(2).toBool());

    ui->pushButton_pl_del->setEnabled(true);

    ui->groupBox_pl_form->setVisible(true);
}

void setting::pl_save()
{
    QString error;
    if (ui->lineEdit_pl_name->text().isEmpty()){
        error.append("Выберите файл");
    } else {
        if (ui->lineEdit_pl_schema->text().isEmpty()){
            error.append("Выберите схему для разбора файла");
        } else {
            if (_ID == 0){
                QSqlQuery query("INSERT INTO files (fname, schema, vid, active) VALUES (?, ?, ?, ?)", DB);
                query.bindValue(0, ui->lineEdit_pl_name->text());
                query.bindValue(1, ui->lineEdit_pl_schema->text());
                query.bindValue(2, 0);
                query.bindValue(3, ui->checkBox_pl_active->isChecked());
                query.exec();
                if (query.lastError().text().size() > 3){
                    error.append(query.lastError().text());
                }
            } else if (_ID > 0){
                QSqlQuery query(QString("UPDATE files SET fname = \'%1\', schema = \'%2\', active = \'%3\' "
                                        "WHERE files.id = \'%4\' ")
                                .arg(ui->lineEdit_pl_name->text())
                                .arg(ui->lineEdit_pl_schema->text())
                                .arg(ui->checkBox_pl_active->isChecked())
                                .arg(_ID), DB);
                query.exec();
                if (query.lastError().text().size() > 3){
                    error.append(query.lastError().text());
                }
            }
        }
    }
    if (error.size() > 0){
        makeMessage(error, false);
    } else {
        makeMessage("Сохранено", true);
        pl_load();
        if (_ID == 0){
            pl_add();
        }
    }
}

void setting::pl_del()
{
    QSqlQuery query(QString("DELETE FROM files WHERE files.id = \'%1\' ").arg(_ID), DB);
    query.exec();
    if (query.lastError().text().size() > 3){
        makeMessage(query.lastError().text(), false);
    } else {
        makeMessage("Удалено", true);
        ui->groupBox_pl_form->hide();
        pl_load();
    }
}

void setting::pl_delN()
{
    QString error;
    if (ui->tableWidget_pl->rowCount() > 0){
        QList<int> del_list;
        for (int row = 0; row < ui->tableWidget_pl->rowCount(); row++){
            if (ui->tableWidget_pl->item(row, 0)->isSelected()){
                del_list.append(ui->tableWidget_pl->item(row, 0)->text().toInt());
            }
        }
        if (del_list.size() > 0){
            for (int x = del_list.size() - 1; x >= 0; x--){
                QSqlQuery query(QString("DELETE FROM files WHERE files.id = \'%1\' ").arg(del_list.at(x)), DB);
                query.exec();
                if (query.lastError().text().size() > 3){
                    error.append(query.lastError().text());
                }
            }
        }
    } else {
        error.append("Нечего удалять");
    }
    if (error.size() > 0){
        makeMessage(error, false);
    } else {
        makeMessage("Удалено", true);
        pl_load();
    }
}

void setting::pl_selectFile()
{
    ui->lineEdit_pl_name->setText(QFileDialog::getOpenFileName(this, "Select file...", "HOME", "XML (*.cml), CSV (*.csv)"));
}

void setting::pl_selectSchema()
{
    ui->lineEdit_pl_schema->setText(QFileDialog::getOpenFileName(this, "Select file...", "HOME", "Meridia Schema (*.ini)"));
}

void setting::makeMessage(const QString str, bool x)
{
    ui->groupBox_messa->setVisible(true);
    if (x){
        ui->groupBox_messa->setStyleSheet("background-color: #228B22; border-radius: 9px;");
    } else {
        ui->groupBox_messa->setStyleSheet("background-color: #8B0000; border-radius: 9px;");
    }
    ui->l_message->setText(str);
    ui->l_message->setStyleSheet("color: #FFF5EE; font-weight: bold; ");

    timer->start(5000);
}


