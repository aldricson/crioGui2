#include "QtTcpClient.h"
#include <QDebug>


/**
 * @brief Constructor for the QtTcpClient class.
 *
 * @param parent The parent QObject (default is nullptr).
 *
 * @details
 * - Initializes the QtTcpClient object with a new QTcpSocket.
 * - Connects the QTcpSocket signals (connected, readyRead, and errorOccurred) to the respective slots in this class:
 *   - The 'connected' signal is connected to the 'onConnected' slot. This signal is emitted by QTcpSocket when it successfully establishes a connection with the server. The 'onConnected' slot is used to handle tasks that should occur once the connection is established, such as logging the connection status.
 *   - The 'readyRead' signal is connected to the 'onDataReceived' slot. This signal is emitted whenever there is data available to be read from the socket. The 'onDataReceived' slot handles the reading and processing of this data, which typically includes server responses to client requests.
 *   - The 'errorOccurred' signal is connected to the 'displayError' slot. This signal is emitted when a socket error occurs during operations like connecting, sending, or receiving data. The 'displayError' slot is used to handle and log these errors for debugging and user notification purposes.
 */
QtTcpClient::QtTcpClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::connected, this, &QtTcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &QtTcpClient::onDataReceived);
    connect(socket, &QTcpSocket::errorOccurred, this, &QtTcpClient::displayError);

}

/**
 * @brief Connects to a TCP server at the specified host and port.
 *
 * @param host The hostname or IP address of the server.
 * @param port The port number to connect to.
 *
 * @details
 * - Initiates a connection to the server using the QTcpSocket.
 */
void QtTcpClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

/**
 * @brief Sends a request to read current from a specified module and channel.
 *
 * @param moduleAlias Alias or name of the module to read from.
 * @param channelIndex Index of the channel on the module.
 *
 * @details
 * - Constructs a request string in the format "readCurrent;<moduleAlias>;<channelIndex>;".
 * - If the socket is connected, the request is sent to the server.
 * - The last request type is recorded as "readCurrent".
 */
void QtTcpClient::sendReadCurrentRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString request = QString("readCurrent;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readCurrent";
        socket->write(request.toUtf8());
    }
}

/**
 * @brief Sends a request to read voltage from a specified module and channel.
 *
 * @param moduleAlias Alias or name of the module to read from.
 * @param channelIndex Index of the channel on the module.
 *
 * @details
 * - Constructs a request string in the format "readVoltage;<moduleAlias>;<channelIndex>;".
 * - If the socket is connected, the request is sent to the server.
 * - The last request type is recorded as "readVoltage".
 */
void QtTcpClient::sendReadVoltageRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString request = QString("readVoltage;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readVoltage";
        socket->write(request.toUtf8());
    }
}


/**
 * @brief Slot triggered when the socket is successfully connected to the server.
 *
 * @details
 * - Logs a message indicating that the connection to the server has been established.
 */
void QtTcpClient::onConnected()
{
    qDebug() << "Connected to server";
    //TODO
}


/**
 * @brief Slot triggered when data is received from the server.
 *
 * @details
 * - Reads and processes the response from the server.
 * - If the response contains "NACK", an error signal is emitted.
 * - Otherwise, based on the last request type, the appropriate signal (currentReadedSignal or voltageReadedSignal) is emitted.
 */
void QtTcpClient::onDataReceived()
{
    QString response = QString::fromUtf8(socket->readAll());
    if (response.contains("NACK"))
    {
        emit errorSignal(response);
        qDebug() << "Error from server:" << response;
        return;
    }
    else if (m_lastRequest == "readCurrent")
    {
        emit currentReadedSignal(response);
    }
    else if (m_lastRequest == "readVoltage")
    {
       emit voltageReadedSignal(response);
    }

    qDebug() << "Response from server:" << response;
}

/**
 * @brief Slot triggered when a socket error occurs.
 *
 * @param socketError The type of socket error.
 *
 * @details
 * - Logs the type of socket error that occurred.
 */
void QtTcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
    //TODO
}
