#include "pref.h"
#include "ui_pref.h"

Pref::Pref(QSqlDatabase db_server, QSqlDatabase db_local, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pref)
{
    ui->setupUi(this);
    sDB = db_server;
    lDB = db_local;

    cPref = new preference();
    LANG = cPref->get_OC_lang();
    cCategory = new ctrlSCategory(sDB, "rmrt", 0);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->groupBox_messa, SLOT(hide()));
    ui->groupBox_messa->hide();



    loadLang();
    loadAttribute();
    loadGroup();
    loadTax();
    loadStock();


    load();

    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(save()));
    connect(ui->pushButton_save, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pushButton_change, SIGNAL(clicked(bool)), this, SLOT(changeFile()));
    connect(ui->pushButton_test, SIGNAL(clicked(bool)), this, SLOT(test_connection()));
}

Pref::~Pref()
{
    delete ui;
}

void Pref::loadLang()
{
    langMap.clear();
    ui->comboBox_sB_lang_2->clear();
    QSqlQuery qLang("SELECT rmrt_language.language_id, rmrt_language.name FROM rmrt_language ", sDB);
    while (qLang.next()){
        ui->comboBox_sB_lang_2->addItem(qLang.value(1).toString());
        langMap.insert(ui->comboBox_sB_lang_2->count() - 1, qLang.value(0).toInt());
    }
}

void Pref::loadTax()
{
    taxMap.clear();
    ui->comboBox_sB_tax_2->clear();
    QSqlQuery qTaxClass("SELECT rmrt_tax_class.tax_class_id, rmrt_tax_class.title FROM rmrt_tax_class", sDB);
    while (qTaxClass.next()){
        ui->comboBox_sB_tax_2->addItem(qTaxClass.value(1).toString());
        taxMap.insert(ui->comboBox_sB_tax_2->count() - 1, qTaxClass.value(0).toInt());
    }
}

void Pref::loadGroup()
{
    groupsMap.clear();
    ui->comboBox_1C_group->clear();
    ui->comboBox_1C_new_group->clear();
    ui->comboBox_sCpl_group->clear();
    ui->comboBox_CSV_new_group->clear();

    QMap<int, QMap<QString,QVariant> > MAP = cCategory->get_categories();
    for (int x = 0; x < MAP.count(); x++){
        QMap<QString,QVariant> _map = MAP.value(x);

        int id = _map.value("categoryId").toInt();
        QString name = cCategory->get_road(id, LANG);

        ui->comboBox_1C_group->addItem(name);
        ui->comboBox_1C_new_group->addItem(name);
        ui->comboBox_sCpl_group->addItem(name);
        ui->comboBox_CSV_new_group->addItem(name);
        groupsMap.insert(x, id);
    }
}

void Pref::loadAttribute()
{
    attributeMap.clear();
    ui->comboBox_sCpl_att->clear();
    ui->comboBox_1C_att->clear();
    QSqlQuery queryGroup(QString("SELECT rmrt_attribute_group_description.name, "
                         "rmrt_attribute_group_description.attribute_group_id "
                         "FROM rmrt_attribute_group_description "
                         "WHERE rmrt_attribute_group_description.language_id = \'%1\' ")
                         .arg(LANG), sDB);
    while (queryGroup.next()){
        attributeMap.insert(ui->comboBox_1C_att->count(), queryGroup.value(1).toInt());
        ui->comboBox_sCpl_att->addItem(queryGroup.value(0).toString());
        ui->comboBox_1C_att->addItem(queryGroup.value(0).toString());
    }
}

void Pref::loadStock()
{
    stockMap.clear();
    ui->comboBox_CSV_new_stock->clear();
    QSqlQuery query(QString("SELECT rmrt_stock_status.stock_status_id, rmrt_stock_status.name "
                         "FROM rmrt_stock_status "
                         "WHERE rmrt_stock_status.language_id = \'%1\' ")
                         .arg(LANG), sDB);
    while (query.next()){
        stockMap.insert(ui->comboBox_CSV_new_stock->count(), query.value(0).toInt());
        ui->comboBox_CSV_new_stock->addItem(query.value(1).toString());
    }
}

