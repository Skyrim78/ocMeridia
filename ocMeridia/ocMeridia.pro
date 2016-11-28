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
    setting.cpp

HEADERS  += ocmeridia.h \
    product_form.h \
    setting.h

FORMS    += ocmeridia.ui \
    product_form.ui \
    setting.ui

RESOURCES +=
