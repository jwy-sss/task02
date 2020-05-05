#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractSocket>
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

private slots:
    void on_openBtn_clicked();
    void openFile();
        void send();
        void startTransfer();
        void updateClientProgress(qint64);
        void displayError(QAbstractSocket::SocketError);
        void on_sendBtn_clicked();
        void addSubFolder(QString path);
        void continuetrans();
        void startretrans();
        void resend();
        void wrap();
        void on_pushButton_zip_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *m_tcpClient;
        QFile *m_localFile;
        qint64 m_totalBytes;     //文件总大小
        qint64 m_bytesWritten;   //已经传了的文件大小
        qint64 m_bytesToWrite;   //此次要传输文件大小
        qint64 m_payloadSize;
        qint64 written;
        qint64 sendtimes;
        bool retransflag;
        QString m_fileName;   //文件路径
        QByteArray m_outBlock;  //分块大小
        QString dir_path;   //文件夹路径
        QString ip_set; //  接收端ip
        QString port_name;   //端口号
        QStringList file_names;//相对路径+文件名
        qint64 file_counts;//文件数
        int current_file_count;
};
#endif // WIDGET_H
