#-------------------------------------------------
#
# Project created by QtCreator 2016-11-12T01:09:57
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ocMeridia
TEMPLATE = app


SOURCES += main.cpp\
        ocmeridia.cpp \
    product_form.cpp \
    setting.cpp \
    local.cpp \
    ctrlsproduct.cpp \
    ctrlscategory.cpp \
    ctrlfile.cpp \
    ctrlpreference.cpp \
    pref.cpp \
    ctrlmanufacturer.cpp \
    ctrldb.cpp \
    ctrloptions.cpp \
    ctrlattribute.cpp

HEADERS  += ocmeridia.h \
    product_form.h \
    setting.h \
    local.h \
    ctrlsproduct.h \
    ctrlscategory.h \
    ctrlfile.h \
    ctrlpreference.h \
    pref.h \
    ctrlmanufacturer.h \
    ctrldb.h \
    ctrloptions.h \
    ctrlattribute.h

FORMS    += ocmeridia.ui \
    product_form.ui \
    setting.ui \
    local.ui \
    pref.ui

RESOURCES +=
