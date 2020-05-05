#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
#include <QFileDialog>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->setupUi(this);
        this->setWindowTitle("client");
        m_payloadSize=64*1024;
        m_totalBytes=0;
        m_bytesWritten=0;
        m_bytesToWrite=0;
        filenumber=0;    //发送的文件编号
        m_tcpClient=new QTcpSocket(this);
        connect(m_tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
        connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
        connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        connect(ui->sendBtn,SIGNAL(clicked(bool)),this,SLOT(on_sendBtn_clicked()));
        connect(ui->openBtn,SIGNAL(clicked(bool)),this,SLOT(on_openBtn_clicked()));
}

void Widget::openFile()
{
    if(a[filenumber]!=NULL)
    {
        m_fileName=a[filenumber];
        filenumber++;
    }
    else{        m_localFile->close();
        m_tcpClient->close();}
    if(!m_fileName.isEmpty())
    {
        ui->sendBtn->setEnabled(true);
        ui->clientStatusLabel->setText(QString("打开文件 %1 成功!").arg(m_fileName));
    }
}
void Widget::send()
{
    ui->sendBtn->setEnabled(false);
    m_bytesWritten=0;
    ui->clientStatusLabel->setText("连接中");
    m_tcpClient->connectToHost(ui->hostLineEdit->text(),ui->portLineEdit->text().toInt());
}
void Widget::startTransfer()
{
    m_localFile=new QFile(m_fileName);
    if(!m_localFile->open(QFile::ReadOnly)){
        qDebug()<<"client：open file error!";
        return;
    }
    m_totalBytes=m_localFile->size();       //获取文件大小
    QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_14);
    QString currentFileName=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);  //获取文件名
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    m_totalBytes+=m_outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<m_totalBytes<<qint64(m_outBlock.size()-sizeof(qint64)*2);
    m_bytesToWrite=m_totalBytes-m_tcpClient->write(m_outBlock);
    ui->clientStatusLabel->setText("已连接");
    m_outBlock.resize(0);
}

void Widget::updateClientProgress(qint64 numBytes)
{
    m_bytesWritten+=(int)numBytes;
    if(m_bytesToWrite>0)           //如果还有东西要写
    {
        m_outBlock=m_localFile->read(qMin(m_bytesToWrite,m_payloadSize));
        m_bytesToWrite-=(int)m_tcpClient->write(m_outBlock);
        m_outBlock.resize(0);
    }
    else           //传输完毕
    {
        m_localFile->close();
    }
    ui->clientProgressBar->setMaximum(m_totalBytes);     //设置进度条
    ui->clientProgressBar->setValue(m_bytesWritten);

    if(m_bytesWritten==m_totalBytes)
    {
        ui->clientStatusLabel->setText(QString("传输文件 %1 成功!").arg(m_fileName));
        m_localFile->close();
        m_tcpClient->close();
        //openFile();
        //startTransfer();
    }
}
void Widget::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<m_tcpClient->errorString();
    m_tcpClient->close();
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText("客户端就绪");
    ui->sendBtn->setEnabled(true);
}
void Widget::on_openBtn_clicked()
{
    a=QFileDialog::getOpenFileNames(this);
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText("状态：等待打开文件！");
    openFile();
}

void Widget::on_sendBtn_clicked()
{
    send();
}

Widget::~Widget()
{
    delete ui;
}
