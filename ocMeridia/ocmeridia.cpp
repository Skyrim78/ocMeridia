#include "ocmeridia.h"
#include "ui_ocmeridia.h"

ocMeridia::ocMeridia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ocMeridia)
{
    ui->setupUi(this);


//    mCategory *mcat = new mCategory();
//    mcat->category_openXML();



    ui->groupBox_messa->hide();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->groupBox_messa, SLOT(hide()));

    //load app
    readSetting();

//    connect_netDB();
//    connect_locDB();

    //setting
    connect(ui->pushButton_sA_connect, SIGNAL(clicked(bool)), this, SLOT(setting_testConnect()));
    connect(ui->pushButton_settingWrite, SIGNAL(clicked(bool)), this, SLOT(writeSetting()));
    connect(ui->comboBox_sB_lang, SIGNAL(activated(int)), this, SLOT(setting_changeLangStore(int)));
    connect(ui->toolButton_dir1C, SIGNAL(clicked(bool)), this, SLOT(setting_selectDir1C()));
    connect(ui->tableWidget_priceFiles, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(setting_selectDirPL()));
    connect(ui->toolButton_pl_edit, SIGNAL(clicked(bool)), this, SLOT(setting_selectDirPL()));
    //connect(ui->toolButton_dirPL, SIGNAL(clicked(bool)), this, SLOT(setting_selectDirPL()));

    //file
    connect(ui->toolButton_file, SIGNAL(clicked(bool)), this, SLOT(file_Select()));
    connect(ui->pushButton_fm_openFile, SIGNAL(clicked(bool)), this, SLOT(file_Open_PL()));


    //category
    category_loadFromDB();
    connect(ui->toolButtoncat_par_clear, SIGNAL(clicked(bool)), ui->lineEdit_cat_active, SLOT(clear()));
    connect(ui->treeWidget_cat_serv, SIGNAL(clicked(QModelIndex)), this, SLOT(category_selectServer()));
    connect(ui->radioButton_cat_add, SIGNAL(clicked(bool)), this, SLOT(category_checkState()));
    connect(ui->radioButton_cat_equi, SIGNAL(clicked(bool)), this, SLOT(category_checkState()));
    connect(ui->pushButton_cat_save, SIGNAL(clicked(bool)), this, SLOT(category_save()));

    //product
    //ui->tableWidget_product->setColumnHidden(0, true);
    connect(ui->tableWidget_product, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(product_openForm()));
    connect(ui->pushButton_loadOnServer, SIGNAL(clicked(bool)), this, SLOT(product_loadOnServer()));
}

ocMeridia::~ocMeridia()
{

    delete ui;
}

void ocMeridia::readSetting()
{
    QSettings sett("setting.ini", QSettings::IniFormat);
    ui->lineEdit_sA_dbname->setText(sett.value("db/name").toString());
    ui->lineEdit_sA_host->setText(sett.value("db/host").toString());
    ui->spinBox_sA_port->setValue(sett.value("db/port").toInt());
    ui->lineEdit_sA_username->setText(sett.value("db/user").toString());
    ui->lineEdit_sA_pass->setText(sett.value("db/pass").toString());

    _STORE = 0;
    _LANG = sett.value("oc/lang").toInt();
    _TAX = sett.value("oc/tax").toInt();
    _STOCK = sett.value("oc/stock").toInt();
    _STATUS = sett.value("oc/status").toInt();
    _QUAN = sett.value("oc/quan").toDouble();
    _DIR = "catalog/";
    _DIR.append(sett.value("oc/dir").toString());
    ui->lineEdit_sB_dir->setText(sett.value("oc/dir").toString());

    //--------------------loads
    ui->radioButton_sC_catout->setChecked(sett.value("load1c/cat_out").toBool());
    ui->radioButton_sC_catin->setChecked(sett.value("load1c/cat_in").toBool());
    ui->radioButton_sC_priceount->setChecked(sett.value("load1c/prod_out").toBool());
    ui->radioButton_sC_pricein->setChecked(sett.value("load1c/prod_in").toBool());
    //ui->doubleSpinBox_sC_pricein->setValue(sett.value("load1c/prod_price").toDouble());
    ui->radioButton_sC_newProdOunt->setChecked(sett.value("load1c/new_prod_out").toBool());
    ui->radioButton_sC_newProdIn->setChecked(sett.value("load1c/new_prod_in").toBool());
    ui->checkBox_sC_upname->setChecked(sett.value("load1c/up_name").toInt());
    ui->checkBox_sC_upprice->setChecked(sett.value("load1c/up_price").toInt());
    ui->checkBox_sC_updesc->setChecked(sett.value("load1c/up_desc").toInt());
    ui->checkBox_sC_upimage->setChecked(sett.value("load1c/up_image").toInt());
    ui->checkBox_sC_upcolor->setChecked(sett.value("load1c/up_color").toInt());

    ui->radioButton_sCpl_catout->setChecked(sett.value("loadpl/cat_out").toBool());
    ui->radioButton_sCpl_catin->setChecked(sett.value("loadpl/cat_in").toBool());
    ui->radioButton_sCpl_prodout->setChecked(sett.value("loadpl/prod_out").toBool());
    ui->radioButton_sCpl_prodin->setChecked(sett.value("loadpl/prod_in").toBool());
    ui->radioButton_sCpl_upall->setChecked(sett.value("loadpl/up_all").toBool());
    ui->radioButton_sCpl_upprice->setChecked(sett.value("loadpl/up_price").toBool());



    //load status products
    ui->comboBox_sB_status->setCurrentIndex(statusMap.key(_STATUS));
    ui->doubleSpinBox_sB_quan->setValue(_QUAN);




    setting_connect_netDB();
    setting_connect_locDB();

    //----auto file
    QSqlQuery queryMainFile("SELECT files.fname FROM files WHERE files.vid = \'2\' ", db_local);
    queryMainFile.next();
    if (queryMainFile.isValid()){
        ui->lineEdit_sD_dir1c->setText(queryMainFile.value(0).toString());
    }
    setting_loadListFiles();
    ui->tableWidget_priceFiles->setColumnHidden(0, true);

}

