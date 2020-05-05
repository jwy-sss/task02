#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
        ui->setupUi(this);
        this->setWindowTitle("server");
        connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
        connect(ui->startBtn,SIGNAL(clicked(bool)),this,SLOT(start()));
}

void Widget::start(){
    if(!(m_tcpServer.listen(QHostAddress::LocalHost,10086))){
        qDebug()<<m_tcpServer.errorString();
        qDebug()<<"listen出错";
        close();
        return;
   }
    ui->startBtn->setEnabled(false);
    m_totalBytes=0;
    m_bytesReceived=0;
    m_fileNameSize=0;
    ui->serverStatusLabel->setText("监听");
    ui->ServerProgressBar->reset();
}

void Widget::acceptConnection(){
    m_tcpServerConnection=m_tcpServer.nextPendingConnection();
    connect(m_tcpServerConnection,SIGNAL(readyRead()),this,SLOT(updateServerProgress()));
    connect(m_tcpServerConnection,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    ui->serverStatusLabel->setText("接受连接");
    //关闭服务器不再监听，直接进入数据收发
    m_tcpServer.close();
}

//更新进度条同时写入文件
void Widget::updateServerProgress(){
    QDataStream in(m_tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_14);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构,qint64占8字节
    if (m_bytesReceived<=sizeof(qint64)*2){
        if((m_tcpServerConnection->bytesAvailable()>=sizeof(qint64)*2)&&(m_fileNameSize==0)){
            // 接收数据总大小信息和文件名大小信息
            in>>m_totalBytes>>m_fileNameSize;
            m_bytesReceived +=sizeof(qint64)*2;
        }
        if((m_tcpServerConnection->bytesAvailable()>=m_fileNameSize)&&(m_fileNameSize!=0)){
            // 接收文件名，并建立文件
            in>>m_fileName;
            ui->serverStatusLabel->setText(tr("接收文件 %1 …").arg(m_fileName));
            m_bytesReceived+=m_fileNameSize;
            m_localFile = new QFile("z"+m_fileName);
            if (!m_localFile->open(QFile::WriteOnly)){
                qDebug() << "server: open file error!";
                return;
            }
        }
        else{
            return;
        }
    }
    // 如果接收的数据小于总数据，那么写入文件
        if(m_bytesReceived<m_totalBytes) {
            m_bytesReceived+=m_tcpServerConnection->bytesAvailable();
            m_inBlock = m_tcpServerConnection->readAll();
            m_localFile->write(m_inBlock);
            m_inBlock.resize(0);
        }
        ui->ServerProgressBar->setMaximum(m_totalBytes);
        ui->ServerProgressBar->setValue(m_bytesReceived);

        // 接收数据完成时
        if (m_bytesReceived==m_totalBytes){
            m_tcpServerConnection->close();
            m_localFile->close();
            //ui->startBtn->setEnabled(true);
            ui->serverStatusLabel->setText(tr("接收文件 %1 成功！").arg(m_fileName));
            start();           //重新开始监听
        }
    }
void Widget::displayError(QAbstractSocket::SocketError socketError){
    Q_UNUSED(socketError)
    qDebug()<<m_tcpServerConnection->errorString();
    m_tcpServerConnection->close();
    ui->ServerProgressBar->reset();
    ui->serverStatusLabel->setText("服务端就绪");
    ui->startBtn->setEnabled(true);
}
/*void Widget::on_startBtn_clicked()
{
    if(!(m_tcpServer.listen(QHostAddress::LocalHost,10086))){
        qDebug()<<m_tcpServer.errorString();
        qDebug()<<"listen出错";
        close();
        return;
   }
    m_tcpServer.listen(QHostAddress::Any,10086);
    start();
}*/
Widget::~Widget()
{
    delete ui;
}
