#include "ocmeridia.h"
#include "ui_ocmeridia.h"

ocMeridia::ocMeridia(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ocMeridia)
{
    ui->setupUi(this);

    cFile = new ctrlFile();
    cPreference = new preference();


    ui->groupBox_messa->hide();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->groupBox_messa, SLOT(hide()));

    readSetting();

    //file
    ui->groupBox_f_maket->hide();
    connect(ui->toolButton_file, SIGNAL(clicked(bool)), this, SLOT(file_Select()));
   // connect(ui->pushButton_fm_openFile, SIGNAL(clicked(bool)), this, SLOT(file_Open_PL()));
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
    //ui->tableWidget_product->setColumnHidden(0, true);
    connect(ui->pushButton_prod_editor, SIGNAL(clicked(bool)), this, SLOT(product_openForm()));
    connect(ui->pushButton_loadOnServer, SIGNAL(clicked(bool)), this, SLOT(product_loadOnServer()));
    connect(ui->pushButton_prod_test, SIGNAL(clicked(bool)), this, SLOT(product_testServer()));

    //связи
    connect(ui->toolButton_cat_connect, SIGNAL(clicked(bool)), this, SLOT(edit_connection()));
    connect(ui->toolButton_att_connect, SIGNAL(clicked(bool)), this, SLOT(edit_connection()));

    //test
    connect(ui->toolButton_p_mStatus, SIGNAL(clicked(bool)), this, SLOT(remakeStatus()));
    connect(ui->toolButton_p_mGroup, SIGNAL(clicked(bool)), this, SLOT(remakeGroup()));
    connect(ui->toolButton_p_mQ, SIGNAL(clicked(bool)), this, SLOT(remakeQuantity()));
    //connect(ui->pushButton_loadOnServer, SIGNAL(clicked(bool)), this, SLOT(product_save()));
    connect(ui->pushButton_fm_openFile, SIGNAL(clicked(bool)), this, SLOT(openCSV()));
}

ocMeridia::~ocMeridia()
{

    delete ui;
}

void ocMeridia::readSetting()
{
    _STORE = 0;
    _LANG = cPreference->get_OC_lang();
    _TAX = cPreference->get_OC_tax();
    //_STOCK = sett.value("oc/stock").toInt();
    //_STATUS = sett.value("oc/status").toInt();
    _QUAN = cPreference->get_OC_quan();
    _DIR = "catalog/";
    _DIR.append(cPreference->get_OC_dir());
    setting_connect_netDB();
    setting_connect_locDB();
}


void ocMeridia::setting_connect_netDB()
{
    db_server = QSqlDatabase::addDatabase("QMYSQL", "serverDB");
    db_server.setDatabaseName(cPreference->get_DB_name());
    db_server.setHostName(cPreference->get_DB_host());
    db_server.setPort(cPreference->get_DB_port());
    db_server.setUserName(cPreference->get_DB_user());
    db_server.setPassword(cPreference->get_DB_pass());
    db_server.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE;");
    db_server.open();

    if (db_server.isOpen()){
        makeMessage("К удаленной БД подключено", true);

        cCategory = new ctrlSCategory(db_server, "rmrt", _STORE);
        cProduct = new ctrlSProduct(db_server, "rmrt", _STORE);
        cManufacturer = new ctrlManufacturer(db_server, "rmrt", _STORE);
        cOption = new ctrlOptions(db_server, "rmrt");
        cAttribute = new ctrlAttribute(db_server, "rmrt", _STORE);

        category_loadFromDB();
        attribute_loadFromDB();
        ui->tab_file->setEnabled(true);
        ui->tab_category->setEnabled(true);
        ui->tab_attribute->setEnabled(true);
        ui->tab_product->setEnabled(true);

    } else {
        makeMessage("К удаленной БД не подключено, проверьте настройки", false);
        ui->tabWidget_main->setCurrentIndex(0);
        ui->tab_file->setEnabled(false);
        ui->tab_category->setEnabled(false);
        ui->tab_attribute->setEnabled(false);
        ui->tab_product->setEnabled(false);
        ui->treeWidget_cat_serv->clear();
        ui->treeWidget_attr->clear();
    }
}

void ocMeridia::setting_connect_locDB()
{
    db_local = QSqlDatabase::addDatabase("QSQLITE", "localDB");
    db_local.setDatabaseName("meridia.db3");
    db_local.open();
}