void ocMeridia::writeSetting()
{
    QSettings sett("setting.ini", QSettings::IniFormat);
    sett.beginGroup("db");
    sett.setValue("name", ui->lineEdit_sA_dbname->text());
    sett.setValue("host", ui->lineEdit_sA_host->text());
    sett.setValue("port", ui->spinBox_sA_port->value());
    sett.setValue("user", ui->lineEdit_sA_username->text());
    sett.setValue("pass", ui->lineEdit_sA_pass->text());
    sett.endGroup();
    sett.beginGroup("oc");
    sett.setValue("lang", langMap.value(ui->comboBox_sB_lang->currentIndex()));
    sett.setValue("tax", taxMap.value(ui->comboBox_sB_tax->currentIndex()));
    sett.setValue("stock", stockMap.value(ui->comboBox_sB_stock->currentIndex()));
    sett.setValue("status", statusMap.value(ui->comboBox_sB_status->currentIndex()));
    sett.setValue("quan", ui->doubleSpinBox_sB_quan->value());
    sett.setValue("dir", ui->lineEdit_sB_dir->text());
    sett.endGroup();
    sett.beginGroup("load1c");
    sett.setValue("cat_out", ui->radioButton_sC_catout->isChecked());
    sett.setValue("cat_in", ui->radioButton_sC_catin->isChecked());
    sett.setValue("cat_target", categoryMap.value(ui->comboBox_sC_catin->currentIndex()));
    sett.setValue("prod_out", ui->radioButton_sC_priceount->isChecked());
    sett.setValue("prod_in", ui->radioButton_sC_pricein->isChecked());
    //sett.setValue("prod_price", ui->doubleSpinBox_sC_pricein->value());
    sett.setValue("new_prod_out", ui->radioButton_sC_newProdOunt->isChecked());
    sett.setValue("new_prod_in", ui->radioButton_sC_newProdIn->isChecked());
    sett.setValue("up_name", ui->checkBox_sC_upname->checkState());
    sett.setValue("up_price", ui->checkBox_sC_upprice->checkState());
    sett.setValue("up_desc", ui->checkBox_sC_updesc->checkState());
    sett.setValue("up_image", ui->checkBox_sC_upimage->checkState());
    sett.setValue("up_color", ui->checkBox_sC_upcolor->checkState());
    sett.endGroup();
    sett.beginGroup("loadpl");
    sett.setValue("cat_out", ui->radioButton_sCpl_catout->isChecked());
    sett.setValue("cat_in", ui->radioButton_sCpl_catin->isChecked());
    sett.setValue("cat_target", categoryMap.value(ui->comboBox_sCpl_catin->currentIndex()));
    sett.setValue("prod_out", ui->radioButton_sCpl_prodout->isChecked());
    sett.setValue("prod_in", ui->radioButton_sCpl_prodin->isChecked());
    sett.setValue("up_all", ui->radioButton_sCpl_upall->isChecked());
    sett.setValue("up_price", ui->radioButton_sCpl_upprice->isChecked());
    sett.endGroup();
    sett.beginGroup("auto");
    sett.setValue("1cdir", ui->lineEdit_sD_dir1c->text());
    //sett.setValue("pldir", ui->lineEdit_sD_dirpl->text());
    sett.endGroup();

    makeMessage("Настройки сохранены", true);
}


void ocMeridia::setting_connect_netDB()
{
    db_server = QSqlDatabase::addDatabase("QMYSQL", "serverDB");
    db_server.setDatabaseName(ui->lineEdit_sA_dbname->text());
    db_server.setHostName(ui->lineEdit_sA_host->text());
    db_server.setPort(ui->spinBox_sA_port->value());
    db_server.setUserName(ui->lineEdit_sA_username->text());
    db_server.setPassword(ui->lineEdit_sA_pass->text());
    db_server.open();

    if (db_server.isOpen()){
        makeMessage("К удаленной БД подключено", true);

        //load store
        _STORE = 0; // дефолтный магазин

        //load language
        langMap.clear();
        ui->comboBox_sB_lang->clear();
        QSqlQuery qLang("SELECT rmrt_language.language_id, rmrt_language.name FROM rmrt_language ", db_server);
        while (qLang.next()){
            ui->comboBox_sB_lang->addItem(qLang.value(1).toString());
            langMap.insert(ui->comboBox_sB_lang->count() - 1, qLang.value(0).toInt());
        }
        if (_LANG > 0){
            ui->comboBox_sB_lang->setCurrentIndex(langMap.key(_LANG));
            loadMaps();
            ui->tab_file->setEnabled(true);
            ui->tab_category->setEnabled(true);
            ui->tab_product->setEnabled(true);
        } else {
            ui->tabWidget_main->setCurrentIndex(0);
            ui->tab_file->setEnabled(false);
            ui->tab_category->setEnabled(false);
            ui->tab_product->setEnabled(false);
            ui->tabWidget_setting->setCurrentIndex(1);
        }

        ui->pushButton_settingWrite->setEnabled(true);



        ui->tab_main->setEnabled(true);
        ui->tab_load->setEnabled(true);
        ui->tab_auto->setEnabled(true);

    } else {
        makeMessage("К удаленной БД не подключено", false);
        ui->tabWidget_main->setCurrentIndex(0);
        ui->tab_file->setEnabled(false);
        ui->tab_category->setEnabled(false);
        ui->tab_product->setEnabled(false);
        ui->tabWidget_setting->setCurrentIndex(0);
        ui->pushButton_settingWrite->setEnabled(false);
        ui->tab_main->setEnabled(false);
        ui->tab_load->setEnabled(false);
        ui->tab_auto->setEnabled(false);
    }
}

void ocMeridia::setting_connect_locDB()
{
    db_local = QSqlDatabase::addDatabase("QSQLITE", "localDB");
    db_local.setDatabaseName("meridia.db3");
    db_local.open();
}

void ocMeridia::setting_testConnect()
{
    if(db_server.isOpen()){
        db_server.close();
        db_server.removeDatabase("serverDB");
    }
    setting_connect_netDB();
    if (!db_local.isOpen()){
        setting_connect_locDB();
    }
}

