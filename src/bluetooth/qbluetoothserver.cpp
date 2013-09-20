/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qbluetoothserver.h"
#include "qbluetoothserver_p.h"
#include "qbluetoothsocket.h"
#include "qbluetoothserviceinfo.h"

QT_BEGIN_NAMESPACE

/*!
    \class QBluetoothServer
    \inmodule QtBluetooth
    \brief The QBluetoothServer class uses the RFCOMM or L2cap protocol to communicate with
    a Bluetooth device.

    QBluetoothServer is used to implement Bluetooth services over RFCOMM or L2cap.

    Start listening for incoming connections with listen(). Wait till the newConnection() signal
    is emitted when a new connection is established, and call nextPendingConnection() to get a QBluetoothSocket
    for the new connection.

    To enable other devices to find your service, create a QBluetoothServiceInfo with the
    applicable attributes for your service and register it using QBluetoothServiceInfo::registerService().
    Call serverPort() to get the channel number that is being used.

    If the \l QBluetoothServiceInfo::Protocol is not supported by a platform, \l listen() will return \c false.

    \sa QBluetoothServiceInfo, QBluetoothSocket
*/

/*!
    \fn void QBluetoothServer::newConnection()

    This signal is emitted when a new connection is available.

    The connected slot should call nextPendingConnection() to get a QBluetoothSocket object to
    send and receive data over the connection.

    \sa nextPendingConnection(), hasPendingConnections()
*/

/*!
    \fn void QBluetoothServer::error(QBluetoothServer::Error error)

    This signal is emitted when an \a error occurs.

    \sa error(), QBluetoothServer::Error
*/

/*!
    \fn void QBluetoothServer::close()

    Closes and resets the listening socket.
*/

/*!
    \enum QBluetoothServer::Error

    This enum describes Bluetooth server error types.

    \value NoError                  No error.
    \value UnknownError             An unknown error occurred.
    \value PoweredOffError          The Bluetooth adapter is powered off.
    \value InputOutputError         An input output error occurred.
    \value ServiceAlreadyRegisteredError  The service or port was already registered
    \value UnsupportedProtocolError The \l {QBluetoothServiceInfo::Protocol}{Protocol} is not
                                    supported on this platform.
*/

/*!
    \fn bool QBluetoothServer::listen(const QBluetoothAddress &address, quint16 port)

    Start listening for incoming connections to \a address on \a port.

    Returns true if the operation succeeded and the server is listening for
    incoming connections, otherwise returns false.

    \sa isListening(), newConnection()
*/

/*!
    \fn void QBluetoothServer::setMaxPendingConnections(int numConnections)

    Sets the maximum number of pending connections to \a numConnections.

    \sa maxPendingConnections()
*/

/*!
    \fn bool QBluetoothServer::hasPendingConnections() const
    Returns true if a connection is pending, otherwise false.
*/

/*!
    \fn QBluetoothSocket *QBluetoothServer::nextPendingConnection()

    Returns a pointer to aQBluetoothSocket for the next pending connection. It is the callers
    responsibility to delete the pointer.
*/

/*!
    \fn QBluetoothAddress QBluetoothServer::serverAddress() const

    Returns the server address.
*/

/*!
    \fn quint16 QBluetoothServer::serverPort() const

    Returns the server port number. On BlackBerry, this port might not correspond to the real RFCOMM port.
*/

/*!
    Constructs a bluetooth server with \a parent and \a serverType.
*/
QBluetoothServer::QBluetoothServer(QBluetoothServiceInfo::Protocol serverType, QObject *parent)
    : QObject(parent), d_ptr(new QBluetoothServerPrivate(serverType))
{
    d_ptr->q_ptr = this;
}

/*!
    Destroys the bluetooth server.
*/
QBluetoothServer::~QBluetoothServer()
{
    delete d_ptr;
}

/*!
    \fn QBluetoothServiceInfo QBluetoothServer::listen(const QBluetoothUuid &uuid, const QString &serviceName)

    Convenience function for registering an SPP service with \a uuid and \a serviceName.
    Because this function already registers the service, the QBluetoothServiceInfo object
    which is returned can not be changed any more.

    Returns a registered QBluetoothServiceInfo instance if sucessful otherwise an
    invalid QBluetoothServiceInfo.

    For an RFCOMM server this function is equivalent to following code snippet.

    \snippet qbluetoothserver.cpp listen
    \snippet qbluetoothserver.cpp listen2
    \snippet qbluetoothserver.cpp listen3

    \sa isListening(), newConnection(), listen()
*/
QBluetoothServiceInfo QBluetoothServer::listen(const QBluetoothUuid &uuid, const QString &serviceName)
{
    Q_D(const QBluetoothServer);
    if (!listen())
        return QBluetoothServiceInfo();
//! [listen]
    QBluetoothServiceInfo serviceInfo;
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, serviceName);
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));

    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             classId);

    serviceInfo.setServiceUuid(uuid);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
//! [listen]
    if (d->serverType == QBluetoothServiceInfo::RfcommProtocol) {
//! [listen2]
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
             << QVariant::fromValue(quint8(serverPort()));
//! [listen2]
    }
//! [listen3]
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);
    bool result = serviceInfo.registerService();
//! [listen3]
    if (!result)
        return QBluetoothServiceInfo();
    return serviceInfo;
}

/*!
    Returns true if the server is listening for incoming connections, otherwise false.
*/
bool QBluetoothServer::isListening() const
{
    Q_D(const QBluetoothServer);

#ifdef QT_QNX_BLUETOOTH
    if (!d->socket)
        return false;
#endif

    return d->socket->state() == QBluetoothSocket::ListeningState;
}

/*!
    Returns the maximum number of pending connections.

    \sa setMaxPendingConnections()
*/
int QBluetoothServer::maxPendingConnections() const
{
    Q_D(const QBluetoothServer);

    return d->maxPendingConnections;
}

/*!
    \fn QBluetoothServer::setSecurityFlags(QBluetooth::SecurityFlags security)
    Sets the Bluetooth security flags to \a security. This function must be called before calling listen().
    On BlackBerry, security flags are not supported and will be ignored.
*/

/*!
    \fn QBluetooth::SecurityFlags QBluetoothServer::securityFlags() const
    Returns the Bluetooth security flags.
*/

/*!
    \fn QBluetoothSocket::ServerType QBluetoothServer::serverType() const
    Returns the type of the QBluetoothServer.
*/
QBluetoothServiceInfo::Protocol QBluetoothServer::serverType() const
{
    Q_D(const QBluetoothServer);
    return d->serverType;
}

/*!
    \fn QBluetoothServer::Error QBluetoothServer::error() const
    Returns the last error of the QBluetoothServer.
*/
QBluetoothServer::Error QBluetoothServer::error() const
{
    Q_D(const QBluetoothServer);
    return d->m_lastError;
}

#include "moc_qbluetoothserver.cpp"

QT_END_NAMESPACE
