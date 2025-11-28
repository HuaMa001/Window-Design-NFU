#ifndef TCPFILESERVER_H
#define TCPFILESERVER_H

#include <QDialog>

class TCPFileServer : public QDialog
{
    Q_OBJECT

public:
    TCPFileServer(QWidget *parent = nullptr);
    ~TCPFileServer();
};
#endif // TCPFILESERVER_H
