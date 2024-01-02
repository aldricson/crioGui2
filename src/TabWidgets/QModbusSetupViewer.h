#ifndef QMODBUSSETUPVIEWER_H
#define QMODBUSSETUPVIEWER_H

#include <QWidget>
#include <QSettings>
#include <QIntValidator>
#include <QRegularExpressionValidator>
#include <QRegularExpression>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include "BasicWidgets/QIpAddressEditor.h"
#include "stringUtils.h"


//Forward declarations may speed up compilation time a bit
//so the includes are now in cpp file
class QLineEdit               ;
class QSpinBox                ;
class QPushButton             ;
class QGroupBox               ;
class QMessageBox             ;
class QtTcpClient             ;
class QModbusAnalogViewer     ;
class QMultiLineTextVisualizer;
class QBetterSwitchButton     ;
class QModbusCrioClient       ;
class QModbusReply            ;
class QTimer                  ;

class QModbusSetupViewer : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString host     READ host     WRITE setHost     NOTIFY hostChanged)
    Q_PROPERTY(quint16 port     READ port     WRITE setPort     NOTIFY portChanged)
    Q_PROPERTY(QMultiLineTextVisualizer *debugOutput   READ debugOutput   WRITE setDebugOutput   NOTIFY debugOutputChanged)
    Q_PROPERTY(QModbusAnalogViewer      *analogsViewer READ analogsViewer WRITE setAnalogsViewer NOTIFY analogsViewerChanged)

public:
    explicit QModbusSetupViewer(QWidget *parent = nullptr);

    const QString &fileName() const;
    void setFileName(const QString &newFileName);

    const QString &host() const;
    void setHost(const QString &newHost);

    quint16 port() const;
    void setPort(quint16 newPort);

    void connectToServer();

    QMultiLineTextVisualizer *debugOutput() const;
    void setDebugOutput(QMultiLineTextVisualizer *newDebugOutput);

    QModbusAnalogViewer *analogsViewer() const;
    void setAnalogsViewer(QModbusAnalogViewer *newAnalogsViewer);

public slots:
    void loadFromFile();
    void saveToFile();
    void uploadToServer();

private slots:
    void onExlogCompatibilityChanged              ()                                ;
    void onModbusSimulationOrAcquisitionChanged   ()                                ;
    void onStartStopModbusChanged                 ()                                ;
    void onModbusTimer                             ()                                ;
    void onSimulationStarted                      (const QString          &response);
    void onSimulationStoped                       (const QString          &response);
    void onAcquisitionStarted                     (const QString          &response);
    void onAcquisitionStoped                      (const QString          &response);
    void OnAnalogsDataReady                       (const QVector<quint16> &data    );

signals:
    void fileNameChanged();

    void hostChanged();

    void portChanged();

    void debugOutputChanged();

    void analogsViewerChanged();

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
    QMultiLineTextVisualizer   *m_debugOutput                = nullptr;
    QTimer                     *m_modbusTimer                = nullptr;
    QString                    m_host                                 ;
    quint16                    m_port                                 ;
    int                        m_nbAnalogics                 = 0      ;
    QString                    m_fileName                             ;
    int                        m_exlogOffset                 = 0      ;
    bool                       m_isSimul                     = true   ;
    bool                       m_modbusReading               = false  ;

    void blockAllSignals            (const bool &blocked             );
    void setAllValidators           ()                                ;
    void createTCPClient            ()                                ;
    void createSectionGroupBoxes    (QGroupBox *parentGroupBox)       ;
    void createLoadSaveUploadButtons(QGroupBox *parentGroupBox )      ;
    void createSimulTimer           ()                                ;
    void setUpLayout                ()                                ;
    void processModbusReply         ()                                ;


};

#endif // QMODBUSSETUPVIEWER_H
