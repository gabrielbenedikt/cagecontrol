#ifndef CQPUSHBUTTON_H
#define CQPUSHBUTTON_H

#include <QPushButton>

class CQPushButton : public QPushButton
{
    Q_OBJECT

public:

    CQPushButton(QString ButtonText, QString id="",QWidget *parent = nullptr);

protected:
    QString id;                 //!< QString to send when pressed_id is emitted. \see setid \see getid \see pressed_id

public slots:
    void setid(QString id);     //!< Sets QString id which is sent when signal pressed_id is emitted. \see getid \see pressed_id \see id
    QString getid();            //!< Returns QString id which is sent when signal pressed_id is emitted. \see setid \see pressed_id \see id
    void triggerOutput();       //!< Function to capture Qt's standard &QAbstracButton::pressed(void) signal and emits pressed_id(QString) instead.

private slots:
    void mousePressEvent(QMouseEvent *e);
signals:
    /*!
     * \brief pressed_id signal emitted when button is pressed. Carries QString id. \see setid \see getid \see id
     */
    void pressed_id(QString);
    void rightClicked();
};

#endif // CQPUSHBUTTON_H
