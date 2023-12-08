#include "QModbusCrioClient.h"
#include <QModbusTcpClient>
#include <QModbusRtuSerialClient>
#include <QVariant>

QModbusCrioClient::QModbusCrioClient(QObject *parent)
    : QObject(parent), modbusClient(nullptr)
{
    setupModbusClient(1);
}

QModbusCrioClient::~QModbusCrioClient()
{
    if (modbusClient)
        modbusClient->disconnectDevice();
    delete modbusClient;
}

void QModbusCrioClient::setupModbusClient(int type)
{
    if (modbusClient)
    {
        modbusClient->disconnectDevice();
        delete modbusClient;
        modbusClient = nullptr;
    }

    if (type == 0) {
        modbusClient = new QModbusRtuSerialClient(this);
    }
    else if (type == 1)
    {
        modbusClient = new QModbusTcpClient(this);
        qInfo()<<"modbus TCP client created @"<<modbusClient;
    }

    connect(modbusClient, &QModbusClient::stateChanged,  this, &QModbusCrioClient::handleStateChanged);
    connect(modbusClient, &QModbusClient::errorOccurred, this, &QModbusCrioClient::handleErrorOccurred);
    qInfo()<<"modbus TCP client signal connected"<<modbusClient;
}

void QModbusCrioClient::connectToServer(const QString &ipAddress, int port)
{
    if (!modbusClient)
        return;

    modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, QVariant(port));
    modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ipAddress);
    qInfo()<<"modbus connection parameters set:"<<ipAddress<<":"<<port;

    modbusClient->setTimeout(1000); // Set timeout (in milliseconds)
    modbusClient->setNumberOfRetries(3); // Set number of retries

    // Attempt to connect
    if (!modbusClient->connectDevice())
    {
        qInfo()<<"Failed to connect: " + modbusClient->errorString();
        emit errorOccurred("Failed to connect: " + modbusClient->errorString());
        return;
    }
    qInfo()<<"Modbus client connection success";
}


void QModbusCrioClient::disconnectFromServer()
{
    if (modbusClient)
        modbusClient->disconnectDevice();
}

QModbusReply* QModbusCrioClient::sendReadRequest(const QModbusDataUnit &readUnit, int serverAddress)
{
    if (!modbusClient || modbusClient->state() != QModbusDevice::ConnectedState)
        return nullptr;

    return modbusClient->sendReadRequest(readUnit, serverAddress);
}

QModbusReply* QModbusCrioClient::sendWriteRequest(const QModbusDataUnit &writeUnit, int serverAddress)
{
    if (!modbusClient || modbusClient->state() != QModbusDevice::ConnectedState)
        return nullptr;

    return modbusClient->sendWriteRequest(writeUnit, serverAddress);
}

void QModbusCrioClient::handleStateChanged(QModbusDevice::State state)
{
    emit connectionStateChanged(state == QModbusDevice::ConnectedState);
}

void QModbusCrioClient::handleErrorOccurred(QModbusDevice::Error error)
{
    emit errorOccurred(modbusClient->errorString());
}

QModbusReply* QModbusCrioClient::readInputRegisters(int serverAddress, int startIndex, int numberOfRegisters)
{
    if (!modbusClient || modbusClient->state() != QModbusDevice::ConnectedState)
        return nullptr;

    // Create a QModbusDataUnit for input registers
    QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters, startIndex, numberOfRegisters);

    // Send the read request and return the reply object
    return modbusClient->sendReadRequest(readUnit, serverAddress);
}
