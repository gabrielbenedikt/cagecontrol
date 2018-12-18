#ifndef UDPLISTENER_H
#define UDPLISTENER_H

/*! \file */

#include <QObject>
#include <QHostAddress>
#include <QSettings>
#include <QUdpSocket>
#include "helper.h"

/*!
 * \brief The UDPlistener class is used to control dinspect with UDP packages
 * \bug There are no known bugs
 *
 */
class UDPlistener : public QObject {
    Q_OBJECT
public:
    /*!
     * \brief UDPlistener listen for commands on a UDP port and execute them
     * \param settings a pointer to a qsettings instance, used to get the port to bind to and known commands
     *
     * UDPlistener opens a UDP socket and binds to a port specified in qsettings.
     * Incoming packages are analysed to check if they contain known commands. If they do,
     * these commands are executed.
     *
     * The commands can be changed in the dinspect settings dialog, but the standard ones are:
     *      - Move(QString, QString)
     *      - Move(QString, double, double)
     */
    explicit UDPlistener(QSettings *settings, QObject *parent = 0);

private:
    QSettings *settings;    //!<configuration
    QUdpSocket socket;      //!<the UDP socket
    //QHostAddress IP;
    uint port;              //!<port the listener listens to
    bool alreadybound;      //!<true if the listener is already listening to a port

signals:
    /*!
     * \brief Move moves the waveplates in a cage to certain angles
     * \param controller either colorcode of cage or 'all'
     * \param HWPang angle of the HWP in degree
     * \param QWPang angle of the QWP in degree
     */
    void Move(QString controller, double HWPang, double QWPang);

    /*!
     * \brief MoveHV moves cage to H/V basis
     * \param controller either colorcode of stage, or 'all'
     */
    void MoveHV(QString controller);

    /*!
     * \brief MovePM moves cage to P/M basis
     * \param controller either colorcode of stage, or 'all'
     */
    void MovePM(QString controller);

    /*!
     * \brief MoveLR moves cage to R/L basis
     * \param controller either colorcode of stage, or 'all'
     */
    void MoveLR(QString controller);


private slots:
    /*!
     * \brief processPendingDatagrams reads data from the UDP socket
     */
    void processPendingDatagrams();
    /*!
     * \brief processCommands extracts commands out of received data and executes them
     * \param msg input: the received message
     */
    void processCommands(QString msg);

public slots:
    /*!
     * \brief bind Binds to a new UDP port
     *
     * This function checks whether the UDP socket is already bound to a a specific port.
     * If so, it closes this connection and binds to the new port.
     * If not, it binds to the port right away.
     */
    void bind();
};

#endif // UDPLISTENER_HH
