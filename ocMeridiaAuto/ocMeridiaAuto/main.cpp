#include "ocmeridiaauto.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ocMeridiaAuto w;
    w.show();

    return a.exec();
}
