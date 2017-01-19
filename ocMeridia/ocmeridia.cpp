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
    ui->groupBox_f_maket->hide();
    connect(ui->toolButton_file, SIGNAL(clicked(bool)), this, SLOT(file_Select()));
    connect(ui->pushButton_fm_openFile, SIGNAL(clicked(bool)), this, SLOT(file_Open_PL()));
    connect(ui->pushButton_fm_load, SIGNAL(clicked(bool)), this, SLOT(file_loadMaket()));
    connect(ui->pushButton_fm_save, SIGNAL(clicked(bool)), this, SLOT(file_saveMaket()));


    //category
    //category_loadFromDB();
    connect(ui->toolButtoncat_par_clear, SIGNAL(clicked(bool)), ui->lineEdit_cat_active, SLOT(clear()));
    connect(ui->treeWidget_cat_serv, SIGNAL(clicked(QModelIndex)), this, SLOT(category_selectServer()));
    connect(ui->radioButton_cat_add, SIGNAL(clicked(bool)), this, SLOT(category_checkState()));
    connect(ui->radioButton_cat_equi, SIGNAL(clicked(bool)), this, SLOT(category_checkState()));
    connect(ui->pushButton_cat_save, SIGNAL(clicked(bool)), this, SLOT(category_save()));
    connect(ui->pushButton_cat_test, SIGNAL(clicked(bool)), this, SLOT(category_testServer()));
    connect(ui->toolButton_cat_update, SIGNAL(clicked(bool)), this, SLOT(category_loadFromDB()));

    //attribute
    connect(ui->radioButton_att_add, SIGNAL(clicked(bool)), this, SLOT(attribute_checkState()));
    connect(ui->radioButton_att_equi, SIGNAL(clicked(bool)), this, SLOT(attribute_checkState()));
    connect(ui->treeWidget_attr, SIGNAL(clicked(QModelIndex)), this, SLOT(attribute_selectServer()));
    connect(ui->pushButton_att_test, SIGNAL(clicked(bool)), this, SLOT(attribute_testServer()));
    connect(ui->pushButton_attr_save, SIGNAL(clicked(bool)), this, SLOT(attribute_save()));
    connect(ui->toolButton_att_update, SIGNAL(clicked(bool)), this, SLOT(attribute_loadFromDB()));


    //product
    ui->tableWidget_product->setColumnHidden(0, true);
    connect(ui->toolButton_prod_editor, SIGNAL(clicked(bool)), this, SLOT(product_openForm()));
    connect(ui->pushButton_loadOnServer, SIGNAL(clicked(bool)), this, SLOT(product_loadOnServer()));
    connect(ui->pushButton_prod_test, SIGNAL(clicked(bool)), this, SLOT(product_testServer()));

    //связи
    connect(ui->toolButton_cat_connect, SIGNAL(clicked(bool)), this, SLOT(local_connect()));
    connect(ui->toolButton_att_connect, SIGNAL(clicked(bool)), this, SLOT(local_connect()));

    //test
    //connect(ui->toolButton_test, SIGNAL(clicked(bool)), ui->toolButton_test, SLOT(hide()));
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
    ui->checkBox_sC_new_product->setChecked(sett.value("load1c/new_product").toBool());
    ui->checkBox_sC_not_price->setChecked(sett.value("load1c/not_price").toBool());
    ui->checkBox_sC_np_group->setChecked(sett.value("load1c/np_group").toBool());

    ui->checkBox_sC_upname->setChecked(sett.value("load1c/up_name").toBool());
    ui->checkBox_sC_upmanuf->setChecked(sett.value("load1c/up_manuf").toBool());
    ui->checkBox_sC_upprice->setChecked(sett.value("load1c/up_price").toBool());
    ui->checkBox_sC_upquan->setChecked(sett.value("load1c/up_quan").toBool());
    ui->checkBox_sC_updesc->setChecked(sett.value("load1c/up_desc").toBool());
    ui->checkBox_sC_upimage->setChecked(sett.value("load1c/up_image").toBool());
    ui->checkBox_sC_upattribute->setChecked(sett.value("load1c/up_attribute").toBool());



    ui->checkBox_sCpl_new_product->setChecked(sett.value("loadpl/new_product").toBool());
    ui->checkBox_sCpl_not_price->setChecked(sett.value("loadpl/not_price").toBool());
    ui->checkBox_sCpl_np_group->setChecked(sett.value("loadpl/np_group").toBool());

    ui->radioButton_sCpl_upall->setChecked(sett.value("loadpl/up_all").toBool());
    ui->radioButton_sCpl_upprice->setChecked(sett.value("loadpl/up_price").toBool());

    //auto
    ui->checkBox_autoActive->setChecked(sett.value("auto/active").toBool());



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
    sett.setValue("group", categoryMap.value(ui->comboBox_sC_group->currentIndex()));
    sett.setValue("attribute", attributeMap.value(ui->comboBox_sC_att->currentIndex()));

    sett.setValue("new_product", ui->checkBox_sC_new_product->isChecked());
    sett.setValue("not_price", ui->checkBox_sC_not_price->isChecked());
    sett.setValue("np_group", ui->checkBox_sC_np_group->isChecked());
    sett.setValue("np_group_id", categoryMap.value(ui->comboBox_sC_np_group->currentIndex()));


    sett.setValue("up_name", ui->checkBox_sC_upname->isChecked());
    sett.setValue("up_manuf", ui->checkBox_sC_upmanuf->isChecked());
    sett.setValue("up_price", ui->checkBox_sC_upprice->isChecked());
    sett.setValue("up_quan", ui->checkBox_sC_upquan->isChecked());
    sett.setValue("up_desc", ui->checkBox_sC_updesc->isChecked());
    sett.setValue("up_image", ui->checkBox_sC_upimage->isChecked());
    sett.setValue("up_attribute", ui->checkBox_sC_upattribute->isChecked());
    sett.endGroup();

    sett.beginGroup("loadpl");
    sett.setValue("group", categoryMap.value(ui->comboBox_sCpl_group->currentIndex()));
    sett.setValue("attribute", attributeMap.value(ui->comboBox_sCpl_att->currentIndex()));

    sett.setValue("new_product", ui->checkBox_sCpl_new_product->isChecked());
    sett.setValue("not_price", ui->checkBox_sCpl_not_price->isChecked());
    sett.setValue("np_group", ui->checkBox_sCpl_np_group->isChecked());
    sett.setValue("np_group_id", categoryMap.value(ui->comboBox_sCpl_np_group->currentIndex()));

    sett.setValue("up_all", ui->radioButton_sCpl_upall->isChecked());
    sett.endGroup();
    sett.beginGroup("auto");
    sett.setValue("1cdir", ui->lineEdit_sD_dir1c->text());
    sett.setValue("active", ui->checkBox_autoActive->isChecked());
    //sett.setValue("pldir", ui->lineEdit_sD_dirpl->text());
    sett.endGroup();

    makeMessage("Настройки сохранены", true);
}


void ocMeridia::setting_connect_netDB()
{
    if (!ui->lineEdit_sA_dbname->text().isEmpty()){
        db_server = QSqlDatabase::addDatabase("QMYSQL", "serverDB");
        db_server.setDatabaseName(ui->lineEdit_sA_dbname->text());
        db_server.setHostName(ui->lineEdit_sA_host->text());
        db_server.setPort(ui->spinBox_sA_port->value());
        db_server.setUserName(ui->lineEdit_sA_username->text());
        db_server.setPassword(ui->lineEdit_sA_pass->text());
        db_server.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE;");
        db_server.open();
    }


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
    /*
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
*/ //перенесено в category_loadFromDB();
    category_loadFromDB();
    attribute_loadFromDB();


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
        ui->groupBox_f_maket->hide();
    } else if (ui->radioButton_f_openPL->isChecked()){
        ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Open", "HOME", "CSV (*.csv)"));
        ui->groupBox_f_maket->setVisible(true);
    }

}

