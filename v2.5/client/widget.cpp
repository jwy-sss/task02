#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
#include <QFileDialog>
Widget::Widget(char* argv[],QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    qDebug()<<"arguments:"<<argv[1]<<argv[2]<<argv[3]<<argv[4];
    dir_path=argv[2];
    port_name=argv[4];
    ui->setupUi(this);
    ui->setupUi(this);
        this->setWindowTitle("client");
        m_payloadSize=64*1024;
        m_totalBytes=0;
        m_bytesWritten=0;
        m_bytesToWrite=0;
        m_tcpClient=new QTcpSocket(this);
        connect(m_tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
        connect(m_tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
        connect(m_tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        connect(ui->sendBtn,SIGNAL(clicked(bool)),this,SLOT(on_sendBtn_clicked()));
        connect(ui->openBtn,SIGNAL(clicked(bool)),this,SLOT(on_openBtn_clicked()));

}

void Widget::openFile()
{
    qDebug() <<dir_path;
    dir_path=QFileDialog::getExistingDirectory(this);
    addSubFolder(dir_path);
    /*/m_fileName=QFileDialog::getOpenFileName(this, tr("Open File"), dir_path);  //弹出对话框选择文件
    if(!m_fileName.isEmpty())
    {
        ui->sendBtn->setEnabled(true);
        ui->clientStatusLabel->setText(QString("打开文件 %1 成功!").arg(m_fileName));
    }*/
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
        m_totalBytes = 0;    //总共需发送的字节数
        m_bytesWritten = 0;       //已发送字节数
        m_bytesToWrite = 0;       //待发送字节数
        //emit outputlog(tr("start sending file to client: %1\n filename: %2")
                          // .arg(this->ui->hostLineEdit->text())
                          // .arg(m_fileName));
    m_localFile=new QFile(m_fileName);
    if(!m_localFile->open(QFile::ReadOnly)){
        qDebug()<<"client：open file error!";
        return;
    }
    //写入发送类型，数据大小，文件类型，文件名大小，文件名
    qDebug()<<"目前正在传输文件"<<current_file_count+1<<m_localFile->size();
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

//发送文件内容
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

    if(current_file_count<file_counts-1)
    {
        if(current_file_count==0)    //如果是第一个文件
        {
            ui->clientProgressBar->setMaximum(m_totalBytes);     //设置进度条
            ui->clientProgressBar->setValue(m_bytesWritten);
            ui->clientProgressBar->setValue(0);
        }
        current_file_count++;
        m_fileName=file_names[current_file_count];
        QTime _Timer = QTime::currentTime().addMSecs(50);
            while( QTime::currentTime() < _Timer )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        startTransfer();
    }
    else
    {
        qDebug()<<"send complete";
        m_totalBytes = 0;    //总共需发送的字节数
        m_bytesWritten = 0;       //已发送字节数
        m_bytesToWrite = 0;       //待发送字节数
    }
    }
//    if(m_bytesWritten==m_totalBytes)
//    {
//        ui->clientStatusLabel->setText(QString("传输文件 %1 成功!").arg(m_fileName));
//        m_localFile->close();
//        m_tcpClient->close();
//        current_file_count++;
//        if(current_file_count<file_counts)
//        {
//            send();
//        }
//    }
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
    //qDebug()<<dir_path;
    QDir kDir;
        if(!kDir.exists(dir_path)) //判断指定目录下是否存在指定目录
        {
            kDir.mkpath(dir_path); //生成指定目录
            qDebug() << "Create";
        }
        else
        {
            qDebug() << "Already exits";
        }
    ui->clientProgressBar->reset();
    ui->clientStatusLabel->setText("状态：等待打开文件！");
    openFile();
}

void Widget::on_sendBtn_clicked()
{
    /*for(int i=0;i<file_counts;i++)
    {m_fileName=file_names[i];
        if(!m_fileName.isEmpty())
        {
            ui->sendBtn->setEnabled(true);
            ui->clientStatusLabel->setText(QString("打开文件 %1 成功!").arg(m_fileName));
        }*/
        current_file_count=0;
        m_fileName=file_names[current_file_count];
        send();
}
void Widget::addSubFolder(QString path)
{
    //获取所选文件类型过滤器
    QStringList filters;
    //filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp");

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path,
        filters,
        QDir::Files | QDir::NoSymLinks,
        QDirIterator::Subdirectories);
    QStringList string_list;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
    }
    qDebug() << string_list;
    file_counts=string_list.count();
    file_names=string_list;
}

Widget::~Widget()
{
    delete ui;
}