//***************************************************************************
//---------------------------FILE--------------------------------------------
void ocMeridia::file_Select()
{
    if (ui->radioButton_f_openMain->isChecked()){
        ui->lineEdit_file->setText(QFileDialog::getOpenFileName(this, "Open", "HOME", "XML 1C (*.cml)"));
        if (!ui->lineEdit_file->text().isEmpty()){
            //file_Open_Main();
            openXML();
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
            QTextCodec *codec = QTextCodec::codecForName(ui->comboBox_f_csv_codec->currentText().toUtf8());
            QString _csv = codec->toUnicode(ar);
            csvData.append(_csv);
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
        ui->comboBox_f_csv_codec->setCurrentIndex(ui->comboBox_f_csv_codec->findText(maket.value("codec").toString()));
        ui->checkBox_fm_articul->setChecked(maket.value("art_check").toBool());
        ui->checkBox_fm_model->setChecked(maket.value("model_check").toBool());
        ui->checkBox_fm_name->setChecked(maket.value("name_check").toBool());
        ui->checkBox_fm_manuf->setChecked(maket.value("manuf_check").toBool());
        ui->checkBox_fm_price->setChecked(maket.value("price_check").toBool());
        ui->checkBox_fm_specPrice->setChecked(maket.value("specPrice_check").toBool());
        ui->checkBox_fm_quan->setChecked(maket.value("quan_check").toBool());
        ui->checkBox_fm_desc->setChecked(maket.value("desc_check").toBool());
        ui->checkBox_fm_image->setChecked(maket.value("image_check").toBool());
        ui->checkBox_fm_attr->setChecked(maket.value("attr_check").toBool());
        ui->checkBox_fm_price_corr->setChecked(maket.value("price_corr_check").toBool());
        ui->checkBox_fm_price_corr->setChecked(maket.value("specPrice_corr_check").toBool());

        ui->lineEdit_fm_category->setText(maket.value("cat").toString());
        ui->lineEdit_fm_desc->setText(maket.value("desc").toString());
        ui->lineEdit_fm_image->setText(maket.value("image").toString());
        ui->lineEdit_fm_attr->setText(maket.value("attr").toString());
        ui->spinBox_fm_articul->setValue(maket.value("art").toInt());
        ui->lineEdit_fm_articulPrefix->setText(maket.value("prefix").toString());
        ui->spinBox_fm_model->setValue(maket.value("model").toInt());
        ui->spinBox_fm_name->setValue(maket.value("name").toInt());
        ui->spinBox_fm_manuf->setValue(maket.value("manuf").toInt());
        ui->spinBox_fm_price->setValue(maket.value("price").toInt());
        ui->spinBox_fm_specPrice->setValue(maket.value("specPrice").toInt());
        ui->spinBox_fm_quan->setValue(maket.value("quan").toInt());
        ui->spinBox_fm_price_cor->setValue(maket.value("price_corr").toInt());
        ui->spinBox_fm_specPrice_cor->setValue(maket.value("specPrice_corr").toInt());
    } else {
        ui->lineEdit_fm_articulPrefix->clear();
        ui->comboBox_f_csv_split->setCurrentIndex(0);
        ui->spinBox_f_csv_firstRow->setValue(1);
        ui->checkBox_fm_articul->setChecked(false);
        ui->checkBox_fm_model->setChecked(false);
        ui->checkBox_fm_name->setChecked(false);
        ui->checkBox_fm_manuf->setChecked(false);
        ui->checkBox_fm_price->setChecked(false);
        ui->checkBox_fm_specPrice->setChecked(false);
        ui->checkBox_fm_quan->setChecked(false);
        ui->checkBox_fm_desc->setChecked(false);
        ui->checkBox_fm_image->setChecked(false);
        ui->checkBox_fm_attr->setChecked(false);
        ui->checkBox_fm_price_corr->setChecked(false);
        ui->checkBox_fm_specPrice_corr->setChecked(false);
        ui->lineEdit_fm_category->clear();
        ui->lineEdit_fm_desc->clear();
        ui->lineEdit_fm_image->clear();
        ui->lineEdit_fm_attr->clear();
        ui->spinBox_fm_articul->setValue(1);
        ui->spinBox_fm_model->setValue(1);
        ui->spinBox_fm_name->setValue(1);
        ui->spinBox_fm_manuf->setValue(1);
        ui->spinBox_fm_price->setValue(1);
        ui->spinBox_fm_specPrice->setValue(1);
        ui->spinBox_fm_quan->setValue(1);
        ui->spinBox_fm_specPrice_cor->setValue(0);
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
    maket.setValue("codec", ui->comboBox_f_csv_codec->currentText());
    maket.setValue("art_check", ui->checkBox_fm_articul->isChecked());
    maket.setValue("model_check", ui->checkBox_fm_model->isChecked());
    maket.setValue("name_check", ui->checkBox_fm_name->isChecked());
    maket.setValue("manuf_check", ui->checkBox_fm_manuf->isChecked());
    maket.setValue("price_check", ui->checkBox_fm_price->isChecked());
    maket.setValue("specPrice_check", ui->checkBox_fm_specPrice->isChecked());
    maket.setValue("quan_check", ui->checkBox_fm_quan->isChecked());
    maket.setValue("desc_check", ui->checkBox_fm_desc->isChecked());
    maket.setValue("image_check", ui->checkBox_fm_image->isChecked());
    maket.setValue("attr_check", ui->checkBox_fm_attr->isChecked());
    maket.setValue("price_corr_check", ui->checkBox_fm_price_corr->isChecked());
    maket.setValue("specPrice_corr_check", ui->checkBox_fm_specPrice_corr->isChecked());

    maket.setValue("cat", ui->lineEdit_fm_category->text());
    maket.setValue("desc", ui->lineEdit_fm_desc->text());
    maket.setValue("image", ui->lineEdit_fm_image->text());
    maket.setValue("attr", ui->lineEdit_fm_attr->text());
    maket.setValue("art", ui->spinBox_fm_articul->value());
    maket.setValue("prefix", ui->lineEdit_fm_articulPrefix->text());
    maket.setValue("model", ui->spinBox_fm_model->value());
    maket.setValue("name", ui->spinBox_fm_name->value());
    maket.setValue("manuf", ui->spinBox_fm_manuf->value());
    maket.setValue("price", ui->spinBox_fm_price->value());
    maket.setValue("specPrice", ui->spinBox_fm_specPrice->value());
    maket.setValue("quan", ui->spinBox_fm_quan->value());
    maket.setValue("price_corr", ui->spinBox_fm_price_cor->value());
    maket.setValue("specPrice_corr", ui->spinBox_fm_specPrice_cor->value());
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

            _cat = lineA.split(splA).at(ui->lineEdit_fm_category->text().split("|").at(0).toInt() - 1);
            if (ui->lineEdit_fm_category->text().split("|").size() > 1){
                for (int xc = 1; xc < ui->lineEdit_fm_category->text().split("|").size(); xc++){
                    _cat.append(QString("|%1").arg(lineA.split(splA).at(ui->lineEdit_fm_category->text().split("|").at(xc).toInt() - 1)));
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

    QMap<int, QMap<QString, QVariant> > categories = cCategory->get_mainCategories();
    for (int x = 0; x < categories.count(); x++){
        QMap<QString, QVariant> map = categories.value(x);
        cCategory->set_current(map.value("categoryId").toInt(), _LANG);

        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(2, map.value("categoryId").toString());
        item->setText(0, cCategory->get_name());
        item->setText(1, cCategory->get_code());
        ui->treeWidget_cat_serv->addTopLevelItem(item);

        category_loadFromDB_parent(item, map.value("categoryId").toString());

    }
    ui->treeWidget_cat_serv->expandAll();
    ui->treeWidget_cat_serv->setColumnHidden(2, true);
    ui->treeWidget_cat_serv->resizeColumnToContents(0);
    ui->treeWidget_cat_serv->resizeColumnToContents(1);
    ui->treeWidget_cat_serv->header()->setStretchLastSection(true);

//--
    categoryMap.clear();
    ui->comboBox_prod_group->clear();
    ui->comboBox_p_mGroup->clear();

    QMap<int, QMap<QString,QVariant> > MAP = cCategory->get_categories();
    for (int x = 0; x < MAP.count(); x++){
        QMap<QString,QVariant> _map = MAP.value(x);

        int id = _map.value("categoryId").toInt();
        QString name = cCategory->get_road(id, _LANG);

        ui->comboBox_prod_group->addItem(name);
        ui->comboBox_p_mGroup->addItem(name);
        categoryMap.insert(x, id);
    }


}


//TODO - Category
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
            QString _CODE = ui->tableWidget_cat_file->item(row, 0)->text();
            int _ID = cCategory->getIdByCode(_CODE);
            if (_ID > 0){ // есть -> подставляем ID в табл. товары, удаляем строку
                // меняем в таблице товаров код1С на ID групп магазина
                for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                    QString productG1C = ui->tableWidget_product->item(rowp, 16)->text();
                    if (productG1C == _CODE){
                        QTableWidgetItem *itemName = new QTableWidgetItem(cCategory->get_road(_ID, _LANG));
                        ui->tableWidget_product->setItem(rowp, 4, itemName);
                        QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(_ID));
                        ui->tableWidget_product->setItem(rowp, 13, itemID);
                    }
                }
                // cтроки для удаления
                del_list.append(row);
            } else {
                // проверяем наличие категории в синонимах
                //TODO - local - category
                QSqlQuery queryL(QString("SELECT cat.id_db FROM cat WHERE cat.cod = \'%1\'")
                                 .arg(_CODE)
                                 , db_local);
                queryL.next();
                if (queryL.isValid()){
                    _ID = queryL.value(0).toInt();
                    // меняем в таблице товаров код1С на ID группы из справочника синонимов
                    for (int rowp = 0; rowp < ui->tableWidget_product->rowCount(); rowp++){
                        QString productG1C = ui->tableWidget_product->item(rowp, 16)->text();
                        if (productG1C == _CODE){
                            QTableWidgetItem *itemName = new QTableWidgetItem(cCategory->get_road(_ID, _LANG));
                            ui->tableWidget_product->setItem(rowp, 4, itemName);
                            QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(_ID));
                            ui->tableWidget_product->setItem(rowp, 13, itemID);
                        }
                    }
                    // cтроки для удаления
                    del_list.append(row);
                } else {
                //оставляем в ручную обработку
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

    //test product
    if (ui->tableWidget_cat_file->rowCount() == 0){
        product_testServer();
    }
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

                QMap<QString,QVariant> saveMap;
                saveMap.insert("id", 0);
                saveMap.insert("parentId", cat_activeID);
                saveMap.insert("top", cat_top);
                saveMap.insert("column", 1);
                saveMap.insert("sortOrder", 0);
                saveMap.insert("status", 1);
                saveMap.insert("code", ui->tableWidget_cat_file->item(row, 0)->text());
                saveMap.insert("name", cat_name);
                saveMap.insert("desc", cat_name);

                QMap<QString,QVariant> feedMain = cCategory->saveMain(saveMap, _LANG);
                if (!feedMain.value("error").toString().isEmpty()){
                    error.append(feedMain.value("error").toString());
                } else {
                    cCategory->makeCategoryPath();
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

            //TODO - local - category
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

//TODO - server - attribute
void ocMeridia::attribute_loadFromDB()
{
    attributeMap.clear();
    ui->treeWidget_attr->clear();
    QSqlQuery queryGroup(QString("SELECT rmrt_attribute_group_description.name, "
                         "rmrt_attribute_group_description.attribute_group_id "
                         "FROM rmrt_attribute_group_description "
                         "WHERE rmrt_attribute_group_description.language_id = \'%1\' ")
                         .arg(_LANG), db_server);
    while (queryGroup.next()){
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
//TODO - server - attribute
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
            query.bindValue(0, ui->tableWidget_fileAttr->item(crow, 0)->text());
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

void ocMeridia::options_testServer()
{
    //1- обновление опций
    cOption->remakeOptions(cFile->getOptionsList(), _LANG);
    //2 - обновление значений опций
    cOption->remakeOptionValue(cFile->getProductOptions(), _LANG);

}
//TODO - server - attribute
void ocMeridia::attribute_testServer()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);
    if (ui->tableWidget_fileAttr->rowCount() > 0){
        QList<int> del_list;
        for (int row = 0; row < ui->tableWidget_fileAttr->rowCount(); row++){

            QString aName = ui->tableWidget_fileAttr->item(row, 0)->text();
            int aID = cAttribute->get_attributeID(aName);

            if (aID == 0){
                QSqlQuery queryL(QString("SELECT att.id_db FROM att WHERE att.name = \'%1\' ")
                                 .arg(aName), db_local);
                queryL.next();
                if (queryL.isValid()){
                    aID = queryL.value(0).toInt();
                    del_list.append(row);
                }
            } else {
                del_list.append(row);
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
}

void ocMeridia::product_testServer()
{
    if (ui->tableWidget_product->rowCount() > 0){

        //product_manufacturer();

        ui->progressBar->setValue(0);
        ui->progressBar->setVisible(true);

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){

            QString sku = ui->tableWidget_product->item(row, 1)->text();
            QString name = ui->tableWidget_product->item(row, 3)->text();

            bool idx = false;
            //проверяем по артикулу + наименование
            int ID = cProduct->getIDforTestA(sku, name);

            if (ID > 0){
                // если есть - ставим ИД
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ID));
                ui->tableWidget_product->setItem(row, 0, itemID);
                idx = true;
            } else {
                //если нет проверяем только по артикулу
                ID = cProduct->getIDforTestB(sku);
                if (ID > 0){
                    // если есть - ставим ИД
                    QTableWidgetItem *itemID = new QTableWidgetItem();
                    itemID->setText(QString::number(ID));
                    ui->tableWidget_product->setItem(row, 0, itemID);
                    idx = true;
                } else {
                    // если нет
                    QTableWidgetItem *itemID = new QTableWidgetItem("0");
                    ui->tableWidget_product->setItem(row, 0, itemID);
                    idx = false;
                }
            }

            //подтягивает текущий статус
            if (ID > 0){ // из БД
                cProduct->set_current(ID);
                int statusID = cProduct->get_status();
                QTableWidgetItem *itemSt = new QTableWidgetItem();

                if (statusID == 0){
                    itemSt->setText("Отключено");
                } else {
                    itemSt->setText("Включено");
                }

                ui->tableWidget_product->setItem(row, 11, itemSt);
                QTableWidgetItem *itemStID = new QTableWidgetItem(QString::number(statusID));
                ui->tableWidget_product->setItem(row, 12, itemStID);
            } else { // из настройки
                int idStatus;
                QString strStatus;

                if (ui->radioButton_f_openMain->isChecked()){
                    idStatus = cPreference->get_XML_newStatusId();
                } else if (ui->radioButton_f_openPL->isChecked()){
                    idStatus = cPreference->get_CSV_newStatusId();
                }

                if (idStatus == 1){
                    strStatus = "Включено";
                } else {
                    strStatus = "Отключено";
                }

                QTableWidgetItem *itemStatus = new QTableWidgetItem(strStatus);
                ui->tableWidget_product->setItem(row, 11, itemStatus);
                QTableWidgetItem *itemStatusID = new QTableWidgetItem(QString::number(idStatus));
                ui->tableWidget_product->setItem(row, 12, itemStatusID);
            }

            //определяем производителя
            if (ui->tableWidget_product->item(row, 14)->text().toInt() == 0){
                QString mName = ui->tableWidget_product->item(row, 5)->text();

                // проверка наличия
                int mID = cManufacturer->getID_byName(mName);
                if (mID == 0){
                    QMap<QString,QVariant> saveMap;
                    saveMap.insert("manufacturerId", mID);
                    saveMap.insert("name", mName);
                    QMap<QString,QVariant> feed = cManufacturer->save(saveMap, _LANG);
                    if (feed.value("error").toString().isEmpty()){
                        mID = feed.value("lastId").toInt();
                    } else {
                        makeMessage(feed.value("error").toString(), false);
                    }
                }
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(mID));
                ui->tableWidget_product->setItem(row, 14, itemID);
            }

            //замена категорий по настройке (для новых товаров
            if (ID == 0){
                if (ui->radioButton_f_openMain->isChecked()){
                    if (cPreference->get_XML_newGroup()){
                        QTableWidgetItem *itemN = new QTableWidgetItem();
                        itemN->setText(cCategory->get_road(cPreference->get_XML_newGroupId(), _LANG));
                        ui->tableWidget_product->setItem(row, 4, itemN);
                        QTableWidgetItem *itemId = new QTableWidgetItem();
                        itemId->setText(QString::number(cPreference->get_XML_newGroupId()));
                        ui->tableWidget_product->setItem(row, 13, itemId);
                    }
                } else if (ui->radioButton_f_openPL->isChecked()){
                    if (cPreference->get_CSV_newGroup()){
                        QTableWidgetItem *itemN = new QTableWidgetItem();
                        itemN->setText(cCategory->get_road(cPreference->get_CSV_newGroupId(), _LANG));
                        ui->tableWidget_product->setItem(row, 4, itemN);
                        QTableWidgetItem *itemId = new QTableWidgetItem();
                        itemId->setText(QString::number(cPreference->get_CSV_newGroupId()));
                        ui->tableWidget_product->setItem(row, 13, itemId);
                    }
                }
            }

            //установка спец цен
            if (ui->radioButton_f_openMain->isChecked()){
                if (cPreference->get_XML_specPrice()){
                    double _specPrice = ui->tableWidget_product->item(row, 6)->text().toDouble();
                    double _sk = 0;
                    if (cPreference->get_XML_specSkidka()){
                        _sk = _specPrice * (cPreference->get_XML_specSkidkaValue() / 100);
                    }
                    double _basePrice = _specPrice + _sk;
                    QTableWidgetItem *itemBasePrice = new QTableWidgetItem(QString::number(_basePrice, 'f', 2));
                    ui->tableWidget_product->setItem(row, 6, itemBasePrice);
                    QTableWidgetItem *itemSpPrice = new QTableWidgetItem(QString::number(_specPrice, 'f', 2));
                    ui->tableWidget_product->setItem(row, 17, itemSpPrice);
                }
            }


            QColor colBG;
            QColor colFG;
            if (idx) {
                colFG.setNamedColor("#FAFAFA");
                colBG.setNamedColor("#006400");
            } else {
                colFG.setNamedColor("#FAFAFA");
                colBG.setNamedColor("#CD0000");
            }
            for (int col = 0; col < ui->tableWidget_product->columnCount(); col++){
                ui->tableWidget_product->item(row, col)->setBackgroundColor(colBG);
                ui->tableWidget_product->item(row, col)->setForeground(colFG);
            }
            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
            QApplication::processEvents();
        }
        ui->progressBar->hide();
//        //замена категорий новых товаров на указанную в настройках
//        if (cPreference->get_XML_newGroup()){
//            product_change_group();
//        }
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
    QString error;
    if (ui->tableWidget_cat_file->rowCount() > 0){
        error.append("Не все категории определены!");
    } else {
        if (ui->tableWidget_fileAttr->rowCount() > 0){
            error.append("Не все атрибуты определены!");
        }
    }

    if (!error.isEmpty()){
        makeMessage(error, false);
    } else {
        if (ui->radioButton_f_openMain->isChecked()){
            product_saveXML();
        } else {
            product_saveCSV();
        }
    }

}

void ocMeridia::product_manufacturer()
{
    if (_test_manufacturer == false){
        ui->progressBar->setVisible(true);
        ui->progressBar->setValue(0);
        QString error;
        if (ui->tableWidget_product->rowCount() > 0){
            for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
                if (ui->tableWidget_product->item(row, 14)->text().toInt() == 0){
                    QString mName = ui->tableWidget_product->item(row, 5)->text();

                    // проверка наличия
                    int mID = cManufacturer->getID_byName(mName);
                    if (mID == 0){
                        QMap<QString,QVariant> saveMap;
                        saveMap.insert("manufacturerId", mID);
                        saveMap.insert("name", mName);
                        QMap<QString,QVariant> feed = cManufacturer->save(saveMap, _LANG);
                        if (feed.value("error").toString().isEmpty()){
                            mID = feed.value("lastId").toInt();
                        } else {
                            makeMessage(feed.value("error").toString(), false);
                        }
                    }
                    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(mID));
                    ui->tableWidget_product->setItem(row, 14, itemID);
                }
                ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
                QApplication::processEvents();
            }
        }
        ui->progressBar->hide();
        _test_manufacturer = true;
    }

}

void ocMeridia::product_saveXML()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    QMap<QString, QVariant> saveMap;
    QMap<int, QMap<QString, QVariant> > saveMapOption;
    QMap<int, QMap<QString, QVariant> > saveMapAttribute;
    QMap<int, QMap<QString, QVariant> > saveMapImage;
    QMap<QString,QVariant> feed;
    QString error;
    if (ui->tableWidget_product->rowCount() > 0){

        bool addXML = true;
        bool upXML = true;

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            //переменные для логики загрузки
            int _productId = ui->tableWidget_product->item(row, 0)->text().toInt();
            int _groupId = ui->tableWidget_product->item(row, 13)->text().toInt();
            double _price = ui->tableWidget_product->item(row, 6)->text().toDouble();
            //----

            //алгоритм
            if (_productId == 0){ //новые позиции
                upXML = false;
                addXML = cPreference->get_XML_newProduct();
                if (_price == 0){
                    addXML = cPreference->get_XML_newNotPrice();
                }
            } else {
                addXML = false;
                upXML = true;
                if (_price == 0){
                    upXML = cPreference->get_XML_newNotPrice();
                }
            }

            if (ui->radioButton_p_Load_onlyGroup->isChecked()){
                int targetGroup = categoryMap.value(ui->comboBox_prod_group->currentIndex());
                if (targetGroup != _groupId){
                    addXML = false;
                    upXML = false;
                }
            } else if (ui->radioButton_p_Load_select->isChecked()){
                if (!ui->tableWidget_product->item(row, 0)->isSelected()){
                    addXML = false;
                    upXML = false;
                }
            }

            //формируем карту сохранения
            saveMap.clear();
            saveMapOption.clear();
            if (addXML or upXML){
                if (addXML){
                    QString _SKU = ui->tableWidget_product->item(row, 1)->text();

                    saveMap.insert("productId", _productId);
                    saveMap.insert("sku", _SKU);
                    saveMap.insert("model", ui->tableWidget_product->item(row, 2)->text());
                    saveMap.insert("name", ui->tableWidget_product->item(row, 3)->text());
                    saveMap.insert("groupId", _groupId);
                    saveMap.insert("manufacturerId", ui->tableWidget_product->item(row, 14)->text().toInt());
                    saveMap.insert("price", _price);
                    saveMap.insert("quantity", ui->tableWidget_product->item(row, 7)->text().toInt());

                    if (ui->tableWidget_product->item(row, 8)->text().isEmpty()){
                        saveMap.insert("description", ui->tableWidget_product->item(row, 3)->text());
                    } else {
                        saveMap.insert("description", ui->tableWidget_product->item(row, 8)->text());
                    }

                    if (ui->tableWidget_product->item(row, 9)->text().size() > 0){
                        saveMap.insert("image", QString("%1/%2").arg(_DIR).arg(ui->tableWidget_product->item(row, 9)->text()));
                    }
                    saveMap.insert("stockStatusId", ui->tableWidget_product->item(row, 15)->text().toInt());
                    saveMap.insert("status", ui->tableWidget_product->item(row, 12)->text().toInt());
                    saveMap.insert("specPrice", ui->tableWidget_product->item(row, 17)->text().toDouble());
                    saveMap.insert("taxClassId", _TAX);
                    saveMap.insert("languageId", _LANG);


                    //выбираем опции по каждому товару - проверка по артикулу
                    int x = 0;
                    for (int rowA = 0; rowA < ui->tableWidget_prod_option->rowCount(); rowA++){
                        if (ui->tableWidget_prod_option->item(rowA, 0)->text() == _SKU){

                            QMap<QString, QVariant> _map;
                            _map.insert("productId", ui->tableWidget_prod_option->item(rowA, 0)->text());
                            _map.insert("optionId", ui->tableWidget_prod_option->item(rowA, 1)->text());
                            _map.insert("optionValueId", ui->tableWidget_prod_option->item(rowA, 2)->text());
                            _map.insert("quantity", cPreference->get_XML_newQuan());
                            //qDebug() << _map;
                            saveMapOption.insert(x, _map);
                            x++;
                        }
                    }
                } else if (upXML){
                    saveMap.insert("productId", _productId);

                    saveMap.insert("specPrice", ui->tableWidget_product->item(row, 17)->text().toDouble());

                    if (cPreference->get_XML_upGroup()){
                        saveMap.insert("groupId", _groupId);
                    } else {
                        saveMap.insert("groupId", 0);
                    }
                    if (ui->tableWidget_product->item(row, 1)->text().size() > 0){
                        saveMap.insert("sku", ui->tableWidget_product->item(row, 1)->text());
                    }
                    if (cPreference->get_XML_upName()){
                        saveMap.insert("name", ui->tableWidget_product->item(row, 3)->text());
                    }
                    if (cPreference->get_XML_upDesc()){
                        saveMap.insert("description", ui->tableWidget_product->item(row, 8)->text());
                    }
                    if (cPreference->get_XML_upManufacturer()){
                        saveMap.insert("manufacturerId", ui->tableWidget_product->item(row, 14)->text().toInt());
                    }
                    if (cPreference->get_XML_upQuan()){
                        saveMap.insert("quantity", ui->tableWidget_product->item(row, 7)->text().toInt());
                    }
                    if (cPreference->get_XML_upPrice()){
                        saveMap.insert("price", _price);
                    }
                    if (cPreference->get_XML_upImage()){
                        saveMap.insert("image", QString("%1/%2").arg(_DIR).arg(ui->tableWidget_product->item(row, 9)->text()));
                    }
                    if (cPreference->get_XML_upStockStatus()){
                        saveMap.insert("stockStatusId", ui->tableWidget_product->item(row, 15)->text().toInt());
                    }
                    if (cPreference->get_XML_upStatus()){
                        saveMap.insert("status", ui->tableWidget_product->item(row, 12)->text().toInt());
                    }
                }
                //qDebug() << saveMap;

                feed = cProduct->saveProduct(saveMap, saveMapOption, saveMapAttribute, saveMapImage);
            }



            if (!feed.value("error").toString().isEmpty()){
                error = feed.value("error").toString();
                break;
            }

            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
            QApplication::processEvents();
        }
    }
    if (!error.isEmpty()){
        makeMessage(error, false);
    } else {
        makeMessage("Товары успешно загружены на сервер!", true);
        product_testServer();
    }

    ui->progressBar->hide();
}