void ocMeridia::file_Open_PL()
{
    for (int r = ui->tableWidget_cat_file->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_cat_file->removeRow(r);
    }
    for (int r = ui->tableWidget_fileAttr->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_fileAttr->removeRow(r);
    }
    for (int r = ui->tableWidget_product->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_product->removeRow(r);
    }
    for (int r = ui->tableWidget_prod_attributes->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_prod_attributes->removeRow(r);
    }
    for (int r = ui->tableWidget_prod_image->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_prod_image->removeRow(r);
    }

    QFile file(ui->lineEdit_file->text());
    if (file.open(QIODevice::ReadOnly)){
        csvData.clear();
        while (!file.atEnd()){
            QByteArray ar = file.readLine();
            //QTextCodec *codec = QTextCodec::codecForName(ui.comboBox_csv_codec->currentText());
            //QTextCodec *codecA = QTextCodec::codecForName("UTF-8");
            //QString str;
            //str = codec->toUnicode(ar);
            //str = codecA->fromUnicode(str);
            csvData.append(ar);
        }
    }
    file.close();
    csv_readData();

    ui->tableWidget_cat_file->resizeColumnsToContents();
    ui->tableWidget_cat_file->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_product->resizeColumnsToContents();
    ui->tableWidget_product->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_attributes->resizeColumnsToContents();
    ui->tableWidget_prod_attributes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_image->resizeColumnsToContents();
    ui->tableWidget_prod_image->horizontalHeader()->setStretchLastSection(true);
    _test_manufacturer = false;

}


void ocMeridia::file_Open_Main()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    for (int r = ui->tableWidget_cat_file->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_cat_file->removeRow(r);
    }
    for (int r = ui->tableWidget_fileAttr->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_fileAttr->removeRow(r);
    }
    for (int r = ui->tableWidget_product->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_product->removeRow(r);
    }
    for (int r = ui->tableWidget_prod_attributes->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_prod_attributes->removeRow(r);
    }
    for (int r = ui->tableWidget_prod_image->rowCount() - 1; r >= 0; r--){
        ui->tableWidget_prod_image->removeRow(r);
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

        //attributes
        ui->tableWidget_fileAttr->insertRow(ui->tableWidget_fileAttr->rowCount());
        QTableWidgetItem *itemAtt = new QTableWidgetItem("Цвет");
        ui->tableWidget_fileAttr->setItem(0, 0, itemAtt);

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

        QStringList att_list;
        for (int y = 0; y < 5000; y++){; //*/nodeProducts.childNodes().count(); y++){ //****
            QDomNode nodeProduct = nodeProducts.childNodes().at(y);
            QString _ID = nodeProduct.firstChildElement("Ид").text();
            if (_ID.size() == 36){
                line.clear();
                line.append(QString("%0").arg(nodeProduct.firstChildElement("ПолноеНаименование").text()));//Наименование
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Группы").firstChildElement("Ид").text()));//Группа
                line.append(QString("||%0").arg(QString::number(mapPrice.value(_ID), 'f', 2)));//Цена
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Картинка").text()));//Картина
                line.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                //атрибут товара Цвет
                QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                for (int x = 0; x < elAttributes.childNodes().count(); x++){
                    QDomNode nAtt = elAttributes.childNodes().at(x);
                    if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                        QString color = nAtt.firstChildElement("Значение").text();
                        QString att_str = attributeProductMap.value(_ID);
                        if (att_str.isEmpty()){
                            attributeProductMap.insert(_ID, QString("Цвет|%3 ").arg(color));
                        } else {
                            att_str.append(" ").append(color);
                            attributeProductMap.remove(_ID);
                            attributeProductMap.insert(_ID, att_str);
                        }
                    }
                }
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
                        line_post.append(QString("||%0").arg(nodeProduct.firstChildElement("Описание").text()));//Описание
                        //атрибут товара Цвет
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                QString color = nAtt.firstChildElement("Значение").text();
                                QString att_str = attributeProductMap.value(_ID);
                                if (att_str.isEmpty()){
                                    attributeProductMap.insert(_ID, QString("Цвет|%3 ").arg(color));
                                } else {
                                    att_str.append(" ").append(color);
                                    attributeProductMap.remove(_ID);
                                    attributeProductMap.insert(_ID, att_str);
                                }
                            }
                        }
                        mapProduct.insert(_ID, line_post);
                    } else {
//                        //атрибут товара Цвет
                        QDomElement elAttributes = nodeProduct.firstChildElement("ЗначенияСвойств");
                        for (int x = 0; x < elAttributes.childNodes().count(); x++){
                            QDomNode nAtt = elAttributes.childNodes().at(x);
                            if (nAtt.firstChildElement("Ид").text() == "ae02b1ee-27c2-11e6-9800-df7e0845cb23"){ //Цвет
                                QString color = nAtt.firstChildElement("Значение").text();
                                QString att_str = attributeProductMap.value(_ID);
                                if (att_str.isEmpty()){
                                    attributeProductMap.insert(_ID, QString("Цвет|%3 ").arg(color));
                                } else {
                                    att_str.append(" ").append(color);
                                    attributeProductMap.remove(_ID);
                                    attributeProductMap.insert(_ID, att_str);
                                }
                            }
                        }
                    }
                }
            }

            ui->progressBar->setValue(qFloor((y+1) * 100 / 5000)); //*/nodeProducts.childNodes().count()));
            QApplication::processEvents();
        }
        for (int x = 0; x < mapProduct.count(); x++){
            ui->tableWidget_product->insertRow(x);

            QTableWidgetItem *item0 = new QTableWidgetItem("0");
            ui->tableWidget_product->setItem(x, 0, item0);

            QTableWidgetItem *itemArticul = new QTableWidgetItem(mapProduct.keys().at(x));
            ui->tableWidget_product->setItem(x, 1, itemArticul);

            QTableWidgetItem *item2 = new QTableWidgetItem("model");
            ui->tableWidget_product->setItem(x, 2, item2);

            QString str = mapProduct.value(mapProduct.keys().at(x));

            QTableWidgetItem *itemName = new QTableWidgetItem(str.split("||").at(0));
            ui->tableWidget_product->setItem(x, 3, itemName);
            QTableWidgetItem *itemGroup = new QTableWidgetItem(str.split("||").at(1));
            ui->tableWidget_product->setItem(x, 4, itemGroup);
            QTableWidgetItem *item5 = new QTableWidgetItem("no");
            ui->tableWidget_product->setItem(x, 5, item5);
            QTableWidgetItem *itemPrice = new QTableWidgetItem(str.split("||").at(2));
            ui->tableWidget_product->setItem(x, 6, itemPrice);
            QTableWidgetItem *item7 = new QTableWidgetItem("0.00");
            ui->tableWidget_product->setItem(x, 7, item7);
            QTableWidgetItem *itemDesc = new QTableWidgetItem(str.split("||").at(4));
            ui->tableWidget_product->setItem(x, 8, itemDesc);
            QTableWidgetItem *itemImage = new QTableWidgetItem(str.split("||").at(3));
            ui->tableWidget_product->setItem(x, 9, itemImage);

            ui->progressBar->setValue(qFloor((x+1) * 100 / mapProduct.count()));
            QApplication::processEvents();
        }
       for (int row = 0; row < attributeProductMap.count(); row++){
            ui->tableWidget_prod_attributes->insertRow(row);

            QString key = attributeProductMap.keys().at(row);

            QTableWidgetItem *itemID = new QTableWidgetItem(key);
            ui->tableWidget_prod_attributes->setItem(row, 0, itemID);

            for (int col = 0; col < attributeProductMap.value(key).split("|").size(); col++){

                QTableWidgetItem *item = new QTableWidgetItem( attributeProductMap.value(key).split("|").at(col));
                ui->tableWidget_prod_attributes->setItem(row, col + 1, item);
            }
        }
    }

    file.close();


    ui->tabWidget_main->setCurrentWidget(ui->tab_category);

    ui->tableWidget_cat_file->resizeColumnsToContents();
    ui->tableWidget_cat_file->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_product->resizeColumnsToContents();
    ui->tableWidget_product->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_attributes->resizeColumnsToContents();
    ui->tableWidget_prod_attributes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_image->resizeColumnsToContents();
    ui->tableWidget_prod_image->horizontalHeader()->setStretchLastSection(true);
    ui->progressBar->hide();

    _test_manufacturer = false;

}

