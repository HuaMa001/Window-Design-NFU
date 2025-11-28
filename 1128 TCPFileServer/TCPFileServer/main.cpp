#include "tcpfileserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TCPFileServer w;
    w.show();
    return a.exec();
}