void ocMeridia::loadMaps()
{
    //status
    statusMap.clear();
    statusMap.insert(0, 1);
    statusMap.insert(1, 0);

    //load tax class
    taxMap.clear();
    ui->comboBox_sB_tax->clear();
    QSqlQuery qTaxClass("SELECT rmrt_tax_class.tax_class_id, rmrt_tax_class.title FROM rmrt_tax_class", db_server);
    while (qTaxClass.next()){
        ui->comboBox_sB_tax->addItem(qTaxClass.value(1).toString());
        taxMap.insert(ui->comboBox_sB_tax->count() - 1, qTaxClass.value(0).toInt());
    }
    ui->comboBox_sB_tax->setCurrentIndex(taxMap.key(_TAX));

    //load stock status
    stockMap.clear();
    ui->comboBox_sB_stock->clear();
    QSqlQuery qStockStatus(QString("SELECT rmrt_stock_status.stock_status_id, rmrt_stock_status.name "
                                   "FROM rmrt_stock_status "
                                   "WHERE rmrt_stock_status.language_id = \'%1\' ")
                           .arg(_LANG), db_server);
    while (qStockStatus.next()){
        ui->comboBox_sB_stock->addItem(qStockStatus.value(1).toString());
        stockMap.insert(ui->comboBox_sB_stock->count() - 1, qStockStatus.value(0).toInt());
    }
    ui->comboBox_sB_stock->setCurrentIndex(stockMap.key(_STOCK));



    //load category
    ui->comboBox_sCpl_catin->clear();
    ui->comboBox_sC_catin->clear();
    categoryMap.clear();
    QSqlQuery qCategory(QString("SELECT rmrt_category_description.category_id, rmrt_category_description.name "
                                "FROM rmrt_category_description "
                                "WHERE rmrt_category_description.language_id = \'%1\' ").arg(_LANG), db_server);
    while (qCategory.next()){
        ui->comboBox_sC_catin->addItem(qCategory.value(1).toString());
        ui->comboBox_sCpl_catin->addItem(qCategory.value(1).toString());
        categoryMap.insert(ui->comboBox_sC_catin->count() - 1, qCategory.value(0).toInt());
    }
    QSettings sett("setting.ini", QSettings::IniFormat);
    ui->comboBox_sC_catin->setCurrentIndex(categoryMap.key(sett.value("load1c/cat_target").toInt()));
    ui->comboBox_sCpl_catin->setCurrentIndex(categoryMap.key(sett.value("loadpl/cat_target").toInt()));

    category_loadFromDB();


}


void ocMeridia::setting_changeLangStore(int idx)
{
    _LANG = langMap.value(idx);
    loadMaps();
}

void ocMeridia::setting_selectDir1C()
{
    QString fname = QFileDialog::getOpenFileName(this, "Select file...", "HOME", "XML 1C (*.cml)");
    if (fname.size() > 0){
        if (ui->lineEdit_sD_dir1c->text().isEmpty()){
            ui->lineEdit_sD_dir1c->setText(fname);
            QSqlQuery query("INSERT INTO files (fname, vid, active) VALUES (?, ?, ?)", db_local);
            query.bindValue(0, fname);
            query.bindValue(1, 2);
            query.bindValue(2, 2);
            query.exec();
        } else {
            ui->lineEdit_sD_dir1c->setText(fname);
            QSqlQuery query(QString("UPDATE files SET fname = \'%1\' WHERE files.vid = \'2\' ").arg(fname), db_local);
            query.exec();
        }
    }
}

void ocMeridia::setting_loadListFiles()
{
    for (int row = ui->tableWidget_priceFiles->rowCount() - 1; row >= 0; row--){
        ui->tableWidget_priceFiles->removeRow(row);
    }
    QSqlQuery queryPriceFile("SELECT files.id, files.fname FROM files WHERE files.vid = \'0\' AND files.active = \'1\' ", db_local);
    int row = 0;
    while (queryPriceFile.next()){
        ui->tableWidget_priceFiles->insertRow(row);
        for (int col = 0; col < 2; col++){
            QTableWidgetItem *item = new QTableWidgetItem(queryPriceFile.value(col).toString());
            ui->tableWidget_priceFiles->setItem(row, col, item);
        }
        row++;
    }
    ui->tableWidget_priceFiles->resizeColumnsToContents();
    ui->tableWidget_priceFiles->horizontalHeader()->setStretchLastSection(true);

}

void ocMeridia::setting_selectDirPL()
{
    setting *se = new setting(db_local, this);
    se->exec();
    setting_loadListFiles();
}
//***************************************************************************
//---------------------------FILE--------------------------------------------
void ocMeridia::file_Select()
{
    if (ui->radioButton_f_openMain->isChecked()){
        ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Open", "HOME", "XML 1C (*.cml)"));
        if (!ui->lineEdit_file->text().isEmpty()){
            file_Open_Main();
        }
    } else if (ui->radioButton_f_openPL->isChecked()){
        ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Open", "HOME", "CSV (*.csv)"));
    }

}

void ocMeridia::file_Open_PL()
{

}