void ocMeridia::file_loadMaket()
{
    ui->lineEdit_fm_maket->setText(QFileDialog::getOpenFileName(this, "Open", "HOME/makets", "ocMeridia Maket (*.ini)"));
    if (!ui->lineEdit_fm_maket->text().isEmpty()){
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
        ui->checkBox_fm_price_corr->setChecked(maket.value("price_corr_check").toBool());

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
        ui->spinBox_fm_price_cor->setValue(maket.value("price_corr").toInt());
    } else {
        ui->comboBox_f_csv_split->setCurrentIndex(0);
        ui->spinBox_f_csv_firstRow->setValue(1);
        ui->checkBox_fm_category->setChecked(false);
        ui->checkBox_fm_articul->setChecked(false);
        ui->checkBox_fm_model->setChecked(false);
        ui->checkBox_fm_name->setChecked(false);
        ui->checkBox_fm_manuf->setChecked(false);
        ui->checkBox_fm_price->setChecked(false);
        ui->checkBox_fm_quan->setChecked(false);
        ui->checkBox_fm_desc->setChecked(false);
        ui->checkBox_fm_image->setChecked(false);
        ui->checkBox_fm_attr->setChecked(false);
        ui->checkBox_fm_price_corr->setChecked(false);
        ui->lineEdit_fm_category->clear();
        ui->lineEdit_fm_desc->clear();
        ui->lineEdit_fm_image->clear();
        ui->lineEdit_fm_attr->clear();
        ui->spinBox_fm_articul->setValue(1);
        ui->spinBox_fm_model->setValue(1);
        ui->spinBox_fm_name->setValue(1);
        ui->spinBox_fm_manuf->setValue(1);
        ui->spinBox_fm_price->setValue(1);
        ui->spinBox_fm_quan->setValue(1);
        ui->spinBox_fm_price_cor->setValue(0);
    }
}

