#include "ctrlpreference.h"

preference::preference()
{
    SETT = new QSettings("setting.ini", QSettings::IniFormat);
}

QString preference::get_DB_name()
{
    return SETT->value("db/name").toString();
}

QString preference::get_DB_host()
{
    return SETT->value("db/host").toString();
}

int preference::get_DB_port()
{
    return SETT->value("db/port").toInt();
}

QString preference::get_DB_user()
{
    return SETT->value("db/user").toString();
}

QString preference::get_DB_pass()
{
    return SETT->value("db/pass").toString();
}

void preference::save_DB_name(QString _v)
{
    SETT->beginGroup("db");
    SETT->setValue("name", _v);
    SETT->endGroup();
}

void preference::save_DB_host(QString _v)
{
    SETT->beginGroup("db");
    SETT->setValue("host", _v);
    SETT->endGroup();
}

void preference::save_DB_port(int _v)
{
    SETT->beginGroup("db");
    SETT->setValue("port", _v);
    SETT->endGroup();
}

void preference::save_DB_user(QString _v)
{
    SETT->beginGroup("db");
    SETT->setValue("user", _v);
    SETT->endGroup();
}

void preference::save_DB_pass(QString _v)
{
    SETT->beginGroup("db");
    SETT->setValue("pass", _v);
    SETT->endGroup();
}

int preference::get_OC_lang()
{
    return SETT->value("oc/lang").toInt();
}

int preference::get_OC_tax()
{
    return SETT->value("oc/tax").toInt();
}

int preference::get_OC_quan()
{
    return SETT->value("oc/quan").toInt();
}

QString preference::get_OC_dir()
{
    return SETT->value("oc/dir").toString();
}

void preference::save_OC_lang(int _v)
{
    SETT->beginGroup("oc");
    SETT->setValue("lang", _v);
    SETT->endGroup();
}

void preference::save_OC_tax(int _v)
{
    SETT->beginGroup("oc");
    SETT->setValue("tax", _v);
    SETT->endGroup();
}

void preference::save_OC_quan(int _v)
{
    SETT->beginGroup("oc");
    SETT->setValue("quan", _v);
    SETT->endGroup();
}

void preference::save_OC_dir(QString _v)
{
    SETT->beginGroup("oc");
    SETT->setValue("dir", _v);
    SETT->endGroup();
}

int preference::get_XML_group()
{
    return SETT->value("load1c/group").toInt();
}

int preference::get_XML_attribute()
{
    return SETT->value("load1c/attribute").toInt();
}

bool preference::get_XML_newProduct()
{
    return SETT->value("load1c/new_product").toBool();
}

bool preference::get_XML_newGroup()
{
    return SETT->value("load1c/new_group").toBool();
}

int preference::get_XML_newGroupId()
{
    return SETT->value("load1c/new_group_id").toInt();
}

bool preference::get_XML_newNotPrice()
{
    return SETT->value("load1c/new_notPrice").toBool();
}

int preference::get_XML_newStatusId()
{
    return SETT->value("load1c/new_status").toInt();
}

int preference::get_XML_newQuan()
{
    return SETT->value("load1c/new_quan").toInt();
}

bool preference::get_XML_specPrice()
{
    return SETT->value("load1c/specPrice").toBool();
}

bool preference::get_XML_specSkidka()
{
    return SETT->value("load1c/specSkidka").toBool();
}

double preference::get_XML_specSkidkaValue()
{
    return SETT->value("load1c/specSkidkaValue").toDouble();
}

bool preference::get_XML_upGroup()
{
    return SETT->value("load1c/up_group").toBool();
}

bool preference::get_XML_upName()
{
    return SETT->value("load1c/up_name").toBool();
}

bool preference::get_XML_upDesc()
{
    return SETT->value("load1c/up_desc").toBool();
}

bool preference::get_XML_upManufacturer()
{
    return SETT->value("load1c/up_manuf").toBool();
}

bool preference::get_XML_upImage()
{
    return SETT->value("load1c/up_image").toBool();
}

bool preference::get_XML_upPrice()
{
    return SETT->value("load1c/up_price").toBool();
}

bool preference::get_XML_upQuan()
{
    return SETT->value("load1c/up_quan").toBool();
}

bool preference::get_XML_upAttribute()
{
    return SETT->value("load1c/up_attribute").toBool();
}

bool preference::get_XML_upOption()
{
    return SETT->value("load1c/up_option").toBool();
}

bool preference::get_XML_upStockStatus()
{
    return SETT->value("load1c/up_stockStatus").toBool();
}

bool preference::get_XML_upStatus()
{
    return SETT->value("load1c/up_status").toBool();
}

void preference::save_XML_group(int _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("group", _v);
    SETT->endGroup();
}

void preference::save_XML_attribute(int _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("attribute", _v);
    SETT->endGroup();
}

void preference::save_XML_newProduct(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_product", _v);
    SETT->endGroup();
}

void preference::save_XML_newNotPrice(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_notPrice", _v);
    SETT->endGroup();
}

void preference::save_XML_newGroup(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_group", _v);
    SETT->endGroup();
}

void preference::save_XML_newGroupId(int _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_group_id", _v);
    SETT->endGroup();
}

void preference::save_XML_newStatusId(int _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_status_id", _v);
    SETT->endGroup();
}

void preference::save_XML_newQuan(int _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("new_quan", _v);
    SETT->endGroup();
}

void preference::save_XML_specPrice(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("specPrice", _v);
    SETT->endGroup();
}

void preference::save_XML_specSkidka(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("specSkidka", _v);
    SETT->endGroup();
}