void ocMeridia::file_Open_Main()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    for (int r = ui->tableWidget_cat_file->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_cat_file->removeRow(r);
    }
    for (int r = ui->tableWidget_product->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_product->removeRow(r);
    }

    QStringList result;

    QFile file(ui->lineEdit_file->text());
    if (file.open(QIODevice::ReadOnly)){
        doc = new QDomDocument();
        doc->setContent(&file);

        QDomNode nodeCom = doc->childNodes().at(1); //коммерч информация
        QDomNode nodeCl = nodeCom.childNodes().at(0); //классификатор

        //reading groups

        QDomNode nodeGroups = nodeCl.childNodes().at(2);

        for (int x = 0; x < nodeGroups.childNodes().count(); x++){
            ui->tableWidget_cat_file->insertRow(x);
            QDomNode nodeGroup = nodeGroups.childNodes().at(x);

            QString _ID = nodeGroup.firstChildElement("Ид").text();
            QTableWidgetItem *itemID = new QTableWidgetItem(_ID);
            ui->tableWidget_cat_file->setItem(x, 0, itemID);

            QString _NAME = nodeGroup.firstChildElement("Наименование").text();
            QTableWidgetItem *itemNAME = new QTableWidgetItem(_NAME);
            ui->tableWidget_cat_file->setItem(x, 1, itemNAME);

            QString _PARENT = nodeGroup.firstChildElement("Родитель").text();
            QTableWidgetItem *itemPar = new QTableWidgetItem(_PARENT);
            ui->tableWidget_cat_file->setItem(x, 2, itemPar);

            result.append(QString("%1||%2||%3").arg(_ID).arg(_NAME).arg(_PARENT));
            ui->progressBar->setValue(qFloor((x+1) * 100 / nodeGroups.childNodes().count()));
            QApplication::processEvents();

        }
        ui->progressBar->setValue(0);

        //reading products

        QDomNode nodeAll = nodeCom.childNodes().at(1); //комерч предложения

        int idxPred = 0;
        int idxProd = 0;
        for (int x = 0; x < nodeAll.childNodes().count(); x++){
            if (nodeAll.childNodes().at(x).nodeName() == "Предложения"){
                idxPred = x;
            }
            if (nodeAll.childNodes().at(x).nodeName() == "Товары"){
                idxProd = x;
            }
        }

        QDomNode nodePredAll = nodeAll.childNodes().at(idxPred);

        QString line;
        QMap<QString, double> mapPrice; //ИД товаров + Цена

        // собираем ID товаров и цены из предложений
        for (int x = 0; x < nodePredAll.childNodes().count(); x++){
            QDomNode nodePred = nodePredAll.childNodes().at(x);
            QString _ID = nodePred.firstChildElement("Ид").text();
            QString _Price = nodePred.firstChildElement("Цены").firstChildElement("Цена")
                    .firstChildElement("ЦенаЗаЕдиницу").text();
            if (_Price.isEmpty()){
                mapPrice.insert(_ID, 0.00);
            } else {
                mapPrice.insert(_ID, _Price.toDouble());
            }

            ui->progressBar->setValue(qFloor((x+1) * 100 / nodePredAll.childNodes().count()));
            QApplication::processEvents();
        }
        ui->progressBar->setValue(0);

        //формируем карту товаров
        QMap<QString, QString> mapProduct;
        QDomNode nodeProducts = nodeAll.childNodes().at(idxProd);
        QString _line;
        for (int y = 0; y < 500; y++){; //*/nodeProducts.childNodes().count(); y++){ //****
            QDomNode nodeProduct = nodeProducts.childNodes().at(y);
            QString _ID = nodeProduct.firstChildElement("Ид").text();
            if (_ID.size() == 36){
                line.clear();
                line.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
                line.append(QString("||%0").arg(QString::number(mapPrice.value(_ID), 'f', 2)));//Цена
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                line.append(QString("||%0||Цвет:").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                        line.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                    }
                }
                //qDebug() << line;
                mapProduct.insert(_ID, line);
            } else if (_ID.size() > 36){
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ИДРодителя"){
                        _ID =  nAtt.firstChildElement("Значение").text();
                    }
                }
                if (_ID.size() == 36){
                    QString line_post = mapProduct.value(_ID);
                    if (line_post.isEmpty()){
                        line_post.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
                        line_post.append(QString("||%0").arg(QString::number(mapPrice.value(_ID), 'f', 2)));//Цена
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                        line_post.append(QString("||%0||Цвет:").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                line_post.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                            }
                        }
                        mapProduct.insert(_ID, line_post);
                    } else {
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                line_post.append(QString("%0,").arg(nAtt.firstChildElement("Значение").text()));
                            }
                        }
                        mapProduct.remove(_ID);
                        mapProduct.insert(_ID, line_post);
                    }
                } else {
                    qDebug() << _ID << "Нет родителя ";
                }
            }
            ui->progressBar->setValue(qFloor((y+1) * 100 / 500)); //*/nodeProducts.childNodes().count()));
            QApplication::processEvents();
        }
        for (int x = 0; x < mapProduct.count(); x++){
            ui->tableWidget_product->insertRow(x);

            QTableWidgetItem *itemID = new QTableWidgetItem(mapProduct.keys().at(x));
            ui->tableWidget_product->setItem(x, 1, itemID);

            QString str = mapProduct.value(mapProduct.keys().at(x));
            for (int col = 0; col < 6 ; col++){
                QTableWidgetItem *item = new QTableWidgetItem();
                if (col == 5){
                    item->setText(str.split("||").at(col).mid(5));
                } else {
                    item->setText(str.split("||").at(col));
                }
                ui->tableWidget_product->setItem(x, col + 2, item);
            }
            ui->progressBar->setValue(qFloor((x+1) * 100 / mapProduct.count()));
            QApplication::processEvents();
        }
    }

    file.close();

    category_testServer();

    ui->tableWidget_cat_file->resizeColumnsToContents();
    ui->tableWidget_cat_file->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_product->resizeColumnsToContents();
    ui->tableWidget_product->horizontalHeader()->setStretchLastSection(true);
    ui->progressBar->hide();

}

void ocMeridia::file_loadMaket()
{
    ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Open", "HOME/makets", "ocMeridia Maket (*.ini)"));
    if (!ui->lineEdit_file->text().isEmpty()){
        QSettings maket(ui->lineEdit_fm_maket->text(), QSettings::IniFormat);
        ui->comboBox_f_csv_split->setCurrentText(maket.value("split").toString());
        ui->spinBox_f_csv_firstRow->setValue(maket.value("firstRow").toInt());
        ui->checkBox_fm_category->setChecked(maket.value("cat_check").toBool());
        ui->checkBox_fm_articul->setChecked(maket.value("art_check").toBool());
        ui->checkBox_fm_model->setChecked(maket.value("model_check").toBool());
        ui->checkBox_fm_name->setChecked(maket.value("name_check").toBool());
        ui->checkBox_fm_manuf->setChecked(maket.value("manuf_check").toBool());
        ui->checkBox_fm_price->setChecked(maket.value("price_check").toBool());
        ui->checkBox_fm_quan->setChecked(maket.value("quan_check").toBool());
        ui->checkBox_fm_desc->setChecked(maket.value("desc_check").toBool());
        ui->checkBox_fm_image->setChecked(maket.value("image_check").toBool());
        ui->checkBox_fm_attr->setChecked(maket.value("attr_check").toBool());
        ui->lineEdit_fm_category->setText(maket.value("cat").toString());
        ui->lineEdit_fm_desc->setText(maket.value("desc").toString());
        ui->lineEdit_fm_image->setText(maket.value("image").toString());
        ui->lineEdit_fm_attr->setText(maket.value("attr").toString());
        ui->spinBox_fm_articul->setValue(maket.value("art").toInt());
        ui->spinBox_fm_model->setValue(maket.value("model").toInt());
        ui->spinBox_fm_name->setValue(maket.value("name").toInt());
        ui->spinBox_fm_manuf->setValue(maket.value("manuf").toInt());
        ui->spinBox_fm_price->setValue(maket.value("price").toInt());
        ui->spinBox_fm_quan->setValue(maket.value("quan").toInt());
    }
}