void ocMeridia::file_saveMaket()
{
    if (ui->lineEdit_fm_maket->text().isEmpty()){
        ui->lineEdit_fm_maket->setText(QFileDialog::getSaveFileName(this, "Open", "HOME/makets/maket.ini", "ocMeridia Maket (*.ini)"));
    }
    QSettings maket(ui->lineEdit_fm_maket->text(), QSettings::IniFormat);
    maket.setValue("split", ui->comboBox_f_csv_split->currentText());
    maket.setValue("firstRow", ui->spinBox_f_csv_firstRow->value());
    maket.setValue("cat_check", ui->checkBox_fm_category->isChecked());
    maket.setValue("art_check", ui->checkBox_fm_articul->isChecked());
    maket.setValue("model_check", ui->checkBox_fm_model->isChecked());
    maket.setValue("name_check", ui->checkBox_fm_name->isChecked());
    maket.setValue("manuf_check", ui->checkBox_fm_manuf->isChecked());
    maket.setValue("price_check", ui->checkBox_fm_price->isChecked());
    maket.setValue("quan_check", ui->checkBox_fm_quan->isChecked());
    maket.setValue("desc_check", ui->checkBox_fm_desc->isChecked());
    maket.setValue("image_check", ui->checkBox_fm_image->isChecked());
    maket.setValue("attr_check", ui->checkBox_fm_attr->isChecked());
    maket.setValue("price_corr_check", ui->checkBox_fm_price_corr->isChecked());

    maket.setValue("cat", ui->lineEdit_fm_category->text());
    maket.setValue("desc", ui->lineEdit_fm_desc->text());
    maket.setValue("image", ui->lineEdit_fm_image->text());
    maket.setValue("attr", ui->lineEdit_fm_attr->text());
    maket.setValue("art", ui->spinBox_fm_articul->value());
    maket.setValue("model", ui->spinBox_fm_model->value());
    maket.setValue("name", ui->spinBox_fm_name->value());
    maket.setValue("manuf", ui->spinBox_fm_manuf->value());
    maket.setValue("price", ui->spinBox_fm_price->value());
    maket.setValue("quan", ui->spinBox_fm_quan->value());
    maket.setValue("price_corr", ui->spinBox_fm_price_cor->value());
}
//***************************************************
//------------------CSV-------------------------------
void ocMeridia::csv_readData()
{
    if (csvData.size() > 0){

        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);

        QString lineA = "";
        QString splA = ui->comboBox_f_csv_split->currentText();

        QList<int> column_list_group;
        for (int z = 0; z < ui->lineEdit_fm_category->text().split("|").size(); z++){
            column_list_group.append(ui->lineEdit_fm_category->text().split("|").at(z).toInt() - 1);
        }

        QList<int> column_list_image; // номера колонок изображений
        for (int z = 0; z < ui->lineEdit_fm_image->text().split("|").size(); z++){
            column_list_image.append(ui->lineEdit_fm_image->text().split("|").at(z).toInt() - 1);
        }
        QList<int> column_list_attr; // номера колонок аттрибутов
        for (int z = 0; z < ui->lineEdit_fm_attr->text().split("|").size(); z++){
            column_list_attr.append(ui->lineEdit_fm_attr->text().split("|").at(z).toInt() - 1);
        }


        // --category
        QStringList _categoryList;
        QString group_line;
        for (int x = 0; x < csvData.size(); x++){
            lineA.clear();
            lineA = csvData.at(x);

            group_line.clear();
            group_line.append(lineA.split(splA).at(column_list_group.at(0)));
            if (column_list_group.size() > 1){
                for (int a = 1; a < column_list_group.size(); a++){
                    group_line.append(QString("|%1").arg(lineA.split(splA).at(column_list_group.at(a))));
                }
            }
            _categoryList.append(group_line);

        }
        _categoryList.removeDuplicates();
        for (int row = 0; row < _categoryList.size(); row++){
            ui->tableWidget_cat_file->insertRow(row);
            QTableWidgetItem *itemA = new QTableWidgetItem(_categoryList.at(row));
            ui->tableWidget_cat_file->setItem(row, 0, itemA);
            QTableWidgetItem *itemB = new QTableWidgetItem(_categoryList.at(row).split("|")
                                                           .at(_categoryList.at(row).split("|").size() - 1));
            ui->tableWidget_cat_file->setItem(row, 1, itemB);

            ui->progressBar->setValue(qFloor((row+1) * 100 / _categoryList.size()));
            QApplication::processEvents();
        }
        //удаление первых строк
        for (int r = ui->spinBox_f_csv_firstRow->value() - 2; r >= 0; r--){
            ui->tableWidget_cat_file->removeRow(r);
        }
        ui->progressBar->setValue(0);
        //****************************************************
        //------------attributes-----------------
        QStringList _attributeList;
        if (ui->checkBox_fm_attr->isChecked()){
            lineA.clear();
            lineA = csvData.at(ui->spinBox_f_csv_firstRow->value() - 2); //строка данных c заголовками

            for (int a = 0; a < column_list_attr.size(); a++){
                _attributeList.append(lineA.split(splA).at(column_list_attr.at(a)));
            }
            for (int row = 0; row < _attributeList.size(); row++){
                ui->tableWidget_fileAttr->insertRow(row);
                QTableWidgetItem *item = new QTableWidgetItem(_attributeList.at(row));
                ui->tableWidget_fileAttr->setItem(row, 0, item);
                ui->progressBar->setValue(qFloor((row+1) * 100 / _attributeList.size()));
                QApplication::processEvents();
            }
        }
        ui->progressBar->setValue(0);
        //*****************************************************
        //------------products---------------------------------
        QStringList _productList;
        //2- собираем товары
        QString product_line;

        //собираем данные
        for (int x = 0; x < csvData.size(); x++){
            lineA.clear();
            lineA = csvData.at(x);

            //variable
            product_line.clear();
            QString _art = "";
            QString _model = "model";
            QString _name = "";
            QString _cat = "";
            QString _price = "";
            QString _quan = "0.00";
            QString _desc = "";
            QString _manuf = "";
            QString _image = "";



            if (ui->checkBox_fm_articul->isChecked()){
                _art.append(lineA.split(splA).at(ui->spinBox_fm_articul->value() - 1));
            }
            product_line.append(QString("||%1").arg(_art));
            if (ui->checkBox_fm_model->isChecked()){
                _model.clear();
                _model.append(lineA.split(splA).at(ui->spinBox_fm_model->value() - 1));
            }
            product_line.append(QString("||%1").arg(_model));

            if (ui->checkBox_fm_name->isChecked()){
                _name.append(lineA.split(splA).at(ui->spinBox_fm_name->value() - 1));
            }
            product_line.append(QString("||%1").arg(_name));

            if (ui->checkBox_fm_category->isChecked()){
                _cat = lineA.split(splA).at(ui->lineEdit_fm_category->text().split("|").at(0).toInt() - 1);
                if (ui->lineEdit_fm_category->text().split("|").size() > 1){
                    for (int xc = 1; xc < ui->lineEdit_fm_category->text().split("|").size(); xc++){
                        _cat.append(QString("|%1").arg(lineA.split(splA).at(ui->lineEdit_fm_category->text().split("|").at(xc).toInt() - 1)));
                    }
                }
            }
            product_line.append(QString("||%1").arg(_cat));

            if (ui->checkBox_fm_manuf->isChecked()){
                _manuf.append(lineA.split(splA).at(ui->spinBox_fm_manuf->value() - 1));
            }
            product_line.append(QString("||%1").arg(_manuf));

            if (ui->checkBox_fm_price->isChecked()){
                _price.append(lineA.split(splA).at(ui->spinBox_fm_price->value() - 1));
                if (ui->checkBox_fm_price_corr->isChecked()){
                    _price.clear();
                    double _pr = lineA.split(splA).at(ui->spinBox_fm_price->value() - 1).toDouble();
                    _price.append(QString::number(_pr + (_pr * (ui->spinBox_fm_price_cor->value() * 0.01)), 'f', 2));
                }

            }
            product_line.append(QString("||%1").arg(_price));

            if (ui->checkBox_fm_quan->isChecked()){
                _quan.append(lineA.split(splA).at(ui->spinBox_fm_quan->value() - 1));
            }
            product_line.append(QString("||%1").arg(_quan));

            if (ui->checkBox_fm_desc->isChecked()){
                for (int xd = 0; xd < ui->lineEdit_fm_desc->text().split("|").size(); xd++){
                    _desc.append(lineA.split(splA).at(ui->lineEdit_fm_desc->text().split("|").at(xd).toInt() - 1));
                }
            }
            product_line.append(QString("||%1").arg(_desc));

            if (ui->checkBox_fm_image->isChecked()){
                // первую позицию в таблицу товаров
                _image = lineA.split(splA).at(column_list_image.at(0));

                // остальные в таблицу изображений
                for (int xi = 1; xi < column_list_image.size(); xi++){
                    int numRow = x - ui->spinBox_f_csv_firstRow->value() + 1; //номер строки связанного товара
                    int row = ui->tableWidget_prod_image->rowCount();
                    ui->tableWidget_prod_image->insertRow(row);
                    QTableWidgetItem *itemA = new QTableWidgetItem(QString::number(numRow));
                    ui->tableWidget_prod_image->setItem(row, 0, itemA);

                    QTableWidgetItem *itemB = new QTableWidgetItem(lineA.split(splA).at(column_list_image.at(xi)));
                    ui->tableWidget_prod_image->setItem(row, 1, itemB);
                }
            }
            product_line.append(QString("||%1").arg(_image));

            if (ui->checkBox_fm_attr->isChecked()){

                // остальные в таблицу изображений
                for (int xa = 0; xa < column_list_attr.size(); xa++){
                    int numRow = x - ui->spinBox_f_csv_firstRow->value() + 1; //номер строки связанного товара
                    int row = ui->tableWidget_prod_attributes->rowCount();
                    ui->tableWidget_prod_attributes->insertRow(row);
                    QTableWidgetItem *itemA = new QTableWidgetItem(QString::number(numRow));
                    ui->tableWidget_prod_attributes->setItem(row, 0, itemA);
                    QTableWidgetItem *itemC = new QTableWidgetItem(_attributeList.at(xa));
                    ui->tableWidget_prod_attributes->setItem(row, 1, itemC);
                    QTableWidgetItem *itemB = new QTableWidgetItem(lineA.split(splA).at(column_list_attr.at(xa)));
                    ui->tableWidget_prod_attributes->setItem(row, 2, itemB);
                }
            }
            _productList.append(product_line);
            ui->progressBar->setValue(qFloor((x+1) * 100 / csvData.size()));
            QApplication::processEvents();
        }
        ui->progressBar->setValue(0);
        //размещаем
        for (int row = 0; row < _productList.size(); row++){
            ui->tableWidget_product->insertRow(row);
            for (int col = 0; col < 10; col++){
                QTableWidgetItem *item = new QTableWidgetItem(_productList.at(row).split("||").at(col));
                ui->tableWidget_product->setItem(row, col, item);
            }
            ui->progressBar->setValue(qFloor((row+1) * 100 / _productList.size()));
            QApplication::processEvents();
        }
        //удаление первых строк товаров
        for (int r = ui->spinBox_f_csv_firstRow->value() - 2; r >= 0; r--){
            ui->tableWidget_product->removeRow(r);
        }
        //удаление первых строк изображений

        int numI = (ui->spinBox_f_csv_firstRow->value() - 1) * (column_list_image.size() - 1);
        for (int r = numI - 1; r >= 0; r--){
            ui->tableWidget_prod_image->removeRow(r);
        }
        //удаление первых строк аттрибутов

        int numA = (ui->spinBox_f_csv_firstRow->value() - 1) * column_list_attr.size();
        for (int r = numA - 1; r >= 0; r--){
            ui->tableWidget_prod_attributes->removeRow(r);
        }
    }

    //all_test();

    ui->progressBar->setValue(0);
    ui->progressBar->hide();
}