void Pref::load()
{
    //DB
    ui->lineEdit_sA_dbname->setText(cPref->get_DB_name());
    ui->lineEdit_sA_host->setText(cPref->get_DB_host());
    ui->spinBox_sA_port->setValue(cPref->get_DB_port());
    ui->lineEdit_sA_username->setText(cPref->get_DB_user());
    ui->lineEdit_sA_pass->setText(cPref->get_DB_pass());

    //OC
    ui->comboBox_sB_lang_2->setCurrentIndex(langMap.key(cPref->get_OC_lang()));
    ui->comboBox_sB_tax_2->setCurrentIndex(taxMap.key(cPref->get_OC_tax()));
    //ui->doubleSpinBox_sB_quan_2->setValue(cPref->get_OC_quan());
    ui->lineEdit_sB_dir_2->setText(cPref->get_OC_dir());

    //XML
    ui->comboBox_1C_group->setCurrentIndex(groupsMap.key(cPref->get_XML_group()));
    ui->comboBox_1C_att->setCurrentIndex(attributeMap.key(cPref->get_XML_attribute()));

    ui->checkBox_1C_new_product->setChecked(cPref->get_XML_newProduct());
    ui->checkBox_1C_new_notPrice->setChecked(cPref->get_XML_newNotPrice());
    ui->checkBox_1C_new_group->setChecked(cPref->get_XML_newGroup());
    ui->comboBox_1C_new_group->setCurrentIndex(groupsMap.key(cPref->get_XML_newGroupId()));
    ui->comboBox_1C_new_status->setCurrentIndex(cPref->get_XML_newStatusId());
    ui->spinBox_1C_new_quan->setValue(cPref->get_XML_newQuan());

    ui->groupBox_1C_sprice->setChecked(cPref->get_XML_specPrice());
    ui->checkBox_1C_skidka->setChecked(cPref->get_XML_specSkidka());
    ui->doubleSpinBox_1C_skidka->setValue(cPref->get_XML_specSkidkaValue());

    ui->checkBox_1C_up_group->setChecked(cPref->get_XML_upGroup());
    ui->checkBox_1C_up_name->setChecked(cPref->get_XML_upName());
    ui->checkBox_1C_up_manuf->setChecked(cPref->get_XML_upManufacturer());
    ui->checkBox_1C_up_price->setChecked(cPref->get_XML_upPrice());
    ui->checkBox_1C_up_quan->setChecked(cPref->get_XML_upQuan());
    ui->checkBox_1C_up_desc->setChecked(cPref->get_XML_upDesc());
    ui->checkBox_1C_up_image->setChecked(cPref->get_XML_upImage());
    ui->checkBox_1C_up_attribute->setChecked(cPref->get_XML_upAttribute());
    ui->checkBox_1C_up_option->setChecked(cPref->get_XML_upOption());
    ui->checkBox_1C_up_stock->setChecked(cPref->get_XML_upStockStatus());
    ui->checkBox_1C_up_status->setChecked(cPref->get_XML_upStatus());

    //CVS
    ui->comboBox_sCpl_group->setCurrentIndex(groupsMap.key(cPref->get_CSV_group()));
    ui->comboBox_sCpl_att->setCurrentIndex(attributeMap.key(cPref->get_CSV_attribute()));
    ui->checkBox_CSV_new_product->setChecked(cPref->get_CSV_newProduct());
    ui->checkBox_CSV_new_notPrice->setChecked(cPref->get_CSV_newNotPrice());
    ui->checkBox_CSV_new_group->setChecked(cPref->get_CSV_newGroup());
    ui->comboBox_CSV_new_group->setCurrentIndex(groupsMap.key(cPref->get_CSV_newGroupId()));
    ui->comboBox_CSV_new_stock->setCurrentIndex(stockMap.key(cPref->get_CSV_newStockStatusId()));
    ui->comboBox_CSV_new_status->setCurrentIndex(cPref->get_CSV_newStatusId());
    ui->spinBox_CSV_new_quan->setValue(cPref->get_CSV_newQuan());


    ui->checkBox_CSV_up_group->setChecked(cPref->get_CSV_upGroup());
    ui->checkBox_CSV_up_name->setChecked(cPref->get_CSV_upName());
    ui->checkBox_CSV_up_manuf->setChecked(cPref->get_CSV_upManufacturer());
    ui->checkBox_CSV_up_price->setChecked(cPref->get_CSV_upPrice());
    ui->checkBox_CSV_up_quan->setChecked(cPref->get_CSV_upQuan());
    ui->checkBox_CSV_up_desc->setChecked(cPref->get_CSV_upDesc());
    ui->checkBox_CSV_up_image->setChecked(cPref->get_CSV_upImage());
    ui->checkBox_CSV_up_attribute->setChecked(cPref->get_CSV_upAttribute());
    ui->checkBox_CSV_up_option->setChecked(cPref->get_CSV_upOption());
    ui->checkBox_CSV_up_stock->setChecked(cPref->get_CSV_upStockStatus());
    ui->checkBox_CSV_up_status->setChecked(cPref->get_CSV_upStatus());

    //AUTO
    ui->checkBox_autoActive->setChecked(cPref->get_AUTO_active());
    //----auto file
    loadAutoFile();
}

