#ifndef QMODBUSSETUPVIEWER_H
#define QMODBUSSETUPVIEWER_H

#include <QWidget>
#include <QSettings>


#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>

#include "QBetterSwitchButton.h"
#include "stringUtils.h"
#include "QIpAddressEditor.h"
#include "QModbusCrioClient.h"

class QLineEdit;
class QSpinBox;
class QPushButton;
class QGroupBox;
class QMessageBox;
class QtTcpClient;
class QModbusAnalogViewer;
class QMultiLineTextVisualizer;

class QModbusSetupViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)


public:
    explicit QModbusSetupViewer(QWidget *parent = nullptr);

    const QString &fileName() const;
    void setFileName(const QString &newFileName);

    const QString &host() const;
    void setHost(const QString &newHost);

    quint16 port() const;
    void setPort(quint16 newPort);

    void connectToServer();

public slots:
    void loadFromFile();
    void saveToFile();
    void uploadToServer();

private slots:
    void onExlogCompatibilityChanged();
    void onModbusSimulationOrAcquisitionChanged();
    void onStartStopModbusChanged();
    void onSimulationStarted(const QString &response);
    void onSimulationStoped(const QString &response);

signals:
    void fileNameChanged();

    void hostChanged();

    void portChanged();

private:
    QSettings *settings                    = nullptr;

    QGroupBox                  *m_containerGroupBox          = nullptr; // GroupBox for Modbus Setup
    QGroupBox                  *m_compatibilityLayerGroupBox = nullptr;
    QGroupBox                  *m_modbusCapacitiesGroupBox   = nullptr;
    QGroupBox                  *m_networkGroupBox            = nullptr;
    QLineEdit                  *coilsLineEdit                = nullptr;
    QLineEdit                  *discreteInputsLineEdit       = nullptr;
    QLineEdit                  *holdingRegistersLineEdit     = nullptr;
    QLineEdit                  *inputRegistersLineEdit       = nullptr;
    QLineEdit                  *listeningPortLineEdit        = nullptr;
    QIpAddressEditor           *listeningInterfaceIpEdit     = nullptr;
    QBetterSwitchButton        *compatibilityLayerSwitch     = nullptr;
    QBetterSwitchButton        *simulateAcquisitionSwitch    = nullptr;
    QBetterSwitchButton        *startStopModbusSwitch        = nullptr;
    QLineEdit                  *nbAnalogsInLineEdit          = nullptr;
    QLineEdit                  *nbAnalogsOutLineEdit         = nullptr;
    QLineEdit                  *nbCountersLineEdit           = nullptr;
    QPushButton                *reloadButton                 = nullptr;
    QPushButton                *saveButton                   = nullptr;
    QPushButton                *uploadButton                 = nullptr;
    QModbusAnalogViewer        *m_analogsViewer              = nullptr;
    QModbusCrioClient          *m_modbusClient               = nullptr;
    QtTcpClient                *m_tcpClient                  = nullptr;
    QMultiLineTextVisualizer   *m_comControl                 = nullptr;
    QString                    m_host;
    quint16                    m_port;

    QString m_fileName;
    int m_exlogOffset = 0;
    bool m_isSimul = true;

    void blockAllSignals(const bool &blocked);
    void setAllValidators();

};

#endif // QMODBUSSETUPVIEWER_H