void ocMeridia::file_saveMaket()
{

}

void ocMeridia::category_loadFromDB()
{
    // загружаем список категорий сервера в форму редактора

    //строим дерево категорий
    ui->treeWidget_cat_serv->clear();

    QSqlQuery query(QString("SELECT rmrt_category.category_id, rmrt_category_description.name, rmrt_category.code "
                     "FROM rmrt_category "
                     "INNER JOIN rmrt_category_description ON rmrt_category_description.category_id = rmrt_category.category_id "
                     "WHERE rmrt_category.parent_id = \'0\' "
                     "AND rmrt_category_description.language_id = \'%0\' ").arg(_LANG), db_server);

    while (query.next()){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, query.value(1).toString());
        item->setText(1, query.value(2).toString());
        item->setText(2, query.value(0).toString());
        ui->treeWidget_cat_serv->addTopLevelItem(item);

        category_loadFromDB_parent(item, query.value(0).toString());
    }
    ui->treeWidget_cat_serv->expandAll();
    ui->treeWidget_cat_serv->setColumnHidden(2, true);
    ui->treeWidget_cat_serv->resizeColumnToContents(0);
    ui->treeWidget_cat_serv->resizeColumnToContents(1);
    ui->treeWidget_cat_serv->header()->setStretchLastSection(true);


}

void ocMeridia::category_loadFromDB_parent(QTreeWidgetItem *par, QString id_parent)
{
    QSqlQuery query(QString("SELECT rmrt_category.category_id, rmrt_category_description.name, rmrt_category.code "
                            "FROM rmrt_category "
                            "INNER JOIN rmrt_category_description ON rmrt_category_description.category_id = rmrt_category.category_id "
                            "WHERE rmrt_category.parent_id = \'%0\' "
                            "AND rmrt_category_description.language_id = \'%1\'  ")
                    .arg(id_parent)
                    .arg(_LANG), db_server);
    while (query.next()){
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0, query.value(1).toString());
        item->setText(1, query.value(2).toString());
        item->setText(2, query.value(0).toString());
        par->addChild(item);

        category_loadFromDB_parent(item, query.value(0).toString());
    }
}

void ocMeridia::category_selectServer()
{
    ui->lineEdit_cat_active->setText(ui->treeWidget_cat_serv->currentItem()->text(0));
    cat_activeID = ui->treeWidget_cat_serv->currentItem()->text(2).toInt();
}

void ocMeridia::category_checkState()
{
    if (ui->radioButton_cat_add->isChecked()){
        ui->tableWidget_cat_file->setSelectionMode(QTableWidget::ExtendedSelection);
        ui->label_cat_active->setText("Родитель:");
    } else if (ui->radioButton_cat_equi->isChecked()){
        ui->tableWidget_cat_file->setSelectionMode(QTableWidget::SingleSelection);
        ui->label_cat_active->setText("Категория:");
    }
}

void ocMeridia::category_testServer()
{
    if (ui->tableWidget_cat_file->rowCount() > 0){
        QList<int> del_list;
        for (int row = 0; row < ui->tableWidget_cat_file->rowCount(); row++){
            //test 1:проверяем наличие каждой записи на сервере по коду 1С
            QSqlQuery queryS(QString("SELECT rmrt_category.category_id "
                                    "FROM rmrt_category "
                                    "WHERE rmrt_category.code = \'%0\' ")
                            .arg(ui->tableWidget_cat_file->item(row, 0)->text()), db_server);
            queryS.next();
            if (queryS.isValid()){ // есть -> подставляем ID в табл. товары, удаляем строку
                // меняем в таблице товаров код1С на ID групп магазина
                for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                    QString productG1C = ui->tableWidget_product->item(rowp, 3)->text();
                    QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                    if (productG1C == group1C){
                        QTableWidgetItem *item = new QTableWidgetItem(queryS.value(0).toString());
                        ui->tableWidget_product->setItem(rowp, 3, item);
                    }
                }
                // cтроки для удаления
                del_list.append(row);
            } else {
                //test2: проверяем наличие записи по имени
                QSqlQuery queryS2(QString("SELECT rmrt_category_description.category_id "
                                          "FROM rmrt_category_description "
                                          "WHERE rmrt_category_description.name = \"%1\" ")
                                  .arg(ui->tableWidget_cat_file->item(row, 1)->text()), db_server);
                queryS2.next();
                if (queryS2.isValid()){
                    //если такая группа есть: добавляем код1С, подставляем ID в таблицу товары, удаляем строку
                    QSqlQuery queryU(QString("UPDATE rmrt_category SET code = \'%0\' "
                                             "WHERE rmrt_category.category_id = \'%1\' ")
                                     .arg(ui->tableWidget_cat_file->item(row, 0)->text())
                                     .arg(queryS2.value(0).toString()), db_server);
                    queryU.exec();
                    // меняем в таблице товаров код1С на ID групп магазина
                    for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                        QString productG1C = ui->tableWidget_product->item(rowp, 3)->text();
                        QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                        if (productG1C == group1C){
                            QTableWidgetItem *item = new QTableWidgetItem(queryS2.value(0).toString());
                            ui->tableWidget_product->setItem(rowp, 3, item);
                        }
                    }
                    // cтроки для удаления
                    del_list.append(row);
                } else {
                    // проверяем наличие категории в синонимах
                    QSqlQuery queryL(QString("SELECT cat.id_db FROM cat WHERE cat.cod = \'%1\'")
                                     .arg(ui->tableWidget_cat_file->item(row, 0)->text())
                                     , db_local);
                    queryL.next();
                    if (queryL.isValid()){
                        // меняем в таблице товаров код1С на ID группы из справочника синонимов
                        for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                            QString productG1C = ui->tableWidget_product->item(rowp, 3)->text();
                            QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                            if (productG1C == group1C){
                                QTableWidgetItem *item = new QTableWidgetItem(queryL.value(0).toString());
                                ui->tableWidget_product->setItem(rowp, 3, item);
                            }
                        }
                        // cтроки для удаления
                        del_list.append(row);
                    } else {
                    //оставляем в ручную обработку
                    }
                }
            }
        }
        if (del_list.size() > 0){
            for (int x = del_list.size() - 1; x >= 0; x--){
                ui->tableWidget_cat_file->removeRow(del_list.at(x));
            }
        }
        // если обработка полностью закончена запускаем процесс проверки товаров
        if (ui->tableWidget_cat_file->rowCount() == 0){
            product_testServer();
        }
    }

}