void Pref::save()
{
    //DB
    cPref->save_DB_name(ui->lineEdit_sA_dbname->text());
    cPref->save_DB_host(ui->lineEdit_sA_host->text());
    cPref->save_DB_port(ui->spinBox_sA_port->value());
    cPref->save_DB_user(ui->lineEdit_sA_username->text());
    cPref->save_DB_pass(ui->lineEdit_sA_pass->text());

    //OC
    cPref->save_OC_lang(langMap.value(ui->comboBox_sB_lang_2->currentIndex()));
    cPref->save_OC_tax(taxMap.value(ui->comboBox_sB_tax_2->currentIndex()));
    //cPref->save_OC_quan(ui->doubleSpinBox_sB_quan_2->value());
    cPref->save_OC_dir(ui->lineEdit_sB_dir_2->text());

    //XML
    cPref->save_XML_group(groupsMap.value(ui->comboBox_1C_group->currentIndex()));
    cPref->save_XML_attribute(attributeMap.value(ui->comboBox_1C_att->currentIndex()));

    cPref->save_XML_newProduct(ui->checkBox_1C_new_product->isChecked());
    cPref->save_XML_newNotPrice(ui->checkBox_1C_new_notPrice->isChecked());
    cPref->save_XML_newGroup(ui->checkBox_1C_new_group->isChecked());
    cPref->save_XML_newGroupId(groupsMap.value(ui->comboBox_1C_new_group->currentIndex()));
    cPref->save_XML_newStatusId(ui->comboBox_1C_new_status->currentIndex());
    cPref->save_XML_newQuan(ui->spinBox_1C_new_quan->value());

    cPref->save_XML_specPrice(ui->groupBox_1C_sprice->isChecked());
    cPref->save_XML_specSkidka(ui->checkBox_1C_skidka->isChecked());
    cPref->save_XML_specSkidkaValue(ui->doubleSpinBox_1C_skidka->value());

    cPref->save_XML_upGroup(ui->checkBox_1C_up_group->isChecked());
    cPref->save_XML_upName(ui->checkBox_1C_up_name->isChecked());
    cPref->save_XML_upManufacturer(ui->checkBox_1C_up_manuf->isChecked());
    cPref->save_XML_upPrice(ui->checkBox_1C_up_price->isChecked());
    cPref->save_XML_upQuan(ui->checkBox_1C_up_quan->isChecked());
    cPref->save_XML_upDesc(ui->checkBox_1C_up_desc->isChecked());
    cPref->save_XML_upImage(ui->checkBox_1C_up_image->isChecked());
    cPref->save_XML_upAttribute(ui->checkBox_1C_up_attribute->isChecked());
    cPref->save_XML_upOption(ui->checkBox_1C_up_option->isChecked());
    cPref->save_XML_upStockStatus(ui->checkBox_1C_up_stock->isChecked());
    cPref->save_XML_upStatus(ui->checkBox_1C_up_status->isChecked());


    //CVS
    cPref->save_CSV_group(groupsMap.value(ui->comboBox_sCpl_group->currentIndex()));
    cPref->save_CSV_attribute(attributeMap.value(ui->comboBox_sCpl_att->currentIndex()));
    cPref->save_CSV_newProduct(ui->checkBox_CSV_new_product->isChecked());
    cPref->save_CSV_newNotPrice(ui->checkBox_CSV_new_notPrice->isChecked());
    cPref->save_CSV_newGroup(ui->checkBox_CSV_new_group->isChecked());
    cPref->save_CSV_newGroupId(groupsMap.value(ui->comboBox_CSV_new_group->currentIndex()));
    cPref->save_CSV_newQuan(ui->spinBox_CSV_new_quan->value());
    cPref->save_CSV_newStatusId(ui->comboBox_CSV_new_status->currentIndex());
    cPref->save_CSV_newStockStatusId(stockMap.value(ui->comboBox_CSV_new_stock->currentIndex()));

    cPref->save_CSV_upGroup(ui->checkBox_CSV_up_group->isChecked());
    cPref->save_CSV_upName(ui->checkBox_CSV_up_name->isChecked());
    cPref->save_CSV_upManufacturer(ui->checkBox_CSV_up_manuf->isChecked());
    cPref->save_CSV_upPrice(ui->checkBox_CSV_up_price->isChecked());
    cPref->save_CSV_upQuan(ui->checkBox_CSV_up_quan->isChecked());
    cPref->save_CSV_upDesc(ui->checkBox_CSV_up_desc->isChecked());
    cPref->save_CSV_upImage(ui->checkBox_CSV_up_image->isChecked());
    cPref->save_CSV_upAttribute(ui->checkBox_CSV_up_attribute->isChecked());
    cPref->save_CSV_upOption(ui->checkBox_CSV_up_option->isChecked());
    cPref->save_CSV_upStockStatus(ui->checkBox_CSV_up_stock->isChecked());
    cPref->save_CSV_upStatus(ui->checkBox_CSV_up_status->isChecked());

    //AUTO
    cPref->save_AUTO_active(ui->checkBox_autoActive->isChecked());



}

