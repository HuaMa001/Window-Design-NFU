#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QtNetwork>
#include <QtWidgets>
class TcpFileSender : public QDialog
{
    Q_OBJECT

public:
    TcpFileSender(QWidget *parent = nullptr);
    ~TcpFileSender();
public:
    void start();
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void openFile();
private:
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLabel;
    QPushButton *startButton;
    QPushButton *openButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QTcpSocket tcpClient;
    qint64 totalByte;
    qint64 byteWritten;
    qint64 byteToWrite;
    qint64 loadSize;
    QString fileName;
    QFile *locoalFile;
    QByteArray outBlock; //資料緩衝區

};
#endif // TCPFILESENDER_H