//****************************************************
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


    //load category maps
    ui->comboBox_sCpl_group->clear();
    ui->comboBox_sC_group->clear();
    ui->comboBox_prod_group->clear();
    ui->comboBox_sC_np_group->clear();
    ui->comboBox_sCpl_np_group->clear();
    categoryMap.clear();
    QSqlQuery qCategory(QString("SELECT rmrt_category_description.category_id, rmrt_category_description.name "
                                "FROM rmrt_category_description "
                                "WHERE rmrt_category_description.language_id = \'%1\' ").arg(_LANG), db_server);
    while (qCategory.next()){
        ui->comboBox_sC_group->addItem(qCategory.value(1).toString());
        ui->comboBox_sCpl_group->addItem(qCategory.value(1).toString());
        ui->comboBox_prod_group->addItem(qCategory.value(1).toString());
        ui->comboBox_sCpl_np_group->addItem(qCategory.value(1).toString());
        ui->comboBox_sC_np_group->addItem(qCategory.value(1).toString());
        categoryMap.insert(ui->comboBox_sC_group->count() - 1, qCategory.value(0).toInt());
    }
    QSettings sett("setting.ini", QSettings::IniFormat);
    ui->comboBox_sC_group->setCurrentIndex(categoryMap.key(sett.value("load1c/group").toInt()));
    ui->comboBox_sCpl_group->setCurrentIndex(categoryMap.key(sett.value("loadpl/group").toInt()));
    ui->comboBox_sC_np_group->setCurrentIndex(categoryMap.key(sett.value("load1c/np_group_id").toInt()));
    ui->comboBox_sCpl_np_group->setCurrentIndex(categoryMap.key(sett.value("loadpl/np_group_id").toInt()));


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
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
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
                    QString productG1C = ui->tableWidget_product->item(rowp, 4)->text();
                    QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                    if (productG1C == group1C){
                        QTableWidgetItem *item = new QTableWidgetItem(queryS.value(0).toString());
                        ui->tableWidget_product->setItem(rowp, 4, item);
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
                    qDebug() << queryU.lastError();
                    // меняем в таблице товаров код1С на ID групп магазина
                    for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                        QString productG1C = ui->tableWidget_product->item(rowp, 4)->text();
                        QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                        if (productG1C == group1C){
                            QTableWidgetItem *item = new QTableWidgetItem(queryS2.value(0).toString());
                            ui->tableWidget_product->setItem(rowp, 4, item);
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
                            QString productG1C = ui->tableWidget_product->item(rowp, 4)->text();
                            QString group1C = ui->tableWidget_cat_file->item(row, 0)->text();
                            if (productG1C == group1C){
                                QTableWidgetItem *item = new QTableWidgetItem(queryL.value(0).toString());
                                ui->tableWidget_product->setItem(rowp, 4, item);
                            }
                        }
                        // cтроки для удаления
                        del_list.append(row);
                    } else {
                    //оставляем в ручную обработку
                    }
                }
            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_cat_file->rowCount()));
            QApplication::processEvents();
        }
        if (del_list.size() > 0){
            for (int x = del_list.size() - 1; x >= 0; x--){
                ui->tableWidget_cat_file->removeRow(del_list.at(x));
            }
        }
        // если обработка полностью закончена переходим к атрибутам
        if (ui->tableWidget_cat_file->rowCount() == 0){
            ui->tabWidget_main->setCurrentWidget(ui->tab_attribute);
            makeMessage("Все категории определены!", true);
        }
    }
    ui->progressBar->setValue(0);
}

void ocMeridia::category_save()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
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
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_cat_file->rowCount()));
            QApplication::processEvents();
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
    ui->progressBar->hide();
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

void ocMeridia::attribute_loadFromDB()
{
    attributeMap.clear();
    ui->comboBox_sCpl_att->clear();
    ui->comboBox_sC_att->clear();
    ui->treeWidget_attr->clear();
    QSqlQuery queryGroup(QString("SELECT rmrt_attribute_group_description.name, "
                         "rmrt_attribute_group_description.attribute_group_id "
                         "FROM rmrt_attribute_group_description "
                         "WHERE rmrt_attribute_group_description.language_id = \'%1\' ")
                         .arg(_LANG), db_server);
    while (queryGroup.next()){

        attributeMap.insert(ui->comboBox_sCpl_att->count(), queryGroup.value(1).toInt());
        ui->comboBox_sCpl_att->addItem(queryGroup.value(0).toString());
        ui->comboBox_sC_att->addItem(queryGroup.value(0).toString());

        QTreeWidgetItem *itemA = new QTreeWidgetItem();
        itemA->setText(0, queryGroup.value(0).toString());
        itemA->setText(1, queryGroup.value(1).toString());
        itemA->setText(2, "");
        ui->treeWidget_attr->addTopLevelItem(itemA);
        QSqlQuery queryAttribute (QString("SELECT rmrt_attribute_description.name, "
                                          "rmrt_attribute_description.attribute_id "
                                          "FROM rmrt_attribute_description "
                                          "INNER JOIN rmrt_attribute ON rmrt_attribute_description.attribute_id = rmrt_attribute.attribute_id "
                                          "WHERE rmrt_attribute_description.language_id = \'%1\' "
                                          "AND rmrt_attribute.attribute_group_id = \'%2\' ")
                                  .arg(_LANG)
                                  .arg(queryGroup.value(1).toString()), db_server);
        while (queryAttribute.next()){
            QTreeWidgetItem *itemB = new QTreeWidgetItem();
            itemB->setText(0, queryAttribute.value(0).toString());
            itemB->setText(1, "");
            itemB->setText(2, queryAttribute.value(1).toString());
            itemA->addChild(itemB);
        }
    }
    ui->treeWidget_attr->resizeColumnToContents(0);
    ui->treeWidget_attr->setColumnHidden(1, true);
    ui->treeWidget_attr->setColumnHidden(2, true);

    QSettings sett("setting.ini", QSettings::IniFormat);
    ui->comboBox_sCpl_att->setCurrentIndex(attributeMap.key(sett.value("loadpl/attribute").toInt()));
    ui->comboBox_sC_att->setCurrentIndex(attributeMap.key(sett.value("load1c/attribute").toInt()));
}

void ocMeridia::attribute_checkState()
{
    if (ui->radioButton_att_add->isChecked()){
        ui->tableWidget_fileAttr->setSelectionMode(QTableWidget::ExtendedSelection);
        ui->label_attr_active->setText("Группа атрибутов:");
    } else if (ui->radioButton_att_equi->isChecked()){
        ui->tableWidget_fileAttr->setSelectionMode(QTableWidget::SingleSelection);
        ui->label_attr_active->setText("Атрибут:");
    }
    ui->lineEdit_attr_active->clear();
    att_activeID = 0;
}

void ocMeridia::attribute_selectServer()
{
    QString _id;
    if (ui->radioButton_att_add->isChecked()){
        _id = ui->treeWidget_attr->currentItem()->text(1);
        if (_id.isEmpty()){
            makeMessage("Выберите группу!", false);
            ui->lineEdit_attr_active->clear();
            att_activeID = 0;
        } else {
            ui->lineEdit_attr_active->setText(ui->treeWidget_attr->currentItem()->text(0));
            att_activeID = _id.toInt();
        }
    } else if (ui->radioButton_att_equi->isChecked()){
        _id = ui->treeWidget_attr->currentItem()->text(2);
        if (_id.isEmpty()){
            makeMessage("Выберите атрибут!", false);
            ui->lineEdit_attr_active->clear();
            att_activeID = 0;
        } else {
            ui->lineEdit_attr_active->setText(ui->treeWidget_attr->currentItem()->text(0));
            att_activeID = _id.toInt();
        }
    }
}