void ocMeridia::category_save()
{
    QString error;
    //QList<int> del_list;
    if (ui->radioButton_cat_add->isChecked()){
        // ****
        //добавляем в ОС
        for (int row = 0; row < ui->tableWidget_cat_file->rowCount(); row++){
            if (ui->tableWidget_cat_file->item(row, 0)->isSelected()){
                QString cat_name = ui->tableWidget_cat_file->item(row, 1)->text();
                QDateTime cat_dt = QDateTime::currentDateTime();

                int cat_top = 0;

                if (ui->lineEdit_cat_active->text().isEmpty()){
                    cat_activeID = 0;
                    cat_top = 1;
                }
                QSqlQuery query_C ("INSERT INTO `rmrt_category` (`parent_id`, `top`, `column`, `status`, `date_added`, `date_modified`, `code`) "
                                "VALUES (?, ?, ?, ?, ?, ?, ?) ", db_server);
                query_C.bindValue(0, cat_activeID);
                query_C.bindValue(1, cat_top);
                query_C.bindValue(2, 1);
                query_C.bindValue(3, _STATUS);
                query_C.bindValue(4, cat_dt.toString("yyyy-MM-dd hh:mm:ss"));
                query_C.bindValue(5, cat_dt.toString("yyyy-MM-dd hh:mm:ss"));
                query_C.bindValue(6, ui->tableWidget_cat_file->item(row, 0)->text());
                query_C.exec();
                if (query_C.lastError().text().size() > 3){
                    error.append(query_C.lastError().text());
                    break;
                } else {
                    int cat_ID = query_C.lastInsertId().toInt();


                    //description
                    QSqlQuery query_CD("INSERT INTO `rmrt_category_description` (`category_id`, `language_id`, `name`, `description`) "
                                       "VALUES (?, ?, ?, ?)", db_server);
                    query_CD.bindValue(0, cat_ID);
                    query_CD.bindValue(1, _LANG);
                    query_CD.bindValue(2, cat_name);
                    query_CD.bindValue(3, cat_name);
                    query_CD.exec();
                    if (query_CD.lastError().text().size() > 3){
                        error.append(query_CD.lastError().text());
                    }
                    //store
                    QSqlQuery query_CS("INSERT INTO `rmrt_category_to_store` (`category_id`, `store_id`) VALUES (?, ?)", db_server);
                    query_CS.bindValue(0, cat_ID);
                    query_CS.bindValue(1, _STORE);
                    query_CS.exec();
                    if (query_CD.lastError().text().size() > 3){
                        error.append(query_CD.lastError().text());
                    }

                    //path--------------------------------------
                    //формируем список oc_category_path
                    QStringList pathList;

                    QSqlQuery queryP_Del("DELETE FROM `rmrt_category_path`", db_server);
                    queryP_Del.exec();

                    //выборка категорий
                    QSqlQuery queryPS ("SELECT `rmrt_category`.`category_id`, `rmrt_category`.`parent_id` FROM `rmrt_category`", db_server);
                    while (queryPS.next()){
                        category_tempList.clear();
                        int _id = queryPS.value(0).toInt();
                        int _parent = queryPS.value(1).toInt();
                        category_tempList.append(QString("%1|%1").arg(_id));
                        if (_parent > 0){
                            category_makePath(_id, _parent);
                        }

                        int level = category_tempList.size() - 1;
                        for (int x = 0; x < category_tempList.size(); x++){
                            pathList.append(QString("%1|%2").arg(category_tempList.at(x)).arg(level));
                            level--;
                        }
                    }
                    //путь категорий в таб oc_cayegory_padd
                    if (pathList.size() > 0){
                        for (int x = 0; x < pathList.size(); x++){
                               QSqlQuery queryS6_b("INSERT INTO `rmrt_category_path` (`category_id`, `path_id`, `level`) "
                                                   "VALUES (?, ?, ?) ", db_server);
                               queryS6_b.bindValue(0, pathList.at(x).split("|").at(0));
                               queryS6_b.bindValue(1, pathList.at(x).split("|").at(1));
                               queryS6_b.bindValue(2, pathList.at(x).split("|").at(2));
                               queryS6_b.exec();
                        }
                    }
                    //-------*********--------------------------------------------------------------------------------
                }
            }
        }
    } else if (ui->radioButton_cat_equi->isChecked()){
        if (ui->lineEdit_cat_active->text().isEmpty()){
            error.append("Выберите категорию магазина, к которой приравниваете категорию из прайса");
        } else {
            // синоним добавляем в локальную БД
            int crow = ui->tableWidget_cat_file->currentRow();

            QSqlQuery query("INSERT INTO `cat` (`cod`, `name`, `id_db`) VALUES (?, ?, ?)", db_local);
            query.bindValue(0, ui->tableWidget_cat_file->item(crow, 0)->text());
            query.bindValue(1, ui->tableWidget_cat_file->item(crow, 1)->text());
            query.bindValue(2, cat_activeID);
            query.exec();

            if (query.lastError().text().size() > 3){
                error.append(query.lastError().text());
            }
        }
    }
    if (error.size() > 0){
        makeMessage(error, false);
    } else {
        makeMessage("Сохранено", true);
        category_loadFromDB();
        category_testServer();
    }
}

