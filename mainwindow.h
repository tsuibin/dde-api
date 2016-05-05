#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

#include <QIODevice>
#include <QSettings>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QVariantList>
#include <QBuffer>
#include <QTimer>

#include <QMutex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkAccessManager *m_sendManager;
    QNetworkAccessManager *m_getMsgManager;
    QNetworkAccessManager *m_getAllDocsManager;
    QNetworkAccessManager *m_getLongpollManager;
    QNetworkAccessManager *m_getHearheatManager;
    QNetworkAccessManager *m_getContinuousManager;
    QNetworkAccessManager *m_getChangesManager;
    QNetworkAccessManager *nam_uuid;
    QJsonObject m_sendJsonData;
    QSettings::SettingsMap m_map;
    QString m_allDocsUrl;
    QString m_dbUrl;
    QString m_continuousUrl;
    QString m_changesUrl;
    QString m_since;

    QMutex m_mutex;
    QBuffer *m_buffer;

    //CRUD
    QNetworkReply * get(QNetworkAccessManager *networkAccessManager, QString &requestUrl);

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_send_clicked();
    void replyFinished(QNetworkReply* reply);
    void uuidFinished(QNetworkReply* reply);
    void sendFinished(QNetworkReply* reply);
    void getMsgFinished(QNetworkReply* reply);
    void getAllDocs(QNetworkReply* reply); //http://127.0.0.1:5984/chat/_all_docs
    void getChanges(QNetworkReply* reply); //http://127.0.0.1:5984/chat/_changes
    void getMsgById(QString Id);
    void readMsg(QNetworkReply *replay);
    void newMsg(QNetworkReply *reply);
    QJsonDocument getJson(QByteArray qba);
    void networkAccessibleChangedSlot(QNetworkAccessManager::NetworkAccessibility);
    void getChangesDoc(QNetworkReply*);



    void on_pushButton_gmsglist_clicked();
    void on_pushButton_continuous_clicked();

    void on_pushButton_changes_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