void ocMeridia::attribute_save()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    QString error;
    if (ui->radioButton_att_add->isChecked()){
        for (int row = 0; row < ui->tableWidget_fileAttr->rowCount(); row++){
            if (ui->tableWidget_fileAttr->item(row, 0)->isSelected()){
                QString att_value = ui->tableWidget_fileAttr->item(row, 0)->text();
                QSqlQuery queryA("INSERT INTO rmrt_attribute (attribute_group_id, sort_order) "
                                "VALUES (?, ?)", db_server);
                queryA.bindValue(0, att_activeID);
                queryA.bindValue(1, 0);
                queryA.exec();
                if (queryA.lastError().text().size() > 3){
                    error.append(queryA.lastError().text());
                    break;
                } else {
                    int att_id = queryA.lastInsertId().toInt();
                    QSqlQuery queryB("INSERT INTO rmrt_attribute_description (attribute_id, language_id, name) "
                                     "VALUES (?, ?, ?)", db_server);
                    queryB.bindValue(0, att_id);
                    queryB.bindValue(1, _LANG);
                    queryB.bindValue(2, att_value);
                    queryB.exec();
                    if (queryB.lastError().text().size() > 3){
                        error.append(queryB.lastError().text());
                    }
                }
            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_fileAttr->rowCount()));
            QApplication::processEvents();
        }
    } else if (ui->radioButton_att_equi->isChecked()){
        if (ui->lineEdit_attr_active->text().isEmpty()){
            error.append("Выберите атрибут магазина, к которому приравниваете атрибут из прайса");
        } else {
            // синоним добавляем в локальную БД
            int crow = ui->tableWidget_fileAttr->currentRow();

            QSqlQuery query("INSERT INTO `att` (`name`, `id_db`) VALUES (?, ?)", db_local);
            query.bindValue(0, ui->tableWidget_cat_file->item(crow, 0)->text());
            query.bindValue(1, att_activeID);
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
        attribute_loadFromDB();
        attribute_testServer();
    }
    ui->progressBar->hide();
}

void ocMeridia::attribute_testServer()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    if (ui->tableWidget_fileAttr->rowCount() > 0){
        QList<int> del_list;
        for (int row = 0; row < ui->tableWidget_fileAttr->rowCount(); row++){
            int id_attribute = 0;
            QString str_attribute = ui->tableWidget_fileAttr->item(row, 0)->text();
            QSqlQuery query(QString("SELECT rmrt_attribute_description.attribute_id "
                                    "FROM rmrt_attribute_description "
                                    "WHERE rmrt_attribute_description.name = \'%1\' ")
                            .arg(str_attribute), db_server);
            query.next();
            if (query.isValid()){
                id_attribute = query.value(0).toInt();
                del_list.append(row);
            } else {
                QSqlQuery queryL(QString("SELECT att.id_db FROM att WHERE att.name = \'%1\' ")
                                 .arg(str_attribute), db_local);
                queryL.next();
                if (queryL.isValid()){
                    id_attribute = queryL.value(0).toInt();
                    del_list.append(row);
                }
            }
            if (id_attribute > 0){
                for (int rowA = 0; rowA < ui->tableWidget_prod_attributes->rowCount(); rowA++){
                    if (str_attribute == ui->tableWidget_prod_attributes->item(rowA, 1)->text()){
                        QTableWidgetItem *item = new QTableWidgetItem(QString::number(id_attribute));
                        ui->tableWidget_prod_attributes->setItem(rowA, 3, item);
                    }
                }
            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_fileAttr->rowCount()));
            QApplication::processEvents();
        }
        if (del_list.size() > 0){
            for (int x = del_list.size() - 1; x >= 0; x--){
                ui->tableWidget_fileAttr->removeRow(del_list.at(x));
            }
        }
    }
    if (ui->tableWidget_fileAttr->rowCount() == 0){
        ui->tabWidget_main->setCurrentWidget(ui->tab_product);
        makeMessage("Все атрибуты определены!", true);
    }
    ui->progressBar->hide();

    //--test
//    QList<int> del_row;
//    for (int row = 1; row < ui->tableWidget_prod_attributes->rowCount(); row++){
//        QString A1 = ui->tableWidget_prod_attributes->item(row, 0)->text();
//        QString A0 = ui->tableWidget_prod_attributes->item(row - 1, 0)->text();
//        QString B1 = ui->tableWidget_prod_attributes->item(row, 1)->text();
//        QString B0 = ui->tableWidget_prod_attributes->item(row - 1, 1)->text();
//        QString C1 = ui->tableWidget_prod_attributes->item(row, 2)->text();
//        QString C0 = ui->tableWidget_prod_attributes->item(row - 1, 2)->text();
//        if (A1 == A0 && B1 == B0){
//            QTableWidgetItem *itemC = new QTableWidgetItem(QString("%1 %2").arg(C0).arg(C1));
//            ui->tableWidget_prod_attributes->setItem(row, 2, itemC);
//            del_row.append(row - 1);
//        }
//    }
//    if (del_row.size() > 0){
//        for (int x = del_row.size()-1; x >= 0; x--){
//            ui->tableWidget_prod_attributes->removeRow(del_row.at(x));
//        }
//    }

}

void ocMeridia::product_testServer()
{
    if (ui->tableWidget_product->rowCount() > 0){

        product_manufacturer();

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
                                  .arg(ui->tableWidget_product->item(row, 3)->text())
                                  .arg(_LANG), db_server);
                queryS2.next();
                if (queryS2.isValid()){
                    //если есть  - ставим код 1С + ставим ИД
                    if (ui->tableWidget_product->item(row, 1)->text().size() == 36){ //если в исходнике есть код 1с добавляем его к товару
                        QSqlQuery queryU(QString("UPDATE rmrt_product "
                                                 "SET sku = \'%0\' "
                                                 "WHERE rmrt_product.product_id = \'%1\' ")
                                         .arg(ui->tableWidget_product->item(row, 1)->text())
                                         .arg(queryS2.value(0).toInt()), db_server);
                        queryU.exec();
                    }
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
            QApplication::processEvents();
        }
        ui->progressBar->hide();
        //замена категорий новых товаров на указанную в настройках
        if (ui->checkBox_sC_np_group->isChecked()){
            product_change_group();
        }
    }
}

void ocMeridia::product_openForm()
{
    if (ui->tableWidget_product->rowCount() > 0){
        QMap<QString, QString> loadProduct;
        ui->progressBar->setValue(0);
        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            if (ui->tableWidget_product->item(row, 0)->text().toInt() == 0){

                loadProduct.insert(ui->tableWidget_product->item(row, 1)->text(), //артикул
                                   ui->tableWidget_product->item(row, 3)->text()); // наименование

            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
            QApplication::processEvents();
        }
        ui->progressBar->hide();
        if (loadProduct.count() > 0){
            product_form *pf = new product_form(_LANG, _STORE, db_server, loadProduct, this);
            pf->exec();
            product_testServer();
        }
    }
}

