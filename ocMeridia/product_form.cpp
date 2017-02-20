#include "product_form.h"
#include "ui_product_form.h"

product_form::product_form(int l, int s, QSqlDatabase db, QMap<QString,QString> ll, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::product_form)
{
    ui->setupUi(this);
    loadProductsMap = ll;
    DB = db;
    _LANG = l;
    _STORE = s;

    cCategory = new ctrlSCategory(db, "rmrt", _STORE);

    ui->groupBox_messa->hide();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->groupBox_messa, SLOT(hide()));

    loadGroups();
    loadFileProducts();
    loadOCProducts();

    connect(ui->checkBox_s_filter, SIGNAL(clicked(bool)), this, SLOT(loadOCProducts()));
    connect(ui->checkBox_s_filter, SIGNAL(clicked(bool)), ui->comboBox_s_group, SLOT(setEnabled(bool)));
    connect(ui->comboBox_s_group, SIGNAL(currentIndexChanged(int)), this, SLOT(loadOCProducts()));
    connect(ui->radioButton_s_all, SIGNAL(clicked(bool)), this, SLOT(loadOCProducts()));
    connect(ui->radioButton_s_cod, SIGNAL(clicked(bool)), this, SLOT(loadOCProducts()));

    connect(ui->tableWidget_s_product, SIGNAL(clicked(QModelIndex)), this, SLOT(selectProduct()));

    connect(ui->radioButton_add, SIGNAL(toggled(bool)), ui->comboBox_form_group, SLOT(setEnabled(bool)));
    connect(ui->radioButton_equi, SIGNAL(toggled(bool)), ui->groupBox_shop, SLOT(setEnabled(bool)));
    connect(ui->radioButton_equi, SIGNAL(toggled(bool)), ui->lineEdit_name, SLOT(setEnabled(bool)));
    connect(ui->radioButton_equi, SIGNAL(toggled(bool)), ui->groupBox_info, SLOT(setEnabled(bool)));
    connect(ui->radioButton_equi, SIGNAL(toggled(bool)), ui->toolButton_clear, SLOT(setEnabled(bool)));
    connect(ui->toolButton_clear, SIGNAL(clicked(bool)), this, SLOT(unselectProduct()));


    connect(ui->radioButton_add, SIGNAL(clicked(bool)), this, SLOT(checkSelectionMode()));
    connect(ui->radioButton_equi, SIGNAL(clicked(bool)), this, SLOT(checkSelectionMode()));

    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(save()));

    //start mode
    ui->radioButton_add->setChecked(true);
    ui->tableWidget_f_product->setSelectionMode(QTableWidget::ExtendedSelection);
  //  ui->tableWidget_s_product->setColumnHidden(0, true);
    ui->groupBox_shop->setEnabled(false);
    ui->groupBox_info->setEnabled(false);
    ui->lineEdit_name->setEnabled(false);
    ui->toolButton_clear->setEnabled(false);

}

product_form::~product_form()
{
    delete ui;
}

void product_form::loadGroups()
{
    groupsMap.clear();
    ui->comboBox_form_group->clear();
    ui->comboBox_s_group->clear();

    QMap<int, QMap<QString,QVariant> > MAP = cCategory->get_categories();
    for (int x = 0; x < MAP.count(); x++){
        QMap<QString,QVariant> _map = MAP.value(x);

        int id = _map.value("categoryId").toInt();
        QString name = cCategory->get_road(id, _LANG);

        ui->comboBox_form_group->addItem(name);
        ui->comboBox_s_group->addItem(name);
        groupsMap.insert(x, id);
    }
}

void product_form::loadFileProducts()
{
    for (int r = ui->tableWidget_f_product->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_f_product->removeRow(r);
    }
    ui->progressBar->setValue(0);
    for (int row = 0; row < loadProductsMap.count(); row++){
        ui->tableWidget_f_product->insertRow(row);
        QString key = loadProductsMap.keys().at(row);

        QTableWidgetItem *itemID = new QTableWidgetItem(key);
        ui->tableWidget_f_product->setItem(row, 0, itemID);

        QTableWidgetItem *itemName = new QTableWidgetItem(loadProductsMap.value(key));
        ui->tableWidget_f_product->setItem(row, 1, itemName);

        ui->progressBar->setValue(qFloor((row + 1) * 100 / loadProductsMap.count()));
        QApplication::processEvents();
    }
    ui->tableWidget_f_product->resizeColumnsToContents();
    ui->tableWidget_f_product->horizontalHeader()->setStretchLastSection(true);
    ui->progressBar->setValue(0);
}

