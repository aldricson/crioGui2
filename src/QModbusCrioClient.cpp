#include "QModbusCrioClient.h"
#include <QModbusReply>

QModbusCrioClient::QModbusCrioClient(QObject *parent)
    : QObject(parent), m_port(502)
{
    m_modbusClient = new QModbusTcpClient(this);
    setupModbusClient();
}

QModbusCrioClient::~QModbusCrioClient() {
    m_modbusClient->disconnectDevice();
}

QString QModbusCrioClient::ip() const {
    return m_ip;
}

void QModbusCrioClient::setIp(const QString &ip) {
    if (m_ip != ip) {
        m_ip = ip;
        setupModbusClient();
        emit ipChanged(ip);
    }
}

int QModbusCrioClient::port() const {
    return m_port;
}

void QModbusCrioClient::setPort(int port) {
    if (m_port != port) {
        m_port = port;
        setupModbusClient();
        emit portChanged(port);
    }
}

void QModbusCrioClient::readAnalogics(int registerStartIndex, int nbRegistersToRead) {
    if (!m_modbusClient->connectDevice())
    {
        qInfo()<<"connection failed";
        handleError("Connection failed");
        return;
    }
    qInfo()<<"connection success";
    QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters, registerStartIndex, nbRegistersToRead);
    QModbusReply *reply = m_modbusClient->sendReadRequest(readUnit, 0); // Unit ID is set to 1

    if (reply) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, &QModbusCrioClient::onReadReady);
        } else {
            delete reply; // Broadcast replies return immediately
        }
    } else {
        handleError("Read error");
    }
}

void QModbusCrioClient::onReadReady() {
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply) {
        return;
    }

    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();
        QVector<quint16> data;
        for (uint i = 0; i < unit.valueCount(); i++) {
            data.push_back(unit.value(i));
        }
        emit analogsDataReady(data);
    } else {
        handleError(reply->errorString());
    }

    reply->deleteLater();
}

void QModbusCrioClient::setupModbusClient() {
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, QVariant(m_ip));
    m_modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant(m_port));
}


void QModbusCrioClient::handleError(const QString &error)
{
    emit errorOccurred(error);
}
