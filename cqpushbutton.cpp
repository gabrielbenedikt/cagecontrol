#include "cqpushbutton.h"
#include "QDebug"
#include "QMouseEvent"

/************************************************************************************************
*                             CQPushButton::CQPushButton                                        *
************************************************************************************************/
CQPushButton::CQPushButton(QString ButtonText, QString id_in, QWidget *parent) : QPushButton (parent)
{
    id = id_in;
    this->setText(ButtonText);
    connect(this,SIGNAL(clicked()),this,SLOT(triggerOutput()));
}

/************************************************************************************************
*                                CQPushButton::setid                                            *
************************************************************************************************/
void CQPushButton::setid(QString id_in)
{
    id=id_in;
}

/************************************************************************************************
*                                 CQPushButton::getid                                           *
************************************************************************************************/
QString CQPushButton::getid()
{
    return id;
}

/************************************************************************************************
*                             CQPushButton::triggerOutput                                       *
************************************************************************************************/
void CQPushButton::triggerOutput()
{
    emit pressed_id(id);
}

/************************************************************************************************
*                            CQPushButton::mousePressEvent                                      *
************************************************************************************************/
void CQPushButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton) {
        emit this->clicked();
    } else {
        emit this->clicked();
    }
}