void product_form::loadOCProducts()
{
    for (int r = ui->tableWidget_s_product->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_s_product->removeRow(r);
    }
    QString str(QString("SELECT rmrt_product.product_id, "
                "rmrt_category_description.`name`, "
                "rmrt_product_description.`name`,"
                "rmrt_product.sku "
                "FROM rmrt_product "
                "INNER JOIN rmrt_product_description "
                "ON rmrt_product_description.product_id = rmrt_product.product_id "
                "INNER JOIN rmrt_product_to_category "
                "ON rmrt_product_to_category.product_id = rmrt_product.product_id "
                "INNER JOIN rmrt_category_description "
                "ON rmrt_category_description.category_id = rmrt_product_to_category.category_id "
                "WHERE (rmrt_product_description.language_id = \'%1\') ").arg(_LANG));
    if (ui->radioButton_s_cod->isChecked()){
        str.append("AND (rmrt_product.sku = \"\") ");
    }
    if (ui->checkBox_s_filter->isChecked()){
        str.append(QString("AND (rmrt_product_to_category.category_id = \'%1\') ")
                   .arg(groupsMap.value(ui->comboBox_s_group->currentIndex())));
    }
    str.append("ORDER BY rmrt_category_description.`name`, rmrt_product_description.`name`");

    QSqlQuery queryP(str, DB);
    int row = 0;
    while (queryP.next()){
        ui->tableWidget_s_product->insertRow(row);
        for (int col = 0; col < 3; col++){
            QString sku = queryP.value(3).toString();
            QTableWidgetItem *item = new QTableWidgetItem(queryP.value(col).toString());
            if (sku.size() == 36){
                item->setForeground(Qt::red);
            }
            ui->tableWidget_s_product->setItem(row, col, item);
        }
        row++;
    }
    ui->tableWidget_s_product->resizeColumnsToContents();
    ui->tableWidget_s_product->horizontalHeader()->setStretchLastSection(true);
}

void product_form::selectProduct()
{
    int crow = ui->tableWidget_s_product->currentRow();
    prodID = ui->tableWidget_s_product->item(crow, 0)->text().toInt();

    QSqlQuery queryInfo(QString("SELECT rmrt_product.sku, rmrt_product.price, rmrt_product.quantity, "
                                "rmrt_product.image, rmrt_category_description.`name`, "
                                "rmrt_product_description.`name` "
                                "FROM rmrt_product "
                                "INNER JOIN rmrt_product_description "
                                "ON rmrt_product_description.product_id = rmrt_product.product_id "
                                "INNER JOIN rmrt_product_to_category "
                                "ON rmrt_product_to_category.product_id = rmrt_product.product_id "
                                "INNER JOIN rmrt_category_description "
                                "ON rmrt_category_description.category_id = rmrt_product_to_category.category_id "
                                "WHERE rmrt_product.product_id = \'%1\' "
                                "AND rmrt_category_description.language_id = \'%2\' "
                                "AND rmrt_product_description.language_id = \'%2\' ")
                        .arg(prodID)
                        .arg(_LANG), DB);
    queryInfo.next();
    QString infostr(QString("<p>Наименование: <b>%0</b><br />").arg(queryInfo.value(5).toString()));
    if (queryInfo.value(0).toString().size() < 36){
        infostr.append(QString("Код 1С: <b>%0</b><br />").arg(queryInfo.value(0).toString()));
    } else {
        infostr.append(QString("Код 1С: <span style=\" color:#ff00ff;\"><b>%0</b></span><br />").arg(queryInfo.value(0).toString()));
    }
    infostr.append(QString("Группа: <b>%0</b><br />").arg(queryInfo.value(4).toString()));
    infostr.append(QString("Цена: <b>%0</b><br />").arg(queryInfo.value(1).toString()));
    infostr.append(QString("Количество: <b>%0</b><br />").arg(queryInfo.value(2).toString()));
    infostr.append(QString("Картинка: <b>%0</b><br />").arg(queryInfo.value(3).toString()));
    ui->textEdit_info->setHtml(infostr);

    ui->lineEdit_name->setText(queryInfo.value(5).toString());
}

void product_form::unselectProduct()
{
    prodID = 0;
    ui->lineEdit_name->clear();
    ui->textEdit_info->clear();
}

