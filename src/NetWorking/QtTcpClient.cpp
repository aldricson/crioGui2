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
 *   - The 'connected' signal is connected to the 'onConnected' slot. This signal is Q_EMITted by QTcpSocket when it successfully establishes a connection with the server. The 'onConnected' slot is used to handle tasks that should occur once the connection is established, such as logging the connection status.
 *   - The 'readyRead' signal is connected to the 'onDataReceived' slot. This signal is Q_EMITted whenever there is data available to be read from the socket. The 'onDataReceived' slot handles the reading and processing of this data, which typically includes server responses to client requests.
 *   - The 'errorOccurred' signal is connected to the 'displayError' slot. This signal is Q_EMITted when a socket error occurs during operations like connecting, sending, or receiving data. The 'displayError' slot is used to handle and log these errors for debugging and user notification purposes.
 */
QtTcpClient::QtTcpClient(QString clientName, QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{
    this->setObjectName(clientName);
    connect(socket, &QTcpSocket::connected, this, &QtTcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &QtTcpClient::onDataReceived);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
                    this,
                    &QtTcpClient::displayError);
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
void QtTcpClient::connectToServer(const QString &host, quint16 port,const QString &message)
{
    qInfo()<<message;
    socket->connectToHost(host, port);
}

void QtTcpClient::disconnectFromServer()
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
         socket->disconnectFromHost();
         if (socket->state() != QAbstractSocket::UnconnectedState)
         {
             socket->waitForDisconnected();  // Wait for the socket to disconnect
         }
         qInfo()<<this->objectName()<<" TCP client disconnected gracefully";
     }
}

/**
 * @brief Connects to a TCP server at the specified host and port.
 *
 * @param host The hostname or IP address of the server.
 * @param port The port number to connect to.
 *
 * @return True if the connection is successful, otherwise False.
 *
 * @details
 * - Initiates a connection to the server using the QTcpSocket.
 * - Returns true if the connection is successful, otherwise returns false.
 */
bool QtTcpClient::checkConnect(const QString &host, quint16 port)
{
    if (socket->state() != QAbstractSocket::ConnectedState)
    {
        setHost(host);
        setPort(port);
        socket->connectToHost(m_host, m_port);
        return socket->waitForConnected(); // Returns true if connected successfully.
    }
    return true; // Already connected.
}

/**
 * @brief Sends a request to read current from a specified module and channel.
 *
 * @param moduleAlias Alias or name of the module to read from.
 * @param channelIndex Index of the channel on the module.
 *
 * @return True if the request is sent successfully, otherwise False.
 *
 * @details
 * - Constructs a request string in the format "readCurrent;<moduleAlias>;<channelIndex>;".
 * - If the socket is connected, the request is sent to the server.
 * - The last request type is recorded as "readCurrent".
 * - Returns true if the request is sent successfully, otherwise returns false.
 */
void QtTcpClient::sendReadCurrentRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (checkConnect(m_host, m_port)) {
        QString request = QString("readCurrent;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readCurrent";
        socket->write(request.toUtf8());
        socket->waitForBytesWritten(); // Returns true if the request is sent successfully.
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

/**
 * @brief Sends a request to read voltage from a specified module and channel.
 *
 * @param moduleAlias Alias or name of the module to read from.
 * @param channelIndex Index of the channel on the module.
 *
 * @return True if the request is sent successfully, otherwise False.
 *
 * @details
 * - Constructs a request string in the format "readVoltage;<moduleAlias>;<channelIndex>;".
 * - If the socket is connected, the request is sent to the server.
 * - The last request type is recorded as "readVoltage".
 * - Returns true if the request is sent successfully, otherwise returns false.
 */
void QtTcpClient::sendReadVoltageRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (checkConnect(m_host, m_port))
    {
        QString request = QString("readVoltage;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readVoltage";
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

/**
 * @brief Sends a request to start Modbus simulation.
 *
 * @return True if the request is sent successfully, otherwise False.
 *
 * @details
 * - Constructs a request string "startModbusSimulation".
 * - If the socket is connected, the request is sent to the server.
 * - The last request type is recorded as "startModbusSimulation".
 * - Returns true if the request is sent successfully, otherwise returns false.
 */
void QtTcpClient::sendStartModbusSimulation()
{
    if (checkConnect(m_host, m_port))
    {
        QString request = "startModbusSimulation";
        m_lastRequest = request;
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

void QtTcpClient::sendStopSimulation()
{
    if (checkConnect(m_host, m_port))
    {
        QString request = "stopModbusSimulation";
        m_lastRequest = request;
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

void QtTcpClient::sendStartModbusAcquisition()
{
    if (checkConnect(m_host, m_port))
    {
        QString request = "startModbusAcquisition";
        m_lastRequest = request;
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

void QtTcpClient::sendStopModbusAcquisition()
{
    if (checkConnect(m_host, m_port))
    {
        QString request = "stopModbusAcquisition";
        m_lastRequest = request;
        socket->write(request.toUtf8());
        socket->waitForBytesWritten();
    }
    else
    {
        Q_EMIT socketNotConnectedSignal("NACK: socket not connected");
    }
}

const QString &QtTcpClient::host() const
{
    return m_host;
}

void QtTcpClient::setHost(const QString &newHost)
{
    if (m_host == newHost)
        return;
    m_host = newHost;
    Q_EMIT hostChanged();
}

quint16 QtTcpClient::port() const
{
    return m_port;
}

void QtTcpClient::setPort(quint16 newPort)
{
    if (m_port == newPort)
        return;
    m_port = newPort;
    Q_EMIT portChanged();
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
 * - If the response contains "NACK", an error signal is Q_EMITted.
 * - Otherwise, based on the last request type, the appropriate signal (currentReadedSignal or voltageReadedSignal) is Q_EMITted.
 */
void QtTcpClient::onDataReceived()
{
    QString response = QString::fromUtf8(socket->readAll());
    if (response.contains("NACK"))
    {
        Q_EMIT errorSignal(response);
        qDebug() << "Error from server:" << response;
        return;
    }
    else if (m_lastRequest == "readCurrent")
    {
        Q_EMIT currentReadedSignal(response);
    }
    else if (m_lastRequest == "readVoltage")
    {
       Q_EMIT voltageReadedSignal(response);
    }
    else if (m_lastRequest == "startModbusSimulation")
    {
        Q_EMIT simulationStartedSignal(response);
    }
    else if (m_lastRequest == "stopModbusSimulation")
    {
        Q_EMIT simulationStopedSignal(response);
    }
    else if (m_lastRequest == "startModbusAcquisition")
    {
        Q_EMIT acquisitionStartedSignal(response);
    }
    else if (m_lastRequest == "stopModbusAcquisition")
    {
        Q_EMIT acquisitionStopedSignal(response);
    }



  //  qDebug() << "Response to " << m_lastRequest<< "from server:" << response;
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
