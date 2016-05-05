#include "msglist.h"
#include "ui_msglist.h"

MsgList::MsgList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MsgList)
{
    ui->setupUi(this);
}

MsgList::~MsgList()
{
    delete ui;
}

void MsgList::changeEvent(QEvent *e)
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

void MsgList::on_pushButton_clicked()
{

}