void product_form::checkSelectionMode()
{
    if (ui->radioButton_add->isChecked()){
        ui->tableWidget_f_product->setSelectionMode(QTableWidget::ExtendedSelection);
    } else if (ui->radioButton_equi->isChecked()){
        ui->tableWidget_f_product->setSelectionMode(QTableWidget::SingleSelection);
    }
}

void product_form::save()
{
    QString error;
    QList<int> del_list;
    ui->progressBar->setValue(0);
    if (ui->radioButton_add->isChecked()){
        for (int row = 0; row < ui->tableWidget_f_product->rowCount(); row++){
            if (ui->tableWidget_f_product->item(row, 0)->isSelected()){
                QSqlQuery queryAddProduct("INSERT INTO rmrt_product (sku, quantity, manufacturer_id, stock_status_id, "
                                          "tax_class_id, status) "
                                          "VALUES (?, ?, ?, ?, ?, ?)", DB);
                queryAddProduct.bindValue(0, ui->tableWidget_f_product->item(row, 0)->text());
                queryAddProduct.bindValue(1, 20);
                queryAddProduct.bindValue(2, 0); // производителя нет
                queryAddProduct.bindValue(3, 7); //stock status  = 7 (в наличие)
                queryAddProduct.bindValue(4, 9); // tax_class = 9 (облагаемый налогом)
                queryAddProduct.bindValue(5, 1); // status = 1 (включено)
                queryAddProduct.exec();
                if (queryAddProduct.lastError().text().size() > 3){
                  error.append(queryAddProduct.lastError().text());
                  break;
                } else {
                    del_list.append(row);
                    int _ID = queryAddProduct.lastInsertId().toInt();
                    //добавляем description
                    QSqlQuery queryAddProdDesc("INSERT INTO rmrt_product_description (product_id, language_id, name) "
                                               "VALUES (?, ?, ?)", DB);
                    queryAddProdDesc.bindValue(0, _ID);
                    queryAddProdDesc.bindValue(1, _LANG);
                    queryAddProdDesc.bindValue(2, ui->tableWidget_f_product->item(row, 1)->text());
                    queryAddProdDesc.exec();
                    if (queryAddProdDesc.lastError().text().size() > 3){
                      error.append(queryAddProdDesc.lastError().text());
                    }

                    //добавляем store
                    QSqlQuery queryAddProdStore("INSERT INTO rmrt_product_to_store (product_id, store_id) "
                                               "VALUES (?, ?)", DB);
                    queryAddProdStore.bindValue(0, _ID);
                    queryAddProdStore.bindValue(1, _STORE);
                    queryAddProdStore.exec();
                    if (queryAddProdStore.lastError().text().size() > 3){
                      error.append(queryAddProdStore.lastError().text());
                    }
                    //добавляем category
                    QSqlQuery queryAddProdCat("INSERT INTO rmrt_product_to_category (product_id, category_id, main_category) "
                                               "VALUES (?, ?, ?)", DB);
                    queryAddProdCat.bindValue(0, _ID);
                    queryAddProdCat.bindValue(1, groupsMap.value(ui->comboBox_form_group->currentIndex()));
                    queryAddProdCat.bindValue(2, 1);
                    queryAddProdCat.exec();
                    if (queryAddProdCat.lastError().text().size() > 3){
                      error.append(queryAddProdCat.lastError().text());
                    }
                }
            }

            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_f_product->rowCount()));
            QApplication::processEvents();
        }
    } else {
        for (int row = 0; row < ui->tableWidget_f_product->rowCount(); row++){
            if (ui->tableWidget_f_product->item(row, 0)->isSelected()){
                QSqlQuery queryUpProd(QString("UPDATE rmrt_product "
                                              "SET sku = \'%0\' "
                                              "WHERE rmrt_product.product_id = \'%1\'")
                                      .arg(ui->tableWidget_f_product->item(row, 0)->text())
                                      .arg(prodID), DB);
                queryUpProd.exec();
                if (queryUpProd.lastError().text().size() > 3){
                    error.append(queryUpProd.lastError().text());
                } else {
                    del_list.append(row);
                }
            }
        }
    }
    if (error.size() > 0){
        makeMessage(error, false);
    } else {
        if (del_list.size() > 0){
            for (int x = del_list.size() - 1; x >= 0; x--){
                ui->tableWidget_f_product->removeRow(del_list.at(x));
            }
        }
    }
    ui->progressBar->setValue(0);
}

void product_form::makeMessage(const QString str, bool x)
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
