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

    /*
    //Start mesaurement?
    if (settings->contains("NET/ReceiveUDPctrlmsgStart")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgStart").toString();
        result = QString::compare(refmsg, msg); //result==0 iff refmsg==msg
        if (result==0) {
            emit(startMeasurement());
        }
    }
    //Stop mesaurement?
    if (settings->contains("NET/ReceiveUDPctrlmsgStop")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgStop").toString();
        result = QString::compare(refmsg, msg); //result==0 iff refmsg==msg
        if (result==0) {
            emit(stopMeasurement());
        }
    }

    //Set detector to TTM?
    if (settings->contains("NET/ReceiveUDPctrlmsgSetDetTTM")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgSetDetTTM").toString();
        result = QString::compare(refmsg, msg); //result==0 iff refmsg==msg
        if (result==0) {

            emit(setDetector("TTM8000"));
        }
    }
    //Set detector to PM100D?
    if (settings->contains("NET/ReceiveUDPctrlmsgSetDetPM100D")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgSetDetPM100D").toString();
        result = QString::compare(refmsg, msg); //result==0 iff refmsg==msg
        if (result==0) {
            emit(setDetector("PM100D"));
        }
    }

    //Change scanrange?
    if (settings->contains("NET/ReceiveUDPctrlmsgSetScanrange")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgSetScanrange").toString();

        //Check if msg contains refmsg
        //msg should be something like setscanrange(1234, 2345)
        //refmsg in this case is setscanrange
        if (msg.startsWith(refmsg+'(') && msg.endsWith(')')) {
            msg.chop(1);//remove the ')'
            msg.remove(0,refmsg.length()+1);//remove the command body + '('
            QStringList range = msg.split(',', QString::SkipEmptyParts); //split in two
            if (range.length()==2) {
                double low=range[0].toDouble();
                double high=range[1].toDouble();
                emit(setScanrange(low, high));
            } else {
                DEBUG_WARNING("Changing the scanrange needs exactly two parameters.\n");
            }

        }
    }

    //Change TTM channel?
    if (settings->contains("NET/ReceiveUDPctrlmsgSetTTMChannel")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgSetTTMChannel").toString();
        if (msg.startsWith(refmsg+'(') && msg.endsWith(')')) {
            msg.chop(1);//remove the ')'
            msg.remove(0,refmsg.length()+1);//remove the command body + '('
            int channel = msg.toUInt();
            emit(setTTMChannel(channel));
        }
    }
    //Change TTM coincidence channel?
    if (settings->contains("NET/ReceiveUDPctrlmsgSetTTMCoincidenceChannel")) {
        refmsg = settings->value("NET/ReceiveUDPctrlmsgSetTTMCoincidenceChannel").toString();
        if (msg.startsWith(refmsg+'(') && msg.endsWith(')')) {
            msg.chop(1);//remove the ')'
            msg.remove(0,refmsg.length()+1);//remove the command body + '('
            int channel = msg.toUInt();
            emit(setTTMCoincidenceChannel(channel));
        }
    }
*/
    // Move motors?
    refmsg="Move";
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
            basis=params[1];
            if (basis=="HV") {
                emit(MoveHV(motorcolor));
            } else if (basis=="PM") {
                emit(MovePM(motorcolor));
            } else if (basis=="LR") {
                emit(MoveLR(motorcolor));
            } else {
                DEBUG_ERROR("Expected basis strings to be 'HV', 'PM', 'LR'. Got: %s\n", basis.toLocal8Bit().data());
            }
        } else {
            DEBUG_ERROR("Move motors: expected 2 or 3 parameters, got %d. msg: %s\n", params.length(), msg.toLocal8Bit().data());
        }
    }
}

