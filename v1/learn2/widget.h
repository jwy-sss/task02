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
    Widget(QString filename,){ m_fileName=filename;


                            }
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_openBtn_clicked();
    void openFile();
        void send();
        void startTransfer();
        void updateClientProgress(qint64);
        void displayError(QAbstractSocket::SocketError);
        void on_sendBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *m_tcpClient;
        QFile *m_localFile;
        qint64 m_totalBytes;     //文件总大小
        qint64 m_bytesWritten;   //已经传了的文件大小
        qint64 m_bytesToWrite;   //此次要传输文件大小
        qint64 m_payloadSize;
        QStringList a;
        int filenumber;
        QString m_fileName;   //文件路径
        QByteArray m_outBlock;  //分块大小
};
#endif // WIDGET_H
