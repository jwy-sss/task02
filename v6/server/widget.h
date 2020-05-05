#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractSocket>
#include <QTcpServer>
class QTcpSocket;
class QFile;
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(char* argv[],QWidget *parent = nullptr);
    ~Widget();

public slots:
    void start();
    void acceptConnection();
    void updateServerProgress();
    void displayError(QAbstractSocket::SocketError socketError);
    //void on_startBtn_clicked();
private:
    Ui::Widget *ui;
        QTcpServer m_tcpServer;    //服务器套接字
        QTcpSocket *m_tcpServerConnection;   //连接后服务器返回的套接字
        qint64 m_totalBytes;
        qint64 m_bytesReceived;
        qint64 m_fileNameSize;   //待接收文件名字节数
        qint64 flag;     //用于断点重传时保证只向客户端发送一次断点信息
        bool headflag;   //断点重传时会多读取一个文件头，用这个来忽略它
        bool appendflag; //用于判断是否要在文件后面写或者直接覆盖文件
        QByteArray m_sendoutBlock;
        QString m_fileName;     //待接收文件的文件名
        QFile *m_localFile;      //待接收文件
        QByteArray m_inBlock;    //缓冲区
        QString dir_path;   //文件夹路径
        QString ip_set; //  接收端ip
        QString port_name;   //端口号

};
#endif // WIDGET_H