void ocMeridia::category_makePath(int _ID, int _PAR)
{
    category_tempList.append(QString("%1|%2").arg(_ID).arg(_PAR));
    QSqlQuery query(QString("SELECT `rmrt_category`.`parent_id` FROM `rmrt_category` WHERE `rmrt_category`.`category_id` = \'%1\' ")
                    .arg(_PAR), db_server);
    query.next();
    int id_parent = query.value(0).toInt();
    if (id_parent > 0){
        category_makePath(_ID, id_parent);
    }
}

void ocMeridia::product_testServer()
{
    if (ui->tableWidget_product->rowCount() > 0){
        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(true);

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            //проверяем по коду 1с
            bool idx = false;
            QSqlQuery queryS(QString("SELECT rmrt_product.product_id "
                                     "FROM rmrt_product "
                                     "WHERE rmrt_product.sku = \'%1\' ")
                             .arg(ui->tableWidget_product->item(row, 1)->text()), db_server);
            queryS.next();
            if (queryS.isValid()){
                // если есть - ставим ИД
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setText(queryS.value(0).toString());
                ui->tableWidget_product->setItem(row, 0, item);
                idx = true;
            } else {
                //если нет - проверяем по наименованию
                QSqlQuery queryS2(QString("SELECT rmrt_product_description.product_id "
                                          "FROM rmrt_product_description "
                                          "WHERE rmrt_product_description.name = \"%1\" "
                                          "AND rmrt_product_description.language_id = \'%2\'  ")
                                  .arg(ui->tableWidget_product->item(row, 2)->text())
                                  .arg(_LANG), db_server);
                queryS2.next();
                if (queryS2.isValid()){
                    //если есть - ставим код 1С + ставим ИД
                    QSqlQuery queryU(QString("UPDATE rmrt_product "
                                             "SET sku = \'%0\' "
                                             "WHERE rmrt_product.product_id = \'%1\' ")
                                     .arg(ui->tableWidget_product->item(row, 1)->text())
                                     .arg(queryS2.value(0).toInt()), db_server);
                    queryU.exec();
                    QTableWidgetItem *item = new QTableWidgetItem();
                    item->setText(queryS2.value(0).toString());
                    ui->tableWidget_product->setItem(row, 0, item);
                    idx = true;
                } else {
                    QTableWidgetItem *item = new QTableWidgetItem("0");
                    ui->tableWidget_product->setItem(row, 0, item);
                    idx = false;
                    //помечаем
                    // обрабатываем по нажатию кнопки или по настройкам
                }
            }
            QColor colBG;
            QColor colFG;
            if (idx) {
                colBG.setNamedColor("#FAFAFA");
                colFG.setNamedColor("#006400");
            } else {
                colBG.setNamedColor("#FAFAFA");
                colFG.setNamedColor("#CD0000");
            }
            for (int col = 0; col < ui->tableWidget_product->columnCount(); col++){
                ui->tableWidget_product->item(row, col)->setBackgroundColor(colBG);
                ui->tableWidget_product->item(row, col)->setForeground(colFG);
            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
        }
        ui->progressBar->hide();
    }
}

void ocMeridia::product_openForm()
{
    if (ui->tableWidget_product->rowCount() > 0){
        QMap<QString, QString> loadProduct;
        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            if (ui->tableWidget_product->item(row, 0)->text().isEmpty()){

                loadProduct.insert(ui->tableWidget_product->item(row, 1)->text(), ui->tableWidget_product->item(row, 2)->text());

            }
        }
        product_form *pf = new product_form(_LANG, _STORE, db_server, loadProduct, this);
        pf->exec();
        product_testServer();
    }
}

