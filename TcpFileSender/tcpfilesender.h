#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>

class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = nullptr);
    ~TcpFileSender();
};
#endif // TCPFILESENDER_H
