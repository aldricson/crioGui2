#include "QModbusSetupViewer.h"
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include "QtTcpClient.h"
#include "QMultiLineTextVisualizer.h"
#include "QModbusAnalogViewer.h"


QModbusSetupViewer::QModbusSetupViewer(QWidget *parent)
    : QWidget(parent)
{
    m_modbusClient = new QModbusCrioClient(this);
    m_tcpClient    = new QtTcpClient(this);
    m_comControl   = new QMultiLineTextVisualizer(this);
    connect (m_tcpClient, &QtTcpClient::simulationStartedSignal, this, &QModbusSetupViewer::onSimulationStarted , Qt::QueuedConnection);
    connect (m_tcpClient, &QtTcpClient::simulationStopedSignal , this, &QModbusSetupViewer::onSimulationStoped  , Qt::QueuedConnection);
    // Create GroupBoxes for different sections
    m_containerGroupBox          = new QGroupBox("Modbus Setup"         , this               );
    m_compatibilityLayerGroupBox = new QGroupBox("Exlog Compatibility"  , m_containerGroupBox);
    m_modbusCapacitiesGroupBox   = new QGroupBox("Modbus Capacities"    , m_containerGroupBox); // Corrected the name to match its purpose
    m_networkGroupBox            = new QGroupBox("Network Configuration", m_containerGroupBox); // Changed name to avoid confusion with Modbus Capacities

    // Create QLineEdit widgets for the m_modbusCapacitiesGroupBox
    coilsLineEdit            = new QLineEdit(m_modbusCapacitiesGroupBox);
    discreteInputsLineEdit   = new QLineEdit(m_modbusCapacitiesGroupBox);
    holdingRegistersLineEdit = new QLineEdit(m_modbusCapacitiesGroupBox);
    inputRegistersLineEdit   = new QLineEdit(m_modbusCapacitiesGroupBox);

    // Create QLineEdit widgets for the m_networkGroupBox
    listeningPortLineEdit    = new QLineEdit       (m_networkGroupBox);
    listeningInterfaceIpEdit = new QIpAddressEditor(m_networkGroupBox);

    // Widgets for the m_compatibilityLayerGroupBox
    compatibilityLayerSwitch = new QBetterSwitchButton("Off", "On", QColor(0xEE4B2B), QColor(0x6495ED), Qt::green, true, m_compatibilityLayerGroupBox);
    nbAnalogsInLineEdit      = new QLineEdit(m_compatibilityLayerGroupBox);
    nbAnalogsOutLineEdit     = new QLineEdit(m_compatibilityLayerGroupBox);
    nbCountersLineEdit       = new QLineEdit(m_compatibilityLayerGroupBox);

    // Create buttons for actions
    reloadButton = new QPushButton ("(Re)load", m_containerGroupBox);
    saveButton = new QPushButton   ("Save"    , m_containerGroupBox);
    uploadButton = new QPushButton ("Upload"  , m_containerGroupBox);

    // Connect buttons to slots
    connect(reloadButton, &QPushButton::clicked, this, &QModbusSetupViewer::loadFromFile);
    connect(saveButton  , &QPushButton::clicked, this, &QModbusSetupViewer::saveToFile);
    connect(uploadButton, &QPushButton::clicked, this, &QModbusSetupViewer::uploadToServer);

    // Create a form layout for the m_modbusCapacitiesGroupBox
    QFormLayout *modbusCapacitiesLayout = new QFormLayout(m_modbusCapacitiesGroupBox);
    modbusCapacitiesLayout->addRow("Coils Max:", coilsLineEdit);
    modbusCapacitiesLayout->addRow("Discrete Inputs Max:", discreteInputsLineEdit);
    modbusCapacitiesLayout->addRow("Holding Registers Max:", holdingRegistersLineEdit);
    modbusCapacitiesLayout->addRow("Input Registers Max:", inputRegistersLineEdit);
    m_modbusCapacitiesGroupBox->setLayout(modbusCapacitiesLayout);

    // Create a form layout for the m_networkGroupBox
    QFormLayout *networkLayout = new QFormLayout(m_networkGroupBox);
    networkLayout->addRow("Listening Port:", listeningPortLineEdit);
    networkLayout->addRow("Listening Interface:", listeningInterfaceIpEdit);
    m_networkGroupBox->setLayout(networkLayout);

    // Create a form layout for the m_compatibilityLayerGroupBox
    QFormLayout *compatibilityLayout = new QFormLayout(m_compatibilityLayerGroupBox);
    compatibilityLayout->addRow("Compatibility Layer:", compatibilityLayerSwitch);
    compatibilityLayout->addRow("Nb Analogs In:", nbAnalogsInLineEdit);
    compatibilityLayout->addRow("Nb Analogs Out:", nbAnalogsOutLineEdit);
    compatibilityLayout->addRow("Nb Counters:", nbCountersLineEdit);
    m_compatibilityLayerGroupBox->setLayout(compatibilityLayout);

    // Create a layout for buttons
    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(reloadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(uploadButton);

    // Create a main layout to arrange the group boxes and button layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_modbusCapacitiesGroupBox);
    mainLayout->addWidget(m_networkGroupBox);
    mainLayout->addWidget(m_compatibilityLayerGroupBox);
    mainLayout->addLayout(buttonLayout);

    // Set the main layout to m_containerGroupBox
    m_containerGroupBox->setLayout(mainLayout);

    m_analogsViewer = new QModbusAnalogViewer(this);
    // Create a layout for this widget and add the m_containerGroupBox to it
    QGridLayout *thisLayout = new QGridLayout(this);
    startStopModbusSwitch     = new QBetterSwitchButton("stop ", "start", QColor(0xEE4B2B), QColor(0x6495ED), Qt::green, false, this);
    simulateAcquisitionSwitch = new QBetterSwitchButton("simulate", " mapped ", QColor(0xEE4B2B), QColor(0x6495ED), Qt::green, false, this);

    thisLayout->addWidget(startStopModbusSwitch ,0,0,1,1);
    thisLayout->addWidget(startStopModbusSwitch ,0,1,1,1);
    thisLayout->addWidget(m_containerGroupBox   ,1,0,1,1);
    thisLayout->addWidget(m_analogsViewer       ,1,1,1,1);
    thisLayout->addWidget(m_comControl          ,2,0,1,1);
    setLayout(thisLayout);

    connect(compatibilityLayerSwitch  , &QBetterSwitchButton::stateChanged , this, &QModbusSetupViewer::onExlogCompatibilityChanged, Qt::QueuedConnection);
    connect(startStopModbusSwitch     , &QBetterSwitchButton::stateChanged , this, &QModbusSetupViewer::onStartStopModbusChanged   , Qt::QueuedConnection);
    connect(simulateAcquisitionSwitch , &QBetterSwitchButton::stateChanged , this, &QModbusSetupViewer::onModbusSimulationOrAcquisitionChanged, Qt::QueuedConnection);
}



void QModbusSetupViewer::loadFromFile()
{
    blockAllSignals(true);

    // Correct group and key names are used based on the provided content structure
    settings->beginGroup("coils");
    coilsLineEdit->setText(settings->value("max").toString());
    settings->endGroup();

    settings->beginGroup("discreteinputs");
    discreteInputsLineEdit->setText(settings->value("max").toString());
    settings->endGroup();

    settings->beginGroup("holdingregisters");
    holdingRegistersLineEdit->setText(settings->value("max").toString());
    settings->endGroup();

    settings->beginGroup("inputregisters");
    inputRegistersLineEdit->setText(settings->value("max").toString());
    settings->endGroup();

    settings->beginGroup("network");
    listeningPortLineEdit->setText(settings->value("listeningport").toString());
    listeningInterfaceIpEdit->setIp(settings->value("listeninginterface").toString());
    settings->endGroup();

    settings->beginGroup("exlog");
    QString boolStr = settings->value("compatibilitylayer").toString();
    bool compatible = textToBool(boolStr);
    compatibilityLayerSwitch->blockSignals(true);
    compatibilityLayerSwitch->setState(compatible);
    compatibilityLayerSwitch->blockSignals(false);
    compatible ? m_exlogOffset=1 : m_exlogOffset=0;
    m_analogsViewer->resetOffset(m_exlogOffset);
    settings->endGroup();

    settings->beginGroup("exlogmapping");
    nbAnalogsInLineEdit->setText(settings->value("nbanalogsin").toString());
    nbAnalogsOutLineEdit->setText(settings->value("nbanalogsout").toString());
    nbCountersLineEdit->setText(settings->value("nbcounters").toString());
    settings->endGroup();

    blockAllSignals(false);
}

void QModbusSetupViewer::saveToFile()
{
    blockAllSignals(true);

    // Correct group and key names are used based on the provided content structure
    settings->beginGroup("coils");
    settings->setValue("max", coilsLineEdit->text());
    settings->endGroup();

    settings->beginGroup("discreteinputs");
    settings->setValue("max", discreteInputsLineEdit->text());
    settings->endGroup();

    settings->beginGroup("holdingregisters");
    settings->setValue("max", holdingRegistersLineEdit->text());
    settings->endGroup();

    settings->beginGroup("inputregisters");
    settings->setValue("max", inputRegistersLineEdit->text());
    settings->endGroup();

    settings->beginGroup("network");
    settings->setValue("listeningport", listeningPortLineEdit->text());
    settings->setValue("listeninginterface", listeningInterfaceIpEdit->ipAddress());
    settings->endGroup();

    settings->beginGroup("exlog");
    QString compatible = boolToText(compatibilityLayerSwitch->getState());
    settings->setValue("compatibilitylayer", compatible);
    settings->endGroup();

    settings->beginGroup("exlogmapping");
    settings->setValue("nbanalogsin", nbAnalogsInLineEdit->text());
    settings->setValue("nbanalogsout", nbAnalogsOutLineEdit->text());
    settings->setValue("nbcounters", nbCountersLineEdit->text());
    settings->endGroup();

    blockAllSignals(false);
}


void QModbusSetupViewer::uploadToServer()
{
    // Placeholder for uploading settings to the server
}

void QModbusSetupViewer::onExlogCompatibilityChanged()
{
    compatibilityLayerSwitch->getState() ? m_exlogOffset=1 : m_exlogOffset=0;
    m_analogsViewer->resetOffset(m_exlogOffset);
}

void QModbusSetupViewer::onModbusSimulationOrAcquisitionChanged()
{
    m_isSimul = !simulateAcquisitionSwitch->getState();
    if (!m_isSimul)
    {
        // Create an information message box
        QMessageBox infoBox;
        infoBox.setIconPixmap(QPixmap(":/path/to/your/logo.png")); // Set the logo pixmap
        infoBox.setWindowTitle("Work in Progress");
        infoBox.setText("Come back soon,\nsoft returns in simulation now.");
        infoBox.exec(); // Display the message box
        simulateAcquisitionSwitch->blockSignals(true);
        simulateAcquisitionSwitch->setState(false);
        simulateAcquisitionSwitch->blockSignals(false);
        simulateAcquisitionSwitch->update();
    }
    else
    {

    }
}

void QModbusSetupViewer::onStartStopModbusChanged()
{
    bool mustStartOrStop = startStopModbusSwitch->getState();

    if (m_isSimul)
    {

        if (mustStartOrStop)
        {
            //must start
            simulateAcquisitionSwitch->setEnabled(false);
            simulateAcquisitionSwitch->update();
            m_comControl->addLastCommand("Starting modbus simulation on Crio");
            m_tcpClient->sendStartModbusSimulation();

        }
        else
        {
            //must stop
             m_comControl->addLastCommand("Stopping modbus simulation on Crio");
             m_tcpClient->sendStopSimulation();
        }
    }
    else
    {
        //TODO
    }

}

void QModbusSetupViewer::onSimulationStarted(const QString &response)
{
    if (response.contains("ACK"))
    {
        m_comControl->addLastOutput("Modbus server simulation on");
    }
    else if (response.contains("NACK"))
    {
        m_comControl->addLastError("Somthing gone wrong when trying to stop simulation:\n"+response);
    }
}

void QModbusSetupViewer::onSimulationStoped(const QString &response)
{
    if (response.contains("ACK"))
    {
        m_comControl->addLastOutput("Modbus server simulation off");
    }
    else if (response.contains("NACK"))
    {
        m_comControl->addLastError("Somthing gone wrong when trying to stop simulation:\n"+response);
    }
    simulateAcquisitionSwitch->setEnabled(true);
    simulateAcquisitionSwitch->update();
}

quint16 QModbusSetupViewer::port() const
{
    return m_port;
}

void QModbusSetupViewer::setPort(quint16 newPort)
{
    m_port = newPort;
    m_tcpClient->setPort(m_port);
    emit portChanged();
}

void QModbusSetupViewer::connectToServer()
{
    m_tcpClient->connectToServer(m_host,m_port,"Modbus control");
}

const QString &QModbusSetupViewer::host() const
{
    return m_host;
}

void QModbusSetupViewer::setHost(const QString &newHost)
{
    m_host = newHost;
    m_tcpClient->setHost(m_host);
    emit hostChanged();
}

void QModbusSetupViewer::blockAllSignals(const bool &blocked)
{
    m_containerGroupBox                   -> blockSignals(blocked);
    coilsLineEdit              -> blockSignals(blocked);
    discreteInputsLineEdit     -> blockSignals(blocked);
    holdingRegistersLineEdit   -> blockSignals(blocked);
    inputRegistersLineEdit     -> blockSignals(blocked);
    listeningPortLineEdit      -> blockSignals(blocked);
    listeningInterfaceIpEdit   -> blockSignals(blocked);
    compatibilityLayerSwitch   -> blockSignals(blocked);
    nbAnalogsInLineEdit        -> blockSignals(blocked);
    nbAnalogsOutLineEdit       -> blockSignals(blocked);
    nbCountersLineEdit         -> blockSignals(blocked);
    reloadButton               -> blockSignals(blocked);
    saveButton                 -> blockSignals(blocked);
    uploadButton               -> blockSignals(blocked);
}

const QString &QModbusSetupViewer::fileName() const
{
    return m_fileName;
}

void QModbusSetupViewer::setFileName(const QString &newFileName)
{
    if (m_fileName == newFileName)
        return;
    m_fileName = newFileName;
    if (settings)
    {
        delete settings;
        settings = nullptr;
    }
   // Recreate the settings object with the new file name
   settings = new QSettings(m_fileName, QSettings::IniFormat);
   emit fileNameChanged();
}


void QModbusSetupViewer::setAllValidators()
{
    // Integer validator for Modbus capacities (assuming max value of 65535 is reasonable for these settings)
    QIntValidator *intValidator = new QIntValidator(0, 65535, this);

    coilsLineEdit->setValidator(intValidator);
    discreteInputsLineEdit->setValidator(intValidator);
    holdingRegistersLineEdit->setValidator(intValidator);
    inputRegistersLineEdit->setValidator(intValidator);
    nbAnalogsInLineEdit->setValidator(intValidator);
    nbAnalogsOutLineEdit->setValidator(intValidator);
    nbCountersLineEdit->setValidator(intValidator);

    // Port validator (1-65535)
    QIntValidator *portValidator = new QIntValidator(1, 65535, this);
    listeningPortLineEdit->setValidator(portValidator);


}

