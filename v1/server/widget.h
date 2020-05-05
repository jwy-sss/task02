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
    Widget(QWidget *parent = nullptr);
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
        QString m_fileName;     //待接收文件的文件名
        QFile *m_localFile;      //待接收文件
        QByteArray m_inBlock;    //缓冲区
};
#endif // WIDGET_H
