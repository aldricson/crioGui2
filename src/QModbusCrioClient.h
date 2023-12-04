#ifndef QModbusCrioClient_H
#define QModbusCrioClient_H

#include <QObject>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QVariant>

class QModbusCrioClient : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit QModbusCrioClient(QObject *parent = nullptr);
    ~QModbusCrioClient();

    QString ip() const;
    void setIp(const QString &ip);

    int port() const;
    void setPort(int port);

    void readAnalogics(int registerStartIndex, int nbRegistersToRead);

signals:
    void analogsDataReady(const QVector<quint16> &data);
    void errorOccurred(const QString &error);

    void ipChanged(const QString &ip);
    void portChanged(int port);

private slots:
    void onReadReady();

private:
    QModbusTcpClient *m_modbusClient;
    QString m_ip;
    int m_port;

    void setupModbusClient();
    void handleError(const QString &error);
};

#endif // QMODBUSCLIENT_H
