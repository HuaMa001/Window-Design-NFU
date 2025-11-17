#include "tcpfilesender.h"

TcpFileSender::TcpFileSender(QWidget *parent)
    : QDialog(parent)
{
    loadSize=1024*4;
    totalByte=0;
    byteWritten=0;
    byteToWrite=0;
    clientProgressBar=new QProgressBar;
    clientStatusLabel=new QLabel("客戶端就緒");
    startButton=new QPushButton("開始");
    openButton=new QPushButton("開檔");
    quitButton=new QPushButton("退出");
    startButton->setEnabled(false);
    buttonBox= new QDialogButtonBox;
    buttonBox->addButton(startButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(openButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::RejectRole);

}

TcpFileSender::~TcpFileSender() {}
void TcpFileSender::start()
{

}
void TcpFileSender::startTransfer()
{

}
void TcpFileSender::updateClientProgress(qint64 numBytes)
{

}
void TcpFileSender::openFile()
{

}
