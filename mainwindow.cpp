#include "mainwindow.h"
#include "ui_mainwindow.h"


//http://127.0.0.1:5984/chat/_all_docs
//http://127.0.0.1:5984/chat/_changes
//http://127.0.0.1:5984/chat/_changes?since=<seqid>
//http://127.0.0.1:5984/chat/_changes?feed=longpoll&since=<seqid> // new msg after seq
//http://127.0.0.1:5984/chat/_changes?feed=longpoll&hearheat=1000&since=<seqid> // new msg after seq, every 1000ms send a newline char
//http://127.0.0.1:5984/chat/_changes?feed=continuous&since=<seqid> // new msg after seq
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_dbUrl = "http://127.0.0.1:5984/chat/";
    m_allDocsUrl = m_dbUrl + "_all_docs";
    m_continuousUrl = m_dbUrl + "_changes?feed=continuous&since=29";


    m_networkAccessManager = new QNetworkAccessManager(this);
    nam_uuid = new QNetworkAccessManager(this);
    m_sendManager = new QNetworkAccessManager(this);
    m_getMsgManager = new QNetworkAccessManager(this);
    m_getAllDocsManager = new QNetworkAccessManager(this);
    m_getContinuousManager = new QNetworkAccessManager(this);

    connect(nam_uuid, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(uuidFinished(QNetworkReply*)));
    connect(m_sendManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(sendFinished(QNetworkReply*)));
    connect(m_getMsgManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(getMsgFinished(QNetworkReply*)));
    connect(m_getAllDocsManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(getAllDocs(QNetworkReply*)));

    //m_getcontinuousManager
    connect(m_getContinuousManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(newMsg(QNetworkReply*)));


    QUrl url(tr("http://127.0.0.1:5984/chat"));
    m_networkAccessManager->put(QNetworkRequest(url),"");

    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_send_clicked()
{




    QUrl url(tr("http://127.0.0.1:5984/_uuids"));

    nam_uuid->get(QNetworkRequest(url));


}

void MainWindow::uuidFinished(QNetworkReply* reply)
{

    QJsonDocument jd = this->getJson(reply->readAll());
    QString uuid = jd.object().value("uuids").toArray().first().toString();

    QString rurl = "http://127.0.0.1:5984/chat/" + uuid;


     QUrl url(rurl);


     m_sendJsonData.insert("msg",ui->textEdit_input->toPlainText());
     m_sendJsonData.insert("uname",ui->lineEdit_uname->text());
     m_sendManager->put(QNetworkRequest(url), QJsonDocument(m_sendJsonData).toJson());
     qDebug() << url.toString();


}
//sendFinished
void MainWindow::sendFinished(QNetworkReply* reply)
{



//    ui->textEdit_history->append( qba );

    QJsonDocument jd = this->getJson(reply->readAll());

    qDebug() << jd.object().value("id").toString();
    qDebug() << jd.object().value("rev").toString();
    qDebug() << jd.object().value("ok").toBool();
    // QString uuid = jd.object().value("id").toArray().first().toString();

     if(jd.object().value("ok").toBool())
     {
         //get msg
         QString uuid = jd.object().value("id").toString();
         QString requestUrl = "http://127.0.0.1:5984/chat/" + uuid;

         m_getMsgManager->get(QNetworkRequest(requestUrl));
         qDebug() << requestUrl;
     }
}

//getMsgFinished

QNetworkReply * MainWindow::get(QNetworkAccessManager *networkAccessManager, QString requestUrl)
{
    return networkAccessManager->get(QNetworkRequest(requestUrl));
}

void MainWindow::replyFinished(QNetworkReply* reply)
{
    ui->textEdit_history->append(reply->readAll());
}

void MainWindow::getMsgFinished(QNetworkReply* reply)
{

    QJsonDocument jd = this->getJson(reply->readAll());

    qDebug() << jd.object().value("_id").toString();
    qDebug() << jd.object().value("_rev").toString();
    qDebug() << jd.object().value("msg").toString();
    QString uname = jd.object().value("uname").toString();
    QString msg = jd.object().value("msg").toString();

    ui->textEdit_history->append( uname +":"+ msg );



}

void MainWindow::getAllDocs(QNetworkReply *reply)
{

    ui->textEdit_history->clear();
    QJsonDocument jd = this->getJson(reply->readAll());

    qDebug() << jd.object().value("total_rows").toDouble();
    qDebug() << jd.object().value("offset").toDouble();
    QJsonValue jv = jd.object().value("rows");

    foreach(QJsonValue jvv, jv.toArray())
    {

        getMsgById(jvv.toObject().value("id").toString());
    }
}

void MainWindow::getMsgById(QString Id)
{


    QNetworkAccessManager *tmp = new QNetworkAccessManager(this);
    connect(tmp, SIGNAL(finished(QNetworkReply*)), this, SLOT(readMsg(QNetworkReply*)));
    connect(tmp, SIGNAL(finished(QNetworkReply*)), tmp, SLOT(deleteLater()));
    QString url = m_dbUrl + Id;
    this->get(tmp, url);
}

void MainWindow::readMsg(QNetworkReply *reply)
{


    QJsonDocument jd = getJson(reply->readAll());
    QString msg = jd.object().value("msg").toString();
    QString uname = jd.object().value("uname").toString();
    ui->textEdit_history->append(uname + ":" + msg);


}

void MainWindow::newMsg(QNetworkReply *reply)
{
    QJsonDocument jd = getJson(reply->readAll());
    qDebug() << jd;
}

QJsonDocument MainWindow::getJson(QByteArray qba)
{

    QJsonParseError jpe;
    QJsonDocument jd = QJsonDocument::fromJson(qba,&jpe);
    if(jpe.error != QJsonParseError::NoError )
    {
        qDebug() << jpe.errorString();
        return QJsonDocument();
    }
    return jd;
}


void MainWindow::getChanges(QNetworkReply *reply)
{

}

void MainWindow::on_pushButton_gmsglist_clicked()
{
    this->get(m_getAllDocsManager, m_allDocsUrl);

}

void MainWindow::on_pushButton_2_clicked()
{
    this->get(m_getContinuousManager, m_continuousUrl);
}
