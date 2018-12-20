#include "helper.h"

/************************************************************************************************
*                                  helper::message                                              *
************************************************************************************************/
void helper::message(QString text)
{
    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}

/************************************************************************************************
*                                   helper::error                                               *
************************************************************************************************/
void helper::error(QString text)
{
    DEBUG_ERROR("%s\n", text.toLatin1().data());

    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}

/************************************************************************************************
*                                  helper::warning                                              *
************************************************************************************************/
void helper::warning(QString text)
{
    DEBUG_WARNING("%s\n", text.toLatin1().data());

    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}

/************************************************************************************************
*                                    helper::info                                               *
************************************************************************************************/
void helper::info(QString text)
{
    DEBUG_INFO("%s\n", text.toLatin1().data());

    QMessageBox msg;
    msg.setText(text);
    msg.exec();
}
