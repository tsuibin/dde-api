#ifndef MSGLIST_H
#define MSGLIST_H

#include <QMainWindow>

namespace Ui {
class MsgList;
}

class MsgList : public QMainWindow
{
    Q_OBJECT

public:
    explicit MsgList(QWidget *parent = 0);
    ~MsgList();

protected:
    void changeEvent(QEvent *e);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MsgList *ui;
};

#endif // MSGLIST_H