void ocMeridia::product_saveCSV()
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(true);

    QMap<QString, QVariant> saveMap;
    QMap<int, QMap<QString, QVariant> > saveMapOption;
    QMap<int, QMap<QString, QVariant> > saveMapAttribute;
    QMap<int, QMap<QString, QVariant> > saveMapImage;
    QMap<QString,QVariant> feed;
    if (ui->tableWidget_product->rowCount() > 0){
        QString error;

        bool add = true;
        bool up = true;

        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){

            //переменные для логики загрузки
            int _productId = ui->tableWidget_product->item(row, 0)->text().toInt();
            int _groupId = ui->tableWidget_product->item(row, 13)->text().toInt();
            double _price = ui->tableWidget_product->item(row, 6)->text().toDouble();
            //----

            //алгоритм

            if (_productId == 0){ //новые позиции
                up = false;
                add = cPreference->get_CSV_newProduct();
                if (_price == 0){
                    add = cPreference->get_CSV_newNotPrice();
                }
            } else {
                add = false;
                up = true;
                if (_price == 0){
                    up = cPreference->get_CSV_newNotPrice();
                }
            }

            if (ui->radioButton_p_Load_onlyGroup->isChecked()){
                int targetGroup = categoryMap.value(ui->comboBox_prod_group->currentIndex());
                if (targetGroup != _groupId){
                    add = false;
                    up = false;
                }
            } else if (ui->radioButton_p_Load_select->isChecked()){
                if (!ui->tableWidget_product->item(row, 0)->isSelected()){
                    add = false;
                    up = false;
                }
            }

            //формируем карту сохранения
            saveMap.clear();
            saveMapAttribute.clear();
            saveMapOption.clear();
            saveMapImage.clear();
            if (add or up){
                if (add){
                    saveMap.insert("productId", _productId);

                    QString _SKU = ui->tableWidget_product->item(row, 1)->text();

                    saveMap.insert("sku", _SKU);
                    saveMap.insert("model", ui->tableWidget_product->item(row, 2)->text());
                    saveMap.insert("name", ui->tableWidget_product->item(row, 3)->text());
                    saveMap.insert("groupId", _groupId);
                    saveMap.insert("manufacturerId", ui->tableWidget_product->item(row, 14)->text().toInt());
                    saveMap.insert("price", _price);
                    saveMap.insert("quantity", ui->tableWidget_product->item(row, 7)->text().toInt());

                    if (ui->tableWidget_product->item(row, 8)->text().isEmpty()){
                        saveMap.insert("description", ui->tableWidget_product->item(row, 3)->text());
                    } else {
                        saveMap.insert("description", ui->tableWidget_product->item(row, 8)->text());
                    }

                    if (ui->tableWidget_product->item(row, 9)->text().size() > 0){
                        saveMap.insert("image", QString("%1/%2").arg(_DIR).arg(ui->tableWidget_product->item(row, 9)->text()));
                    }
                    saveMap.insert("stockStatusId", ui->tableWidget_product->item(row, 15)->text().toInt());
                    saveMap.insert("status", ui->tableWidget_product->item(row, 12)->text().toInt());
                    saveMap.insert("specPrice", ui->tableWidget_product->item(row, 17)->text().toDouble());

                    saveMap.insert("taxClassId", _TAX);
                    saveMap.insert("languageId", _LANG);

                    //выбираем атрибуты по каждому товару - проверка по артикулу
                    int x = 0;
                    for (int rowA = 0; rowA < ui->tableWidget_prod_attributes->rowCount(); rowA++){
                        if (ui->tableWidget_prod_attributes->item(rowA, 0)->text() == _SKU){

                            QMap<QString, QVariant> _map;
                            _map.insert("productId", ui->tableWidget_prod_attributes->item(rowA, 0)->text());
                            _map.insert("attributeId", ui->tableWidget_prod_attributes->item(rowA, 1)->text());
                            _map.insert("languageId", _LANG);
                            _map.insert("text", ui->tableWidget_prod_attributes->item(rowA, 2)->text());

                            //qDebug() << _map;
                            saveMapAttribute.insert(x, _map);
                            x++;
                        }
                    }
                    //выбираем изображения по каждому товару - проверка по артикулу
                    x = 0;
                    for (int rowA = 0; rowA < ui->tableWidget_prod_image->rowCount(); rowA++){
                        if (ui->tableWidget_prod_image->item(rowA, 0)->text() == _SKU){

                            QMap<QString, QVariant> _map;
                            _map.insert("productId", ui->tableWidget_prod_image->item(rowA, 0)->text());
                            _map.insert("image", QString("%1/%2").arg(_DIR).arg(ui->tableWidget_prod_image->item(rowA, 1)->text()));

                            //qDebug() << _map;
                            saveMapImage.insert(x, _map);
                            x++;
                        }
                    }


                } else if (up){
                    saveMap.insert("productId", _productId);

                    saveMap.insert("specPrice", ui->tableWidget_product->item(row, 17)->text().toDouble());

                    if (cPreference->get_XML_upGroup()){
                        saveMap.insert("groupId", _groupId);
                    } else {
                        saveMap.insert("groupId", 0);
                    }
                    if (ui->tableWidget_product->item(row, 1)->text().size() > 0){
                        saveMap.insert("sku", ui->tableWidget_product->item(row, 1)->text());
                    }
                    if (cPreference->get_XML_upName()){
                        saveMap.insert("name", ui->tableWidget_product->item(row, 3)->text());
                    }
                    if (cPreference->get_XML_upDesc()){
                        saveMap.insert("description", ui->tableWidget_product->item(row, 8)->text());
                    }
                    if (cPreference->get_XML_upManufacturer()){
                        saveMap.insert("manufacturerId", ui->tableWidget_product->item(row, 14)->text().toInt());
                    }
                    if (cPreference->get_XML_upQuan()){
                        saveMap.insert("quantity", ui->tableWidget_product->item(row, 7)->text().toInt());
                    }
                    if (cPreference->get_XML_upPrice()){
                        saveMap.insert("price", _price);
                    }
                    if (cPreference->get_XML_upImage()){
                        saveMap.insert("image", QString("%1/%2").arg(_DIR).arg(ui->tableWidget_product->item(row, 9)->text()));
                    }
                    if (cPreference->get_XML_upStockStatus()){
                        saveMap.insert("stockStatusId", ui->tableWidget_product->item(row, 15)->text().toInt());
                    }
                    if (cPreference->get_XML_upStatus()){
                        saveMap.insert("status", ui->tableWidget_product->item(row, 12)->text().toInt());
                    }

                }
                feed = cProduct->saveProduct(saveMap, saveMapOption, saveMapAttribute, saveMapImage);
            }


            if (!feed.value("error").toString().isEmpty()){
                error = feed.value("error").toString();
                break;
            }

            ui->progressBar->setValue(qFloor((row + 1) * 100 / ui->tableWidget_product->rowCount()));
            QApplication::processEvents();
        }
        if (!error.isEmpty()){
            makeMessage(error, false);
        } else {
            makeMessage("Товары успешно загружены на сервер!", true);
            product_testServer();
        }
    }

    ui->progressBar->hide();
}