void ocMeridia::product_loadOnServer()
{

    if (ui->tableWidget_product->rowCount() > 0){
        QString error;
        bool idxIN = true;
        bool idxUP = true;
        double price;
        QString image = "";

        // ID аттрибута "ЦВЕТ"
        QSqlQuery query_GA(QString("SELECT rmrt_attribute_description.attribute_id "
                                   "FROM rmrt_attribute_description "
                                   "WHERE rmrt_attribute_description.name = \'Цвет\' "
                                   "AND rmrt_attribute_description.language_id = \'%1\' ").arg(_LANG), db_server);
        query_GA.next();
        int IDcolor = query_GA.value(0).toInt();
        qDebug() << IDcolor;
        //*******

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            image.clear();
            price = ui->tableWidget_product->item(row, 4)->text().toDouble();
            int id = ui->tableWidget_product->item(row, 0)->text().toInt();
            if (id == 0){
                idxIN = true;
                idxUP = false;
                //add?
                if (ui->radioButton_sC_newProdIn->isChecked()){
                    idxIN = true;
                    // price?
                    //price = ui->tableWidget_product->item(row, 4)->text().toDouble();
                    if (price == 0.00){
                        if (ui->radioButton_sC_priceount->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                            idxIN = false;
                        } else if (ui->radioButton_sC_pricein->isChecked()) { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                            idxIN = true;
                        }
                    }
                } else if (ui->radioButton_sC_newProdOunt->isChecked()){
                    idxIN = false;
                }
            } else if (id > 0){
                idxUP = true;
                idxIN = false;

                if (price == 0.00){
                    if (ui->radioButton_sC_priceount->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                        idxUP = false;
                    } else if (ui->radioButton_sC_pricein->isChecked()) { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                        idxUP = true;
                    }
                }

            }
            if (idxIN){
                if (!ui->tableWidget_product->item(row, 5)->text().isEmpty()){
                    image = _DIR.append("/").append(ui->tableWidget_product->item(row, 5)->text());
                }

                QSqlQuery queryAddProduct("INSERT INTO rmrt_product (sku, quantity, manufacturer_id, stock_status_id, "
                                          "tax_class_id, status, price, image, date_added, date_modified) "
                                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", db_server);
                queryAddProduct.bindValue(0, ui->tableWidget_product->item(row, 1)->text());
                queryAddProduct.bindValue(1, QString::number(_QUAN, 'f', 2));
                queryAddProduct.bindValue(2, 0); // производителя нет
                queryAddProduct.bindValue(3, _STOCK); //stock status  = 7 (в наличие)
                queryAddProduct.bindValue(4, _TAX); // tax_class = 9 (облагаемый налогом)
                queryAddProduct.bindValue(5, _STATUS); // status = 1 (включено)
                queryAddProduct.bindValue(6, price); // price
                queryAddProduct.bindValue(7, image); // image
                queryAddProduct.bindValue(8, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
                queryAddProduct.bindValue(9, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
                queryAddProduct.exec();
                if (queryAddProduct.lastError().text().size() > 3){
                  error.append(queryAddProduct.lastError().text());
                  break;
                } else {
                    int _ID = queryAddProduct.lastInsertId().toInt();
                    //добавляем description
                    QSqlQuery queryAddProdDesc("INSERT INTO rmrt_product_description (product_id, language_id, name, description) "
                                               "VALUES (?, ?, ?, ?)", db_server);
                    queryAddProdDesc.bindValue(0, _ID);
                    queryAddProdDesc.bindValue(1, _LANG);
                    queryAddProdDesc.bindValue(2, ui->tableWidget_product->item(row, 2)->text());
                    queryAddProdDesc.bindValue(3, ui->tableWidget_product->item(row, 6)->text());
                    queryAddProdDesc.exec();
                    if (queryAddProdDesc.lastError().text().size() > 3){
                      error.append(queryAddProdDesc.lastError().text());
                    }

                    //добавляем store
                    QSqlQuery queryAddProdStore("INSERT INTO rmrt_product_to_store (product_id, store_id) "
                                               "VALUES (?, ?)", db_server);
                    queryAddProdStore.bindValue(0, _ID);
                    queryAddProdStore.bindValue(1, _STORE);
                    queryAddProdStore.exec();
                    if (queryAddProdStore.lastError().text().size() > 3){
                      error.append(queryAddProdStore.lastError().text());
                    }
                    //добавляем category
                    QSqlQuery queryAddProdCat("INSERT INTO rmrt_product_to_category (product_id, category_id, main_category) "
                                               "VALUES (?, ?, ?)", db_server);
                    queryAddProdCat.bindValue(0, _ID);
                    queryAddProdCat.bindValue(1, ui->tableWidget_product->item(row, 3)->text());
                    queryAddProdCat.bindValue(2, 1);
                    queryAddProdCat.exec();
                    if (queryAddProdCat.lastError().text().size() > 3){
                      error.append(queryAddProdCat.lastError().text());
                    }

                    //добавляем аттрибуты
                    QString strAttr = ui->tableWidget_product->item(row, 7)->text();
                    if (strAttr.size() > 1){
                        for (int x = 0; x < strAttr.split(",").size(); x++){
                            if (!strAttr.split(",").at(x).isEmpty()){
                                QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                                "VALUES (?, ?, ?, ?)", db_server);
                                queryAddProdAttribute.bindValue(0, _ID);
                                queryAddProdAttribute.bindValue(1, IDcolor);
                                queryAddProdAttribute.bindValue(2, _LANG);
                                queryAddProdAttribute.bindValue(3, strAttr.split(",").at(x));
                                queryAddProdAttribute.exec();
                                if (queryAddProdAttribute.lastError().text().size() > 3){
                                  error.append(queryAddProdAttribute.lastError().text());
                                }
                            }
                        }
                    }
                }
            }
            if (idxUP){
                QString mainStr;
                if (ui->checkBox_sC_upprice->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product SET price = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(price)
                                   .arg(id));
                }
                if (ui->checkBox_sC_upimage->isChecked()){
                    if (!ui->tableWidget_product->item(row, 5)->text().isEmpty()){
                        image = _DIR.append("/").append(ui->tableWidget_product->item(row, 5)->text());
                    }
                    mainStr.append(QString("UPDATE rmrt_product SET image = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(image)
                                   .arg(id));
                }
                if (ui->checkBox_sC_upname->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product_description SET `name` = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(ui->tableWidget_product->item(row, 2)->text())
                                   .arg(id)
                                   .arg(_LANG));
                }
                if (ui->checkBox_sC_updesc->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product_description SET description = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(ui->tableWidget_product->item(row, 6)->text())
                                   .arg(id)
                                   .arg(_LANG));
                }
                if (ui->checkBox_sC_upcolor->isChecked()){
                    QString strAttr = ui->tableWidget_product->item(row, 7)->text();
                    if (strAttr.size() > 1){
                        //clear;
                        QSqlQuery queryClearProdAttribute(QString("DELETE FROM rmrt_product_attribute WHERE rmrt_product_attribute.product_id = \'%1\' ")
                                                          .arg(id), db_server);
                        queryClearProdAttribute.exec();
                        for (int x = 0; x < strAttr.split(",").size(); x++){
                            if (!strAttr.split(",").at(x).isEmpty()){
                                QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                                "VALUES (?, ?, ?, ?)", db_server);
                                queryAddProdAttribute.bindValue(0, id);
                                queryAddProdAttribute.bindValue(1, IDcolor);
                                queryAddProdAttribute.bindValue(2, _LANG);
                                queryAddProdAttribute.bindValue(3, strAttr.split(",").at(x));
                                queryAddProdAttribute.exec();
                                if (queryAddProdAttribute.lastError().text().size() > 3){
                                  error.append(queryAddProdAttribute.lastError().text());
                                }
                            }
                        }
                    }
                }
                if (mainStr.size() > 0){
                    QSqlQuery queryUP(mainStr, db_server);
                    queryUP.exec();
                    if (queryUP.lastError().text().size() > 3){
                      error.append(queryUP.lastError().text());
                      break;
                    }
                }
            }
        }
        if (error.size() > 0){
            makeMessage(error, false);
        } else {
            makeMessage("Товары успешно загружены на сервер!", true);
            product_testServer();
        }
    }

}



void ocMeridia::makeMessage(const QString str, bool x)
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