void Pref::loadAutoFile()
{
    //--main
    QSqlQuery queryMainFile("SELECT files.fname FROM files WHERE files.vid = \'2\' ", lDB);
    queryMainFile.next();
    if (queryMainFile.isValid()){
        ui->lineEdit_sD_dir1c->setText(queryMainFile.value(0).toString());
    }
    //--pl
    for (int row = ui->tableWidget_priceFiles->rowCount() - 1; row >= 0; row--){
        ui->tableWidget_priceFiles->removeRow(row);
    }
    QSqlQuery queryPriceFile("SELECT files.id, files.fname "
                             "FROM files "
                             "WHERE files.vid = \'0\' AND files.active = \'1\' ", lDB);
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
    ui->tableWidget_priceFiles->setColumnHidden(0, true);
    //
}

void Pref::changeFile()
{
    setting *se = new setting(lDB, this);
    se->exec();
    loadAutoFile();
}

void Pref::test_connection()
{
    tDB = QSqlDatabase::addDatabase("QMYSQL", "testDB");
    tDB.setDatabaseName(ui->lineEdit_sA_dbname->text());
    tDB.setHostName(ui->lineEdit_sA_host->text());
    tDB.setPort(ui->spinBox_sA_port->value());
    tDB.setUserName(ui->lineEdit_sA_username->text());
    tDB.setPassword(ui->lineEdit_sA_pass->text());
    tDB.setConnectOptions("MYSQL_OPT_RECONNECT=TRUE;");
    tDB.open();
    if (tDB.isOpen()){
        message("К удаленной БД подключено", true);
        sDB = tDB;
        save();
        loadAttribute();
        loadGroup();
        loadLang();
        loadTax();
        load();
    } else {
        message("К удаленной БД не подключено, проверьте настройки", false);
    }
}

void Pref::message(QString str, bool x)
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
