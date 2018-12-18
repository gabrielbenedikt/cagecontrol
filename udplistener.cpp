#include "udplistener.h"

/************************************************************************************************
*                                                                                               *
*                                UDPlistener::UDPlistener                                       *
*                                                                                               *
************************************************************************************************/
UDPlistener::UDPlistener(QSettings *insettings, QObject *parent) : QObject(parent)
{
    settings = insettings;

    bind();
    alreadybound=true;
}

/************************************************************************************************
*                                                                                               *
*                                   UDPlistener::bind                                           *
*                                                                                               *
************************************************************************************************/
void UDPlistener::bind()
{
    if (!alreadybound) {//first initialization
        if (settings->contains("NETWORK/UDDPPORT")) {
            port = settings->value("NETWORK/UDDPPORT").toUInt();
            socket.bind(port, QAbstractSocket::DefaultForPlatform);
            connect(&socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        } else {
            helper::warning("UDP control port not valid.\n");
            return;
        }
    } else {
        if (settings->contains("NETWORK/UDDPPORT")) {
            disconnect(&socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
            socket.close();
            port = settings->value("NETWORK/UDDPPORT").toUInt();
            socket.bind(port, QAbstractSocket::DefaultForPlatform);
            connect(&socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
        } else {
            helper::warning("UDP control port not valid.\n");
            return;
        }
    }
}

/************************************************************************************************
*                                                                                               *
*                          UDPlistener::processPendingDatagrams                                 *
*                                                                                               *
************************************************************************************************/
void UDPlistener::processPendingDatagrams()
{
    QByteArray datagram;
    QDataStream in(&datagram, QIODevice::ReadOnly);
    QString message;

    do {
        datagram.resize(socket.pendingDatagramSize());
        socket.readDatagram(datagram.data(), datagram.size());
    } while (socket.hasPendingDatagrams());

    in >> message;

    DEBUG_INFO("Received control message: %s\n", message.toLatin1().data());

    processCommands(message);
}

/************************************************************************************************
*                                                                                               *
*                              UDPlistener::processCommands                                     *
*                                                                                               *
************************************************************************************************/
void UDPlistener::processCommands(QString msg)
{
    int result = 0;
    QString refmsg = "";
    QStringList params; params << "" << "";
    double HWPang=0;
    double QWPang=0;
    QString motorcolor="";
    QString basis="";
    bool ok;

    msg=msg.toLower();

    // Move motors?
    refmsg="move";
    if (msg.startsWith(refmsg+'(') && msg.endsWith(')')) {
        msg.chop(1);//remove ')'
        msg.remove(0,refmsg.length()+1);//remove command body + '('
        params = msg.split(',');
        // we expect 3 parameters
        if (params.length()==3) {
            motorcolor=params[0];
            HWPang=params[1].toDouble(&ok);
            if (ok) {
                QWPang=params[2].toDouble(&ok);
                if (ok) {
                    emit(Move(motorcolor,HWPang,QWPang));
                } else {
                    DEBUG_ERROR("Conversion to double failed: %s\n", params[2].toLocal8Bit().data());
                }
            } else {
                DEBUG_ERROR("Conversion to double failed: %s\n", params[1].toLocal8Bit().data());
            }
        } else if (params.length()==2) {
            motorcolor=params[0].toLower();
            basis=params[1].toLower();
            if (basis=="hv") {
                emit(MoveHV(motorcolor));
            } else if (basis=="pm") {
                emit(MovePM(motorcolor));
            } else if (basis=="lr") {
                emit(MoveLR(motorcolor));
            } else {
                DEBUG_ERROR("Expected basis strings to be 'HV', 'PM', 'LR'. Got: %s\n", basis.toLocal8Bit().data());
            }
        } else {
            DEBUG_ERROR("Move motors: expected 2 or 3 parameters, got %d. msg: %s\n", params.length(), msg.toLocal8Bit().data());
        }
    }
}