void preference::save_XML_specSkidkaValue(double _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("specSkidkaValue", _v);
    SETT->endGroup();
}

void preference::save_XML_upGroup(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_group", _v);
    SETT->endGroup();
}



void preference::save_XML_upName(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_name", _v);
    SETT->endGroup();
}

void preference::save_XML_upDesc(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_desc", _v);
    SETT->endGroup();
}

void preference::save_XML_upManufacturer(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_manuf", _v);
    SETT->endGroup();
}

void preference::save_XML_upImage(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_image", _v);
    SETT->endGroup();
}

void preference::save_XML_upPrice(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_price", _v);
    SETT->endGroup();
}

void preference::save_XML_upQuan(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_quan", _v);
    SETT->endGroup();
}

void preference::save_XML_upAttribute(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_attribute", _v);
    SETT->endGroup();
}

void preference::save_XML_upOption(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_option", _v);
    SETT->endGroup();
}

void preference::save_XML_upStockStatus(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_stockStatus", _v);
    SETT->endGroup();
}

void preference::save_XML_upStatus(bool _v)
{
    SETT->beginGroup("load1c");
    SETT->setValue("up_status", _v);
    SETT->endGroup();
}

int preference::get_CSV_group()
{
    return SETT->value("csv/group").toInt();
}

int preference::get_CSV_attribute()
{
    return SETT->value("csv/attribute").toInt();
}

bool preference::get_CSV_newProduct()
{
    return SETT->value("csv/new_product").toBool();
}

bool preference::get_CSV_newNotPrice()
{
    return SETT->value("csv/new_notPrice").toBool();
}

bool preference::get_CSV_newGroup()
{
    return SETT->value("csv/new_group").toBool();
}

int preference::get_CSV_newGroupId()
{
    return SETT->value("csv/new_groupId").toInt();
}

int preference::get_CSV_newQuan()
{
    return SETT->value("csv/new_quantity").toInt();
}

int preference::get_CSV_newStockStatusId()
{
    return SETT->value("csv/new_stockStatusId").toInt();
}

int preference::get_CSV_newStatusId()
{
    return SETT->value("csv/new_status").toInt();
}

bool preference::get_CSV_upGroup()
{
    return SETT->value("csv/up_group").toBool();
}

bool preference::get_CSV_upName()
{
    return SETT->value("csv/up_name").toBool();
}

bool preference::get_CSV_upDesc()
{
    return SETT->value("csv/up_desc").toBool();
}

bool preference::get_CSV_upImage()
{
    return SETT->value("csv/up_image").toBool();
}

bool preference::get_CSV_upManufacturer()
{
    return SETT->value("csv/up_manuf").toBool();
}


bool preference::get_CSV_upPrice()
{
    return SETT->value("loadpl/up_price").toBool();
}

bool preference::get_CSV_upQuan()
{
    return SETT->value("csv/up_quan").toBool();
}

bool preference::get_CSV_upAttribute()
{
    return SETT->value("csv/up_attribute").toBool();
}

bool preference::get_CSV_upOption()
{
    return SETT->value("csv/up_option").toBool();
}

bool preference::get_CSV_upStockStatus()
{
    return SETT->value("csv/up_stockStatus").toBool();
}

bool preference::get_CSV_upStatus()
{
    return SETT->value("csv/up_status").toBool();
}

void preference::save_CSV_group(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("group", _v);
    SETT->endGroup();
}

void preference::save_CSV_attribute(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("attribute", _v);
    SETT->endGroup();
}

void preference::save_CSV_newProduct(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_product", _v);
    SETT->endGroup();
}

void preference::save_CSV_newNotPrice(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_notPrice", _v);
    SETT->endGroup();
}

void preference::save_CSV_newGroup(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_group", _v);
    SETT->endGroup();
}

void preference::save_CSV_newGroupId(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_groupId", _v);
    SETT->endGroup();
}

void preference::save_CSV_newQuan(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_quantity", _v);
    SETT->endGroup();
}

void preference::save_CSV_newStockStatusId(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_stockStatusId", _v);
    SETT->endGroup();
}

void preference::save_CSV_newStatusId(int _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("new_statusId", _v);
    SETT->endGroup();
}

void preference::save_CSV_upGroup(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_group", _v);
    SETT->endGroup();
}

void preference::save_CSV_upName(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_name", _v);
    SETT->endGroup();
}

void preference::save_CSV_upDesc(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_desc", _v);
    SETT->endGroup();
}

void preference::save_CSV_upManufacturer(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_manuf", _v);
    SETT->endGroup();
}

void preference::save_CSV_upImage(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_image", _v);
    SETT->endGroup();
}

void preference::save_CSV_upPrice(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_price", _v);
    SETT->endGroup();
}



void preference::save_CSV_upQuan(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_quan", _v);
    SETT->endGroup();
}

void preference::save_CSV_upAttribute(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_attribute", _v);
    SETT->endGroup();
}

void preference::save_CSV_upOption(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_option", _v);
    SETT->endGroup();
}

void preference::save_CSV_upStockStatus(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_stockStatus", _v);
    SETT->endGroup();
}

void preference::save_CSV_upStatus(bool _v)
{
    SETT->beginGroup("csv");
    SETT->setValue("up_status", _v);
    SETT->endGroup();
}

bool preference::get_AUTO_active()
{
    return SETT->value("auto/active").toBool();
}

void preference::save_AUTO_active(bool _v)
{
    SETT->beginGroup("auto");
    SETT->setValue("active", _v);
    SETT->endGroup();
}