void ocMeridia::product_change_group()
{
    for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
        int id = ui->tableWidget_product->item(row, 0)->text().toInt();
        if (id == 0){
            QTableWidgetItem *itemN = new QTableWidgetItem();
            itemN->setText(cCategory->get_road(cPreference->get_XML_newGroupId(), _LANG));
            ui->tableWidget_product->setItem(row, 4, itemN);
            QTableWidgetItem *itemId = new QTableWidgetItem();
            itemId->setText(QString::number(cPreference->get_XML_newGroupId()));
            ui->tableWidget_product->setItem(row, 13, itemId);
        }
    }
}

void ocMeridia::edit_connection()
{
    local *l = new local(db_local, db_server, this);
    l->exec();
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

void ocMeridia::openXML()
{
    // очистка таблиц
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


    cFile->readXML(ui->lineEdit_file->text());

    // загружаем категории
    QMap<int, QMap<QString, QVariant> > mapGroup = cFile->getGroups();
    for (int row = 0; row < mapGroup.count(); row++){
        ui->tableWidget_cat_file->insertRow(row);
        QMap<QString, QVariant> _map = mapGroup.value(row);
        QTableWidgetItem *itemCatId = new QTableWidgetItem(_map.value("code").toString());
        ui->tableWidget_cat_file->setItem(row, 0, itemCatId);
        QTableWidgetItem *itemCatName = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_cat_file->setItem(row, 1, itemCatName);
        QTableWidgetItem *itemCatParent = new QTableWidgetItem(_map.value("parent").toString());
        ui->tableWidget_cat_file->setItem(row, 2, itemCatParent);
    }
    // Загружаем товары основная таблица
    QMap<QString, QMap<QString, QVariant> > mapProd = cFile->getProducts();
    for (int row = 0; row < mapProd.count(); row++){
        ui->tableWidget_product->insertRow(row);
        QMap<QString, QVariant> _map = mapProd.value(mapProd.keys().at(row));
        QTableWidgetItem *itemId = new QTableWidgetItem("0");
        ui->tableWidget_product->setItem(row, 0, itemId);
        QTableWidgetItem *itemArticul = new QTableWidgetItem(_map.value("articul").toString());
        ui->tableWidget_product->setItem(row, 1, itemArticul);
        QTableWidgetItem *itemModel = new QTableWidgetItem(_map.value("model").toString());
        ui->tableWidget_product->setItem(row, 2, itemModel);
        QTableWidgetItem *itemName = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_product->setItem(row, 3, itemName);
        //
        int cID = cCategory->getIdByCode(_map.value("groupId").toString());
        QString cName = "";
        if (cID > 0){
            cName = cCategory->get_road(cID, _LANG);
        }
        QTableWidgetItem *itemGroup = new QTableWidgetItem(cName);
        ui->tableWidget_product->setItem(row, 4, itemGroup);
        QTableWidgetItem *itemManufacturer = new QTableWidgetItem(_map.value("manufacturerId").toString());
        ui->tableWidget_product->setItem(row, 5, itemManufacturer);        
        QTableWidgetItem *itemPrice = new QTableWidgetItem(_map.value("price").toString());
        ui->tableWidget_product->setItem(row, 6, itemPrice);
        QTableWidgetItem *itemQuantity = new QTableWidgetItem(_map.value("quantity").toString());
        ui->tableWidget_product->setItem(row, 7, itemQuantity);
        QTableWidgetItem *itemDesc = new QTableWidgetItem(_map.value("desc").toString());
        ui->tableWidget_product->setItem(row, 8, itemDesc);
        QTableWidgetItem *itemImage = new QTableWidgetItem(_map.value("image").toString());
        ui->tableWidget_product->setItem(row, 9, itemImage);
        QTableWidgetItem *itemStockStatus = new QTableWidgetItem(_map.value("stockStatus").toString());
        ui->tableWidget_product->setItem(row, 10, itemStockStatus);

        QTableWidgetItem *itemCatID = new QTableWidgetItem(QString::number(cID));
        ui->tableWidget_product->setItem(row, 13, itemCatID);
        QTableWidgetItem *item14 = new QTableWidgetItem("0");
        ui->tableWidget_product->setItem(row, 14, item14);
        QTableWidgetItem *item15 = new QTableWidgetItem(_map.value("stockStatusID").toString());
        ui->tableWidget_product->setItem(row, 15, item15);
        QTableWidgetItem *itemCatCode = new QTableWidgetItem(_map.value("groupId").toString());
        ui->tableWidget_product->setItem(row, 16, itemCatCode);
        QTableWidgetItem *itemSPrice = new QTableWidgetItem("0");
        ui->tableWidget_product->setItem(row, 17, itemSPrice);
    }
    // Загружаем изображения товаров
    // Загружаем атрибуты товаров
//    QMap<QString, QMap<QString, QVariant> > mapProdAtt = cFile->getProductAttributes();
//    for (int row = 0; row < mapProdAtt.count(); row++){
//        ui->tableWidget_prod_attributes->insertRow(row);
//        QMap<QString, QVariant> _map = mapProdAtt.value(mapProdAtt.keys().at(row));
//        QTableWidgetItem *itemIdProd = new QTableWidgetItem(mapProdAtt.keys().at(row));
//        ui->tableWidget_prod_attributes->setItem(row, 0, itemIdProd);
//        QTableWidgetItem *itemId = new QTableWidgetItem(_map.value("name").toString());
//        ui->tableWidget_prod_attributes->setItem(row, 1, itemId);
//        QTableWidgetItem *itemValue = new QTableWidgetItem(_map.value("val").toString());
//        ui->tableWidget_prod_attributes->setItem(row, 2, itemValue);
//    }
    // Загружаем опции товаров
    QMap<int, QMap<QString, QVariant> > mapProOption = cFile->getProductOptions();
    for (int row = 0; row < mapProOption.count(); row++){
        ui->tableWidget_prod_option->insertRow(row);
        QMap<QString, QVariant> _map = mapProOption.value(mapProOption.keys().at(row));
        QTableWidgetItem *itemIdProd = new QTableWidgetItem(_map.value("productId").toString());
        ui->tableWidget_prod_option->setItem(row, 0, itemIdProd);
        QTableWidgetItem *itemName = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_prod_option->setItem(row, 1, itemName);
        QTableWidgetItem *itemValue = new QTableWidgetItem(_map.value("value").toString());
        ui->tableWidget_prod_option->setItem(row, 2, itemValue);
        QTableWidgetItem *itemPrice = new QTableWidgetItem(_map.value("price").toString());
        ui->tableWidget_prod_option->setItem(row, 3, itemPrice);
        QTableWidgetItem *itemQuan = new QTableWidgetItem(_map.value("quan").toString());
        ui->tableWidget_prod_option->setItem(row, 4, itemQuan);
    }
    category_testServer();
    options_testServer();

    ui->tabWidget_main->setCurrentWidget(ui->tab_category);

    ui->tableWidget_cat_file->resizeColumnsToContents();
    ui->tableWidget_cat_file->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_product->resizeColumnsToContents();
    ui->tableWidget_product->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_attributes->resizeColumnsToContents();
    ui->tableWidget_prod_attributes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_image->resizeColumnsToContents();
    ui->tableWidget_prod_image->horizontalHeader()->setStretchLastSection(true);

}

void ocMeridia::openCSV()
{
    // очистка таблиц
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
            QTextCodec *codec = QTextCodec::codecForName(ui->comboBox_f_csv_codec->currentText().toUtf8());
            QString _csv = codec->toUnicode(ar);
            csvData.append(_csv);
        }
    }
    file.close();
    //csv_readData();

    cFile->readCSV(csvData, ui->lineEdit_fm_maket->text());

    // загружаем категории
    QMap<int, QMap<QString, QVariant> > mapGroup = cFile->getGroups();
    for (int row = 0; row < mapGroup.count(); row++){
        ui->tableWidget_cat_file->insertRow(row);
        QMap<QString, QVariant> _map = mapGroup.value(row);
        QTableWidgetItem *itemCatId = new QTableWidgetItem(_map.value("code").toString());
        ui->tableWidget_cat_file->setItem(row, 0, itemCatId);
        QTableWidgetItem *itemCatName = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_cat_file->setItem(row, 1, itemCatName);
        QTableWidgetItem *itemCatParent = new QTableWidgetItem(_map.value("parent").toString());
        ui->tableWidget_cat_file->setItem(row, 2, itemCatParent);
    }
    // загружаем атрибуты
    QMap<QString, QMap<QString, QVariant> > mapAttr = cFile->getAttributes();
    for (int row = 0; row < mapAttr.count(); row++){
        ui->tableWidget_fileAttr->insertRow(row);
        QMap<QString, QVariant> _map = mapAttr.value(mapAttr.keys().at(row));
        QTableWidgetItem *item = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_fileAttr->setItem(row, 0, item);
    }
    QMap<QString, QMap<QString, QVariant> > mapProd = cFile->getProducts();
    for (int row = 0; row < mapProd.count(); row++){
        ui->tableWidget_product->insertRow(row);
        QMap<QString, QVariant> _map = mapProd.value(mapProd.keys().at(row));
        QTableWidgetItem *itemId = new QTableWidgetItem("0");
        ui->tableWidget_product->setItem(row, 0, itemId);
        QTableWidgetItem *itemArticul = new QTableWidgetItem(_map.value("articul").toString());
        ui->tableWidget_product->setItem(row, 1, itemArticul);
        QTableWidgetItem *itemModel = new QTableWidgetItem(_map.value("model").toString());
        ui->tableWidget_product->setItem(row, 2, itemModel);
        QTableWidgetItem *itemName = new QTableWidgetItem(_map.value("name").toString());
        ui->tableWidget_product->setItem(row, 3, itemName);
        //
        int cID = cCategory->getIdByCode(_map.value("groupId").toString());
        QString cName = "";
        if (cID > 0){
            cName = cCategory->get_road(cID, _LANG);
        }
        QTableWidgetItem *itemGroup = new QTableWidgetItem(cName);
        ui->tableWidget_product->setItem(row, 4, itemGroup);
        QTableWidgetItem *itemManufacturer = new QTableWidgetItem(_map.value("manufacturerId").toString());
        ui->tableWidget_product->setItem(row, 5, itemManufacturer);
        QTableWidgetItem *itemPrice = new QTableWidgetItem(QString::number(_map.value("price").toDouble(), 'f', 2));
        ui->tableWidget_product->setItem(row, 6, itemPrice);
        QTableWidgetItem *itemQuantity = new QTableWidgetItem(_map.value("quantity").toString());
        ui->tableWidget_product->setItem(row, 7, itemQuantity);
        QTableWidgetItem *itemDesc = new QTableWidgetItem(_map.value("desc").toString());
        ui->tableWidget_product->setItem(row, 8, itemDesc);
        QTableWidgetItem *itemImage = new QTableWidgetItem(_map.value("image").toString());
        ui->tableWidget_product->setItem(row, 9, itemImage);
        QTableWidgetItem *itemStockStatus = new QTableWidgetItem(_map.value("stockStatus").toString());
        ui->tableWidget_product->setItem(row, 10, itemStockStatus);

        QTableWidgetItem *itemCatID = new QTableWidgetItem(QString::number(cID));
        ui->tableWidget_product->setItem(row, 13, itemCatID);
        QTableWidgetItem *item14 = new QTableWidgetItem("0");
        ui->tableWidget_product->setItem(row, 14, item14);
        QTableWidgetItem *item15 = new QTableWidgetItem(QString::number(_map.value("stockStatusId").toInt()));
        ui->tableWidget_product->setItem(row, 15, item15);
        QTableWidgetItem *itemCatCode = new QTableWidgetItem(_map.value("groupId").toString());
        ui->tableWidget_product->setItem(row, 16, itemCatCode);
        QTableWidgetItem *itemSPrice = new QTableWidgetItem(QString::number(_map.value("specPrice").toDouble(), 'f', 2));
        ui->tableWidget_product->setItem(row, 17, itemSPrice);
    }
    //атрибуты товаров
    QMap<int, QMap<QString, QVariant> > mapProductAttribute = cFile->getProductAttributes();
    for (int x = 0; x < mapProductAttribute.count(); x++){
        QMap<QString,QVariant> _map = mapProductAttribute.value(x);

        ui->tableWidget_prod_attributes->insertRow(x);
        QTableWidgetItem *itemID = new QTableWidgetItem(_map.value("productId").toString());
        ui->tableWidget_prod_attributes->setItem(x, 0, itemID);
        QTableWidgetItem *itemN = new QTableWidgetItem(_map.value("attributeId").toString());
        ui->tableWidget_prod_attributes->setItem(x, 1, itemN);
        QTableWidgetItem *itemT = new QTableWidgetItem(_map.value("text").toString());
        ui->tableWidget_prod_attributes->setItem(x, 2, itemT);
    }
    //изображения товаров
    QMap<int, QMap<QString, QVariant> > mapProductImage = cFile->getProductImages();
    for (int x = 0; x < mapProductImage.count(); x++){
        QMap<QString,QVariant> _map = mapProductImage.value(x);
        ui->tableWidget_prod_image->insertRow(x);
        QTableWidgetItem *itemID = new QTableWidgetItem(_map.value("productId").toString());
        ui->tableWidget_prod_image->setItem(x, 0, itemID);
        QTableWidgetItem *itemN = new QTableWidgetItem(_map.value("image").toString());
        ui->tableWidget_prod_image->setItem(x, 1, itemN);
    }

    category_testServer();
    attribute_testServer();


    ui->tableWidget_cat_file->resizeColumnsToContents();
    ui->tableWidget_cat_file->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_product->resizeColumnsToContents();
    ui->tableWidget_product->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_attributes->resizeColumnsToContents();
    ui->tableWidget_prod_attributes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_prod_image->resizeColumnsToContents();
    ui->tableWidget_prod_image->horizontalHeader()->setStretchLastSection(true);
}

