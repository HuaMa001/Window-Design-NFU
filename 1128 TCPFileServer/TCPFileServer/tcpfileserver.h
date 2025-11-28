#ifndef TCPFILESERVER_H
#define TCPFILESERVER_H
#include <QtNetwork>
#include <QDialog>
#include <QtWidgets>

class TCPFileServer : public QDialog
{
    Q_OBJECT

public:
    TCPFileServer(QWidget *parent = nullptr);
    ~TCPFileServer();
public slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void dispayError(QAbstractSocket::SocketError socketError);
private:
    QProgressBar *serverProgressBar;
    QLabel *serverStatusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 totalBytes;
    qint64 byteReceived;
    qint64 fileNameSize;
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;

};
#endif // TCPFILESERVER_H
