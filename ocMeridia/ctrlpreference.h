#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QSettings>

class preference
{
public:
    preference();

    QSettings *SETT;

public slots:
    QString get_DB_name();
    QString get_DB_host();
    int get_DB_port();
    QString get_DB_user();
    QString get_DB_pass();

    void save_DB_name(QString _v);
    void save_DB_host(QString _v);
    void save_DB_port(int _v);
    void save_DB_user(QString _v);
    void save_DB_pass(QString _v);

    int get_OC_lang();
    int get_OC_tax();
    int get_OC_quan();
    QString get_OC_dir();

    void save_OC_lang(int _v);
    void save_OC_tax(int _v);
    void save_OC_quan(int _v);
    void save_OC_dir(QString _v);

    int get_XML_group();
    int get_XML_attribute();

    bool get_XML_newProduct();
    bool get_XML_newGroup();
    int get_XML_newGroupId();
    bool get_XML_newNotPrice();
    int get_XML_newStatusId();
    int get_XML_newQuan();

    bool get_XML_specPrice();
    bool get_XML_specSkidka();
    double get_XML_specSkidkaValue();

    bool get_XML_upGroup();
    bool get_XML_upName();
    bool get_XML_upDesc();
    bool get_XML_upManufacturer();
    bool get_XML_upImage();
    bool get_XML_upPrice();
    bool get_XML_upQuan();
    bool get_XML_upAttribute();
    bool get_XML_upOption();
    bool get_XML_upStockStatus();
    bool get_XML_upStatus();

    void save_XML_group(int _v);
    void save_XML_attribute(int _v);

    void save_XML_newProduct(bool _v);
    void save_XML_newNotPrice(bool _v);
    void save_XML_newGroup(bool _v);
    void save_XML_newGroupId(int _v);
    void save_XML_newStatusId(int _v);
    void save_XML_newQuan(int _v);

    void save_XML_specPrice(bool _v);
    void save_XML_specSkidka(bool _v);
    void save_XML_specSkidkaValue(double _v);

    void save_XML_upGroup(bool _v);
    void save_XML_upName(bool _v);
    void save_XML_upDesc(bool _v);
    void save_XML_upManufacturer(bool _v);
    void save_XML_upImage(bool _v);
    void save_XML_upPrice(bool _v);
    void save_XML_upQuan(bool _v);
    void save_XML_upAttribute(bool _v);
    void save_XML_upOption(bool _v);
    void save_XML_upStockStatus(bool _v);
    void save_XML_upStatus(bool _v);

    int get_CSV_group();
    int get_CSV_attribute();

    bool get_CSV_newProduct();
    bool get_CSV_newNotPrice();
    bool get_CSV_newGroup();
    int get_CSV_newGroupId();
    int get_CSV_newQuan();
    int get_CSV_newStockStatusId();
    int get_CSV_newStatusId();

    bool get_CSV_upGroup();
    bool get_CSV_upName();
    bool get_CSV_upDesc();
    bool get_CSV_upImage();
    bool get_CSV_upManufacturer();
    bool get_CSV_upPrice();
    bool get_CSV_upQuan();
    bool get_CSV_upAttribute();
    bool get_CSV_upOption();
    bool get_CSV_upStockStatus();
    bool get_CSV_upStatus();



    void save_CSV_group(int _v);
    void save_CSV_attribute(int _v);

    void save_CSV_newProduct(bool _v);
    void save_CSV_newNotPrice(bool _v);
    void save_CSV_newGroup(bool _v);
    void save_CSV_newGroupId(int _v);
    void save_CSV_newQuan(int _v);
    void save_CSV_newStockStatusId(int _v);
    void save_CSV_newStatusId(int _v);

    void save_CSV_upGroup(bool _v);
    void save_CSV_upName(bool _v);
    void save_CSV_upDesc(bool _v);
    void save_CSV_upManufacturer(bool _v);
    void save_CSV_upImage(bool _v);
    void save_CSV_upPrice(bool _v);
    void save_CSV_upQuan(bool _v);
    void save_CSV_upAttribute(bool _v);
    void save_CSV_upOption(bool _v);
    void save_CSV_upStockStatus(bool _v);
    void save_CSV_upStatus(bool _v);

    bool get_AUTO_active();
    void save_AUTO_active(bool _v);

};

#endif // PREFERENCE_H