void ocMeridia::remakeStatus()
{
    if (ui->tableWidget_product->rowCount() > 0){
        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            if (ui->radioButton_p_mStatus_all->isChecked()){
                QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mStatus->currentText());
                ui->tableWidget_product->setItem(row, 11, itemName);
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->comboBox_p_mStatus->currentIndex()));
                ui->tableWidget_product->setItem(row, 12, itemID);
            } else if (ui->radioButton_p_mStatus_new->isChecked()){
                int pID = ui->tableWidget_product->item(row, 0)->text().toInt();
                if (pID == 0){
                    QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mStatus->currentText());
                    ui->tableWidget_product->setItem(row, 11, itemName);
                    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->comboBox_p_mStatus->currentIndex()));
                    ui->tableWidget_product->setItem(row, 12, itemID);
                }
            } else if (ui->radioButton_p_mStatus_select->isChecked()){
                if (ui->tableWidget_product->item(row, 0)->isSelected()){
                    QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mStatus->currentText());
                    ui->tableWidget_product->setItem(row, 11, itemName);
                    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(ui->comboBox_p_mStatus->currentIndex()));
                    ui->tableWidget_product->setItem(row, 12, itemID);
                }
            }
        }
    }

}

void ocMeridia::remakeGroup()
{
    if (ui->tableWidget_product->rowCount() > 0){
        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            if (ui->radioButton_p_mGroup_all->isChecked()){
                QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mGroup->currentText());
                ui->tableWidget_product->setItem(row, 4, itemName);
                QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(categoryMap.value(ui->comboBox_p_mGroup->currentIndex())));
                ui->tableWidget_product->setItem(row, 13, itemID);
            } else if (ui->radioButton_p_mGroup_new->isChecked()){
                int pID = ui->tableWidget_product->item(row, 0)->text().toInt();
                if (pID == 0){
                    QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mGroup->currentText());
                    ui->tableWidget_product->setItem(row, 4, itemName);
                    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(categoryMap.value(ui->comboBox_p_mGroup->currentIndex())));
                    ui->tableWidget_product->setItem(row, 13, itemID);
                }
            } else if (ui->radioButton_p_mGroup_select->isChecked()){
                if (ui->tableWidget_product->item(row, 0)->isSelected()){
                    QTableWidgetItem *itemName = new QTableWidgetItem(ui->comboBox_p_mGroup->currentText());
                    ui->tableWidget_product->setItem(row, 4, itemName);
                    QTableWidgetItem *itemID = new QTableWidgetItem(QString::number(categoryMap.value(ui->comboBox_p_mGroup->currentIndex())));
                    ui->tableWidget_product->setItem(row, 13, itemID);
                }
            }
        }
    }
}

