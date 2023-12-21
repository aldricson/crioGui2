#ifndef QModbusCrioClient_H
#define QModbusCrioClient_H

#include <QObject>
#include <QModbusClient>
#include <QModbusDataUnit>

class QVariant;

class QModbusCrioClient : public QObject
{
    Q_OBJECT

public:
    explicit QModbusCrioClient(QObject *parent = nullptr);
    ~QModbusCrioClient();

    void setupModbusClient(int type); // type: 0 for Serial, 1 for TCP
    void connectToServer(const QString &ipAddress, int port);
    void disconnectFromServer();

    QModbusReply* sendReadRequest(const QModbusDataUnit &readUnit, int serverAddress);
    QModbusReply* sendWriteRequest(const QModbusDataUnit &writeUnit, int serverAddress);
    QModbusReply* readInputRegisters(int serverAddress, int startIndex, int numberOfRegisters);

signals:
    void errorOccurred(const QString &error);
    void connectionStateChanged(bool connected);

private:
    QModbusClient *modbusClient = nullptr;

    void handleStateChanged(QModbusDevice::State state);
    void handleErrorOccurred(QModbusDevice::Error error);
};

#endif // QModbusCrioClient_H