void ocMeridia::product_loadOnServer()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    if (ui->tableWidget_product->rowCount() > 0){
        QString error;

        bool idxIN;
        bool idxUP;

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            idxIN = false;
            idxUP = false;
            int _id = ui->tableWidget_product->item(row, 0)->text().toInt();

            QString _art = "";
            if (ui->tableWidget_product->item(row, 1)->text().size() > 0){
                _art.append(ui->tableWidget_product->item(row, 1)->text());
            }

            QString _model = "";
            if (ui->tableWidget_product->item(row, 2)->text().size() > 0){
                _model.append(ui->tableWidget_product->item(row, 2)->text());
            }

            QString _name = "";
            if (ui->tableWidget_product->item(row, 3)->text().size() > 0){
                _name.append(ui->tableWidget_product->item(row, 3)->text());
            }

            int _group_id = ui->tableWidget_product->item(row, 4)->text().toInt();
            int _manuf_id = ui->tableWidget_product->item(row, 5)->text().toInt();
            double _price = ui->tableWidget_product->item(row, 6)->text().toDouble();
            double _quan = 0.00;
            if (ui->tableWidget_product->item(row, 7)->text().toDouble() == 0.00){
                _quan = ui->doubleSpinBox_sB_quan->value();
            }
            QString _desc = "";
            if (ui->tableWidget_product->item(row, 8)->text().size() > 0){
                _desc.append(ui->tableWidget_product->item(row, 8)->text());
            } else {
                _desc.append(_name);
            }

            QString _image = "";
            if (ui->tableWidget_product->item(row, 9)->text().size() > 0){
                _image.append(QString("%1/%2").arg(_DIR).arg(ui->tableWidget_product->item(row, 9)->text()));
            }

            if (ui->radioButton_prod_onlyGroup->isChecked()){
                int target_group = categoryMap.value(ui->comboBox_prod_group->currentIndex());
                if (_group_id == target_group){
                    if (ui->radioButton_f_openMain->isChecked()){ // загрузка основного файла
                        if (_id == 0){
                            idxIN = true;
                            idxUP = false;
                            //add?
                            if (ui->checkBox_sC_new_product->isChecked()){
                                idxIN = true;
                                // price?
                                if (_price == 0.00){
                                    if (ui->checkBox_sC_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                        idxIN = true;
                                    } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                        idxIN = false;
                                    }
                                }
                            } else {
                                idxIN = false;
                            }
                        } else if (_id > 0){
                            idxUP = true;
                            idxIN = false;

                            if (_price == 0.00){
                                if (ui->checkBox_sC_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                    idxUP = true;
                                } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                    idxUP = false;
                                }
                            }
                        }
                    } else if (ui->radioButton_f_openPL->isChecked()){
                        if (_id == 0){
                            idxIN = true;
                            idxUP = false;
                            //add?
                            if (ui->checkBox_sCpl_new_product->isChecked()){
                                idxIN = true;
                                // price?
                                if (_price == 0.00){
                                    if (ui->checkBox_sCpl_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                        idxIN = true;
                                    } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                        idxIN = false;
                                    }
                                }
                            } else {
                                idxIN = false;
                            }
                        } else if (_id > 0){
                            idxUP = true;
                            idxIN = false;

                            if (_price == 0.00){
                                if (ui->checkBox_sCpl_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                    idxUP = true;
                                } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                    idxUP = false;
                                }
                            }
                        }
                    }
                }
            } else if (ui->radioButton_prod_all->isChecked()){
                if (ui->radioButton_f_openMain->isChecked()){
                    if (_id == 0){
                        idxIN = true;
                        idxUP = false;
                        //add?
                        if (ui->checkBox_sC_new_product->isChecked()){
                            idxIN = true;
                            // price?
                            if (_price == 0.00){
                                if (ui->checkBox_sC_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                    idxIN = true;
                                } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                    idxIN = false;
                                }
                            }
                        } else {
                            idxIN = false;
                        }
                    } else if (_id > 0){
                        idxUP = true;
                        idxIN = false;

                        if (_price == 0.00){
                            if (ui->checkBox_sC_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                idxUP = true;
                            } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                idxUP = false;
                            }
                        }
                    }
                } else if (ui->radioButton_f_openPL->isChecked()){
                    if (_id == 0){
                        idxIN = true;
                        idxUP = false;
                        //add?
                        if (ui->checkBox_sCpl_new_product->isChecked()){
                            idxIN = true;
                            // price?
                            if (_price == 0.00){
                                if (ui->checkBox_sCpl_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                    idxIN = true;
                                } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                    idxIN = false;
                                }
                            }
                        } else {
                            idxIN = false;
                        }
                    } else if (_id > 0){
                        idxUP = true;
                        idxIN = false;

                        if (_price == 0.00){
                            if (ui->checkBox_sCpl_not_price->isChecked()){ // если в настройках стоит пропускать с 0 ценой
                                idxUP = true;
                            } else { // если в настройках стоит товары с 0 ценой добавлять с ценой Х
                                idxUP = false;
                            }
                        }
                    }
                }
            }
            if (idxIN){
//                if (!ui->tableWidget_product->item(row, 5)->text().isEmpty()){
//                    image = _DIR.append("/").append(ui->tableWidget_product->item(row, 5)->text());
//                }

                QSqlQuery queryAddProduct("INSERT INTO rmrt_product (sku, model, manufacturer_id, price, quantity, image, "
                                          "stock_status_id, tax_class_id, status, date_added, date_modified) "
                                          "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)", db_server);
                queryAddProduct.bindValue(0, _art);
                queryAddProduct.bindValue(1, _model);
                queryAddProduct.bindValue(2, _manuf_id);
                queryAddProduct.bindValue(3, _price);
                queryAddProduct.bindValue(4, _quan);
                queryAddProduct.bindValue(5, _image);
                queryAddProduct.bindValue(6, _STOCK);
                queryAddProduct.bindValue(7, _TAX);
                queryAddProduct.bindValue(8, _STATUS);
                queryAddProduct.bindValue(9, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
                queryAddProduct.bindValue(10, QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));
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
                    queryAddProdDesc.bindValue(2, _name);
                    queryAddProdDesc.bindValue(3, _desc);
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
                    queryAddProdCat.bindValue(1, _group_id);
                    queryAddProdCat.bindValue(2, 1);
                    queryAddProdCat.exec();
                    if (queryAddProdCat.lastError().text().size() > 3){
                      error.append(queryAddProdCat.lastError().text());
                    }

                    //добавляем аттрибуты
                    if (ui->tableWidget_prod_attributes->rowCount() > 0){
                        for (int rowAtt = 0; rowAtt < ui->tableWidget_prod_attributes->rowCount(); rowAtt++){

                            QString idx;
                            if (ui->radioButton_f_openMain->isChecked()){
                                idx = _art; // для сравнения основного файла используем артикул
                            } else if (ui->radioButton_f_openPL->isChecked()){
                                idx = QString::number(row); // для сравнения прайс - листов используем номера строк
                            }

                            if (ui->tableWidget_prod_attributes->item(rowAtt, 0)->text() == idx){
                                int id_attribute = ui->tableWidget_prod_attributes->item(rowAtt, 3)->text().toInt();
                                QString value_attribute = ui->tableWidget_prod_attributes->item(rowAtt, 2)->text();
                                QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                                "VALUES (?, ?, ?, ?)", db_server);
                                queryAddProdAttribute.bindValue(0, _ID);
                                queryAddProdAttribute.bindValue(1, id_attribute);
                                queryAddProdAttribute.bindValue(2, _LANG);
                                queryAddProdAttribute.bindValue(3, value_attribute);
                                queryAddProdAttribute.exec();
                                if (queryAddProdAttribute.lastError().text().size() > 3){
                                    qDebug() << queryAddProdAttribute.lastError();
                                    qDebug() << _ID << id_attribute << value_attribute;
                                  error.append(queryAddProdAttribute.lastError().text());
                                }
                            }
                        }
                    }
                    //*************
                    //добавляем изображение
                    if (ui->tableWidget_prod_image->rowCount() > 0){
                        for (int rowI = 0; rowI < ui->tableWidget_prod_image->rowCount(); rowI++){
                            if (ui->tableWidget_prod_image->item(rowI, 0)->text().toInt() == row){
                                QString _dop_image(QString("%1/%2").arg(_DIR).arg(ui->tableWidget_prod_image->item(rowI, 1)->text()));
                                QSqlQuery queryAddProdImage("INSERT INTO rmrt_product_image(product_id, image, sort_order) "
                                                                "VALUES (?, ?, ?)", db_server);
                                queryAddProdImage.bindValue(0, _ID);
                                queryAddProdImage.bindValue(1, _dop_image);
                                queryAddProdImage.bindValue(2, 0);
                                queryAddProdImage.exec();
                                if (queryAddProdImage.lastError().text().size() > 3){
                                  error.append(queryAddProdImage.lastError().text());
                                }
                            }
                        }
                    }
                    //*************
                }
            }
            if (idxUP){                
                QString mainStr;
                if (ui->checkBox_sC_upname->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product_description SET `name` = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(_name)
                                   .arg(_id)
                                   .arg(_LANG));
                }
                if (ui->checkBox_sC_upmanuf->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product SET manufacturer_id = \'%1\' "
                                           "WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(_manuf_id)
                                   .arg(_id));
                }

                if (ui->checkBox_sC_upprice->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product SET price = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(_price)
                                   .arg(_id));
                }
                if (ui->checkBox_sC_upquan->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product SET quantity = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(_quan)
                                   .arg(_id));
                }
                if (ui->checkBox_sC_updesc->isChecked()){
                    mainStr.append(QString("UPDATE rmrt_product_description SET description = \'%1\' "
                                           "WHERE rmrt_product_description.product_id = \'%2\' "
                                           "AND rmrt_product_description.language_id = \'%3\'; ")
                                   .arg(_desc)
                                   .arg(_id)
                                   .arg(_LANG));
                }

                if (ui->checkBox_sC_upimage->isChecked()){                    
                    mainStr.append(QString("UPDATE rmrt_product SET image = \'%1\' WHERE rmrt_product.product_id = \'%2\'; ")
                                   .arg(_image)
                                   .arg(_id));
                }


                if (ui->checkBox_sC_upattribute->isChecked()){
                    if (ui->tableWidget_prod_attributes->rowCount() > 0){
                        //clear;
                        QSqlQuery queryClearProdAttribute(QString("DELETE FROM rmrt_product_attribute WHERE rmrt_product_attribute.product_id = \'%1\' ")
                                                          .arg(_id), db_server);
                        queryClearProdAttribute.exec();

                        for (int rowAtt = 0; rowAtt < ui->tableWidget_prod_attributes->rowCount(); rowAtt++){

                            QString idx;
                            if (ui->radioButton_f_openMain->isChecked()){
                                idx = _art; // для сравнения основного файла используем артикул
                            } else if (ui->radioButton_f_openPL->isChecked()){
                                idx = QString::number(row); // для сравнения прайс - листов используем номера строк
                            }

                            if (ui->tableWidget_prod_attributes->item(rowAtt, 0)->text() == idx){
                                int id_attribute = ui->tableWidget_prod_attributes->item(rowAtt, 3)->text().toInt();
                                QString value_attribute = ui->tableWidget_prod_attributes->item(rowAtt, 2)->text();
                                QSqlQuery queryAddProdAttribute("INSERT INTO rmrt_product_attribute (product_id, attribute_id, language_id, text) "
                                                                "VALUES (?, ?, ?, ?)", db_server);
                                queryAddProdAttribute.bindValue(0, _id);
                                queryAddProdAttribute.bindValue(1, id_attribute);
                                queryAddProdAttribute.bindValue(2, _LANG);
                                queryAddProdAttribute.bindValue(3, value_attribute);
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
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
            QApplication::processEvents();
        }
        if (error.size() > 0){
            makeMessage(error, false);
        } else {
            makeMessage("Товары успешно загружены на сервер!", true);
            product_testServer();
        }
    }
    ui->progressBar->hide();
}

void ocMeridia::product_manufacturer()
{
    if (_test_manufacturer == false){
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);
        QString error;
        if (ui->tableWidget_product->rowCount() > 0){
            for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
                QString prod_manuf(ui->tableWidget_product->item(row, 5)->text());
                // проверка наличия
                QSqlQuery query_test(QString("SELECT rmrt_manufacturer_description.manufacturer_id "
                                             "FROM rmrt_manufacturer_description "
                                             "WHERE rmrt_manufacturer_description.language_id = \'%1\' "
                                             "AND rmrt_manufacturer_description.name = \'%2\' ")
                                     .arg(_LANG)
                                     .arg(prod_manuf), db_server);
                query_test.next();
                if (query_test.isValid()){
                    //если есть - ставим в таблицу
                    QTableWidgetItem *item = new QTableWidgetItem(query_test.value(0).toString());
                    ui->tableWidget_product->setItem(row, 5, item);
                } else {
                    // нет - добавляем
                    int man_id = 0;
                    QSqlQuery query_add("INSERT INTO rmrt_manufacturer (name, image, sort_order) "
                                                "VALUES (?, ?, ?)", db_server);
                    query_add.bindValue(0, prod_manuf);
                    query_add.bindValue(1, "");
                    query_add.bindValue(2, 0);
                    query_add.exec();
                    if (query_add.lastError().text().size() > 3){
                        error.append(query_add.lastError().text());
                        break;
                    } else {
                        man_id = query_add.lastInsertId().toInt();
                        // desc
                        QSqlQuery queryDesc("INSERT INTO rmrt_manufacturer_description (manufacturer_id, language_id, name, description) "
                                            "VALUES (?, ?, ?, ?)", db_server);
                        queryDesc.bindValue(0, man_id);
                        queryDesc.bindValue(1, _LANG);
                        queryDesc.bindValue(2, prod_manuf);
                        queryDesc.bindValue(3, prod_manuf);
                        queryDesc.exec();
                        if (queryDesc.lastError().text().size() > 3){
                            error.append(queryDesc.lastError().text());
                        }
                        //store
                        QSqlQuery queryStore("INSERT INTO rmrt_manufacturer_to_store (manufacturer_id, store_id) "
                                                                "VALUES (?, ?)", db_server);
                        queryStore.bindValue(0, man_id);
                        queryStore.bindValue(1, _STORE);
                        queryStore.exec();
                        if (queryStore.lastError().text().size() > 3){
                            error.append(queryStore.lastError().text());
                        }
                    }

                    if (error.size() > 0){
                        makeMessage(error, false);
                    } else {
                        QTableWidgetItem *item = new QTableWidgetItem(QString::number(man_id));
                        ui->tableWidget_product->setItem(row, 5, item);
                    }
                }
                ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
                QApplication::processEvents();
            }
        }
        ui->progressBar->hide();
        _test_manufacturer = true;
    }

}

void ocMeridia::product_change_group()
{
    for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
        int id = ui->tableWidget_product->item(row, 0)->text().toInt();
        if (id == 0){
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(QString::number(categoryMap.value(ui->comboBox_sC_np_group->currentIndex())));
            ui->tableWidget_product->setItem(row, 4, item);
        }
    }
}

void ocMeridia::local_connect()
{
    local *loc = new local(db_local, db_server, this);
    loc->exec();
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