void ocMeridia::remakeQuantity()
{
    if (ui->tableWidget_product->rowCount() > 0){
        for (int row = 0; row < ui->tableWidget_product->rowCount(); row++){
            if (ui->radioButton_p_mQ_all->isChecked()){
                QTableWidgetItem *itemQ = new QTableWidgetItem(QString::number(ui->spinBox_p_mQ->value()));
                ui->tableWidget_product->setItem(row, 7, itemQ);
            } else if (ui->radioButton_p_mQ_new->isChecked()){
                int pID = ui->tableWidget_product->item(row, 0)->text().toInt();
                if (pID == 0){
                    QTableWidgetItem *itemQ = new QTableWidgetItem(QString::number(ui->spinBox_p_mQ->value()));
                    ui->tableWidget_product->setItem(row, 7, itemQ);
                }
            } else if (ui->radioButton_p_mQ_select->isChecked()){
                if (ui->tableWidget_product->item(row, 0)->isSelected()){
                    QTableWidgetItem *itemQ = new QTableWidgetItem(QString::number(ui->spinBox_p_mQ->value()));
                    ui->tableWidget_product->setItem(row, 7, itemQ);
                }
            }
        }
    }
}

void ocMeridia::on_aPref_triggered()
{
    Pref *p = new Pref(db_server,db_local, this);
    p->exec();

    readSetting();
}
