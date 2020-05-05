#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
#include <QDebug>
Widget::Widget(char* argv[],QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget) {
    dir_path=argv[1];
    ip_set=argv[2];
    port_name=argv[3];
    ui->setupUi(this);
    this->setWindowTitle("server");
    connect(&m_tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    connect(ui->startBtn,SIGNAL(clicked(bool)),this,SLOT(start()));
    appendflag=false;
    ui->ipaddr->setText(ip_set);
    ui->portnum->setText(port_name);
}

void Widget::start(){
    if(ui->portnum->text()!=nullptr)
    {
        port_name=ui->portnum->text();
    }
    if(!(m_tcpServer.listen(QHostAddress::LocalHost,port_name.toInt()))){
        qDebug()<<m_tcpServer.errorString();
        qDebug()<<"listen出错";
        close();
        return;
   }
    qDebug()<<port_name;
    ui->startBtn->setEnabled(false);
    m_totalBytes=0;
    m_bytesReceived=0;
    m_fileNameSize=0;
    flag=1;
    headflag=false;
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
void Widget::updateServerProgress() {
    QDataStream in(m_tcpServerConnection);
    //
    //m_bytesReceived=0;
    in.setVersion(QDataStream::Qt_5_14);
    // 如果接收到的数据小于16个字节，保存到来的文件头结构,qint64占8字节
    if (m_bytesReceived<=sizeof(qint64)*2)
    {
        if((m_tcpServerConnection->bytesAvailable()>=sizeof(qint64)*2)&&(m_fileNameSize==0))
        {
            // 接收数据总大小信息和文件名大小信息
            in>>m_totalBytes>>m_fileNameSize;
            m_bytesReceived +=sizeof(qint64)*2;
        }
        if((m_tcpServerConnection->bytesAvailable()>=m_fileNameSize)&&(m_fileNameSize!=0))
        {
            // 接收文件名，并建立文件
            in>>m_fileName;
            //qDebug() <<"m_fileName"<<m_fileName<<"m_fileNameSize"<<m_fileNameSize;
            ui->serverStatusLabel->setText(tr("接收文件 %1 …").arg(m_fileName));
            m_bytesReceived+=m_fileNameSize;
            //qDebug()<<m_bytesReceived;

            //还原相对路径
            QString Current_path=dir_path+m_fileName.left(m_fileName.lastIndexOf('/')+1);//所有路径内容

            QDir kDir;
            if(!kDir.exists(Current_path)) { //判断指定目录下是否存在指定目录
                kDir.mkpath(Current_path); //生成指定目录
                qDebug() << "Create";
            } else {
                qDebug() << "Already exits";
            }

            QString Current_m_fileName=m_fileName.right(m_fileName.size()-m_fileName.lastIndexOf('/')-1);//仅文件名

            m_localFile = new QFile(Current_path+Current_m_fileName);//文件m_localFile是路径+文件名
           // QDataStream sendOut(m_tcpServerConnection);
            QDataStream sendOut(&m_sendoutBlock,QIODevice::WriteOnly);
            sendOut.setVersion(QDataStream::Qt_5_14);
            //向客户端发送已经存在的文件信息，或者文件不存在的信息
            //m_localFile->open(QFile::WriteOnly|QIODevice::Append);  //打开文件
            if(flag==1&&m_localFile->size()>0)  //保证每个文件只发送一次断点信息
            {   sendOut<<m_localFile->size();
                m_tcpServerConnection->write(m_sendoutBlock);
                qDebug()<< "已经发送了大小为"<<m_localFile->size();
                flag=0;
                //m_bytesReceived-=90;
                if(appendflag!=true)  //不是覆盖写的时候
                m_bytesReceived+=m_localFile->size();
                headflag=true;
                //qDebug()<<"多出来的文件头"<<m_tcpServerConnection->bytesAvailable();
                //m_inBlock = m_tcpServerConnection->readAll();
                //m_localFile->close();
                //return;
            }
            if(appendflag==true)   //如果是出错了之后，即客户端挂掉则只能全部重传，则覆盖写
            {
            if (!m_localFile->open(QFile::WriteOnly))
            {
                qDebug() << "server: open file error!";
                return;
            }
            }
            else    //默认为续写
            {
                if (!m_localFile->open(QFile::WriteOnly|QIODevice::Append))
                {
                    qDebug() << "server: open file error!";
                    return;
                }
            }
        }else{
            return;
        }

    }
    qDebug()<<"预计接受的大小："<<m_totalBytes;
    // 如果接收到的数据小于总数据，那么写入文件
    if(m_bytesReceived<m_totalBytes){
        m_bytesReceived+=m_tcpServerConnection->bytesAvailable();
        qDebug()<<"多出来的文件头"<<m_tcpServerConnection->bytesAvailable();
        qDebug()<<"接收到的大小"<<m_bytesReceived;
        m_inBlock = m_tcpServerConnection->readAll();
        if(m_inBlock.size()<=m_fileNameSize+sizeof(qint64)*2&&headflag==true) //用于忽略粘包的文件头
           {headflag=false;
            qDebug()<<"成功忽略文件头"<<m_inBlock.size();
            m_bytesReceived-=m_inBlock.size();
        }
        else
        {m_localFile->write(m_inBlock);}
        m_inBlock.resize(0);
    }
    ui->ServerProgressBar->setMaximum(m_totalBytes);
    ui->ServerProgressBar->setValue(m_bytesReceived);
    // 接收数据完成时
    if (m_bytesReceived==m_totalBytes){
         qDebug() <<(tr("接收文件 %1 …完成").arg(m_fileName));
        //m_tcpServerConnection->close();
        m_localFile->close();
        //ui->startBtn->setEnabled(true);
        ui->serverStatusLabel->setText(tr("接收文件 %1 成功！").arg(m_fileName));
        m_bytesReceived=0;
        m_fileNameSize=0;
        flag=1;
        //updateServerProgress();           //重新开始传输
    }
}

void Widget::displayError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError)
    qDebug()<<m_tcpServerConnection->errorString();
    m_tcpServerConnection->close();
    ui->ServerProgressBar->reset();
    m_localFile->close();
    ui->serverStatusLabel->setText("服务端就绪");
    ui->startBtn->setEnabled(true);
    appendflag=true; //出现错误，则下次写文件直接覆盖掉
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
Widget::~Widget() {
    delete ui;
}

