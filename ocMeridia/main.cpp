#include "ocmeridia.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ocMeridia w;
    w.show();

    return a.exec();
}
