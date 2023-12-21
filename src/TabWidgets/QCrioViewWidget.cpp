/**
 * @file QCrioViewWidget.cpp
 * @brief Implementation of the QCrioViewWidget class for managing UI components and interactions in the CrioView tab.
 *
 * @class QCrioViewWidget
 * @extends QWidget
 *
 * The QCrioViewWidget class is designed to encapsulate all user interface components and functionalities specific to the CrioView tab.
 * It includes various UI elements like IP address editor, login and password fields, connect button, server state label, and custom test widgets.
 *
 * Key functionalities and methods include:
 * - Initialization of UI components and layout configuration.
 * - Setup of signal-slot connections for UI interactions.
 * - Handlers for changes in UI components like IP address, login, and password.
 * - Emitting signals for SSH connection requests and server state changes.
 * - Utility methods for updating UI components based on server and module status.
 *
 * @note This class is responsible for handling the user interface logic related to the CrioView tab, separating it from the main window logic.
 *
 * Detailed method documentation:
 * - setupUi(): Initializes and configures all UI elements within the widget.
 * - setupConnections(): Establishes signal-slot connections for interactive UI components.
 * - handleConnection(): Slot triggered by the connect button; emits a signal to request an SSH connection.
 * - onServerChangeState(): Slot triggered by the server state switch button; emits a signal to indicate a change in the server state.
 * - Various setters and getters for individual UI components, providing encapsulated access and modification of UI elements.
 */

#include "QCrioViewWidget.h"
#include <QIcon>
#include "BasicWidgets/QWaitWindow.h"


QCrioViewWidget::QCrioViewWidget(const QString &inifilePath, QWidget *parent) : QWidget(parent)
{
    m_iniPath = inifilePath;
    setupUi();
    setupConnections();
}

QLabel *QCrioViewWidget::ipLabel() const
{
    return m_ipLabel;
}

void QCrioViewWidget::setIpLabel(QLabel *newIpLabel)
{
    if (m_ipLabel == newIpLabel)
        return;
    m_ipLabel = newIpLabel;
    emit ipLabelChangedSignal();
}

QIpAddressEditor *QCrioViewWidget::ipEdit() const
{
    return m_ipEdit;
}

void QCrioViewWidget::setIpEdit(QIpAddressEditor *newIpEdit)
{
    if (m_ipEdit == newIpEdit)
        return;
    m_ipEdit = newIpEdit;
    emit ipEditChangedSignal();
}

QLabel *QCrioViewWidget::passwordLabel() const
{
    return m_passwordLabel;
}

void QCrioViewWidget::setPasswordLabel(QLabel *newPasswordLabel)
{
    if (m_passwordLabel == newPasswordLabel)
        return;
    m_passwordLabel = newPasswordLabel;
    emit passwordLabelChangedSignal();
}

QLineEdit *QCrioViewWidget::passwordEdit() const
{
    return m_passwordEdit;
}

void QCrioViewWidget::setPasswordEdit(QLineEdit *newPasswordEdit)
{
    if (m_passwordEdit == newPasswordEdit)
        return;
    m_passwordEdit = newPasswordEdit;
    emit passwordEditChangedSignal();
}

QAction *QCrioViewWidget::togglePasswordAction() const
{
    return m_togglePasswordAction;
}

void QCrioViewWidget::setTogglePasswordAction(QAction *newTogglePasswordAction)
{
    if (m_togglePasswordAction == newTogglePasswordAction)
        return;
    m_togglePasswordAction = newTogglePasswordAction;
    emit togglePasswordActionChangedSignal();
}

QLabel *QCrioViewWidget::loginLabel() const
{
    return m_loginLabel;
}

void QCrioViewWidget::setLoginLabel(QLabel *newLoginLabel)
{
    if (m_loginLabel == newLoginLabel)
        return;
    m_loginLabel = newLoginLabel;
    emit loginLabelChangedSignal();
}

QLineEdit *QCrioViewWidget::loginEdit() const
{
    return m_loginEdit;
}

void QCrioViewWidget::setLoginEdit(QLineEdit *newLoginEdit)
{
    if (m_loginEdit == newLoginEdit)
        return;
    m_loginEdit = newLoginEdit;
    emit loginEditChangedSignal();
}

QPushButton *QCrioViewWidget::connectButton() const
{
    return m_connectButton;
}

void QCrioViewWidget::setConnectButton(QPushButton *newConnectButton)
{
    if (m_connectButton == newConnectButton)
        return;
    m_connectButton = newConnectButton;
    emit connectButtonChangedSignal();
}

QLabel *QCrioViewWidget::serverStateLabel() const
{
    return m_serverStateLabel;
}

void QCrioViewWidget::setServerStateLabel(QLabel *newServerStateLabel)
{
    if (m_serverStateLabel == newServerStateLabel)
        return;
    m_serverStateLabel = newServerStateLabel;
    emit serverStateLabelChangedSignal();
}

QBetterSwitchButton *QCrioViewWidget::startStopServerSwitchButton() const
{
    return m_startStopServerSwitchButton;
}

void QCrioViewWidget::setStartStopServerSwitchButton(QBetterSwitchButton *newStartStopServerSwitchButton)
{
    if (m_startStopServerSwitchButton == newStartStopServerSwitchButton)
        return;
    m_startStopServerSwitchButton = newStartStopServerSwitchButton;
    emit startStopServerSwitchButtonChangedSignal();
}

QMultiLineTextVisualizer *QCrioViewWidget::terminalOutput() const
{
    return m_terminalOutput;
}

void QCrioViewWidget::setTerminalOutput(QMultiLineTextVisualizer *newTerminalOutput)
{
    if (m_terminalOutput == newTerminalOutput)
        return;
    m_terminalOutput = newTerminalOutput;
    emit terminalOutputChangedSignal();
}

QReadCurrentTestWidget *QCrioViewWidget::currentTestWidget() const
{
    return m_currentTestWidget;
}

void QCrioViewWidget::setCurrentTestWidget(QReadCurrentTestWidget *newCurrentTestWidget)
{
    if (m_currentTestWidget == newCurrentTestWidget)
        return;
    m_currentTestWidget = newCurrentTestWidget;
    emit currentTestWidgetChangedSignal();
}

QReadVoltageTestWidget *QCrioViewWidget::voltageTestWidget() const
{
    return m_voltageTestWidget;
}

void QCrioViewWidget::setVoltageTestWidget(QReadVoltageTestWidget *newVoltageTestWidget)
{
    if (m_voltageTestWidget == newVoltageTestWidget)
        return;
    m_voltageTestWidget = newVoltageTestWidget;
    emit voltageTestWidgetChangedSignal();
}

QProgressBar *QCrioViewWidget::modulesLoadingProgressBar() const
{
    return m_modulesLoadingProgressBar;
}

void QCrioViewWidget::setModulesLoadingProgressBar(QProgressBar *newModulesLoadingProgressBar)
{
    if (m_modulesLoadingProgressBar == newModulesLoadingProgressBar)
        return;
    m_modulesLoadingProgressBar = newModulesLoadingProgressBar;
    emit modulesLoadingProgressBarChanged();
}

QModulesIniListView *QCrioViewWidget::modulesListView()
{
    return m_modulesListView;
}

void QCrioViewWidget::setModulesListView(QModulesIniListView *newModulesListView)
{
    if (m_modulesListView == newModulesListView)
        return;
    m_modulesListView = newModulesListView;
    emit modulesListViewChanged();
}

QLabel *QCrioViewWidget::moduleListLabel() const
{
    return m_moduleListLabel;
}

void QCrioViewWidget::setModuleListLabel(QLabel *newModuleListLabel)
{
    if (m_moduleListLabel == newModuleListLabel)
        return;
    m_moduleListLabel = newModuleListLabel;
    emit moduleListLabelChanged();
}

QStringListModel *QCrioViewWidget::moduleListModel() const
{
    return m_moduleListModel;
}

void QCrioViewWidget::setModuleListModel(QStringListModel *newModuleListModel)
{
    if (m_moduleListModel == newModuleListModel)
        return;
    m_moduleListModel = newModuleListModel;
    emit moduleListModelChanged();
}

void QCrioViewWidget::onServerStartSuccesfull(const int &screenSession, const QString &lastCommand)
{
    m_terminalOutput->addLastCommand (lastCommand);
    m_terminalOutput->addLastOutput("Server started successfully");
    QMessageBox::information(nullptr, "Server Status", "Server started successfully", QMessageBox::Ok);
    m_startStopServerSwitchButton->setEnabled(true);
    m_startStopServerSwitchButton->blockSignals(true);
    m_startStopServerSwitchButton->setState(true);
    m_startStopServerSwitchButton->blockSignals(false);
    m_startStopServerSwitchButton->update();
    currentTestWidget()->setEnabled(true);
    voltageTestWidget()->setEnabled(true);
    serverStateLabel()->setText("server status:\n running on\nsession"+QString::number(screenSession));

}

void QCrioViewWidget::onResetModuleLoadProgressBar(const int &min, const int &max, const int &value)
{
   modulesLoadingProgressBar()->setMaximum(max);
   modulesLoadingProgressBar()->setMinimum(min);
   modulesLoadingProgressBar()->setValue(value);
}

void QCrioViewWidget::onUpdateModelList(QIniTreeWidget *tree, QStringList &moduleList)
{
    m_modulesListView->setIniTreeWidget(tree);
    m_moduleListModel->setStringList(moduleList);
    m_modulesListView->setModel(m_moduleListModel);
    m_terminalOutput->addLastOutput("update module list succes");
}



void QCrioViewWidget::onFillChannelsWithModules(QComboBox *modulesCb, QComboBox *channelCb, int index)
{
    if (!modulesCb || !channelCb) return; // Check for null pointers

    // Check if index is within the valid range
    if (index < 0 || index >= modulesCb->count()) return;

    QVariant userData = modulesCb->itemData(index);
    NIDeviceModule* module = userData.value<NIDeviceModule*>();
    if (module)
    {
        channelCb->blockSignals(true);
        channelCb->clear();

        std::vector<std::string> strLst;
        try {
            strLst = module->getChanNames();
        } catch (const std::exception& e) {
            // Handle any exceptions thrown by getChanNames
            // e.g., log the error or display a message to the user
            channelCb->blockSignals(false);
            return;
        }

        for (const auto& name : strLst)
        {
            channelCb->addItem(QString::fromStdString(name));
        }
        channelCb->blockSignals(false);
    }
}


void QCrioViewWidget::onUpdateControlsAfterModulesDownloaded(QCrioModulesDataExtractor *dataExtractor,
                                                             QStringList &currentModulePathList,
                                                             QStringList &voltageModulePathList,
                                                             const quint16 &commandPort)
{
    m_startStopServerSwitchButton->setEnabled(true);
    m_startStopServerSwitchButton->update();
    //check the state of the witch false = server stoped , true server started
    if (m_startStopServerSwitchButton->getState())
    {
        //fill the combobox with modules
        dataExtractor->extractCurrentModules(m_currentTestWidget->getModulesComboBox());
        onFillChannelsWithModules(m_currentTestWidget->getModulesComboBox(),
                                  m_currentTestWidget->getChannelComboBox(),
                                  0);

        dataExtractor->extractVoltageModules(m_voltageTestWidget->getModulesComboBox());
        onFillChannelsWithModules(m_voltageTestWidget->getModulesComboBox(),
                                  m_voltageTestWidget->getChannelComboBox(),
                                  0);


        //we set the host and the port for this 2 Vizu widgets because they have their own TCP client
        m_currentTestWidget->setHost(m_ipEdit->ipAddress());
        m_currentTestWidget->setPort(commandPort);
        m_currentTestWidget->tcpConnect();

        m_voltageTestWidget->setHost(m_ipEdit->ipAddress());
        m_voltageTestWidget->setPort(commandPort);
        m_voltageTestWidget->tcpConnect();

    }
}

void QCrioViewWidget::onUpdateControlsInReactionToServerState(const QString &lastCommand, const bool &serverState)
{
    m_terminalOutput->addLastCommand(lastCommand);
    m_currentTestWidget->setEnabled(serverState);
    m_voltageTestWidget->setEnabled(serverState);

    if (m_startStopServerSwitchButton->getState()!=serverState)
    {
        m_startStopServerSwitchButton->setEnabled(true);
        m_startStopServerSwitchButton->setState(serverState);
        m_startStopServerSwitchButton->update();
        m_startStopServerSwitchButton->setEnabled(false);
    }
}

void QCrioViewWidget::onUpdateControlsInReactionOfaStopFailure()
{
    m_terminalOutput->addLastError("failed to stop server");
    m_currentTestWidget->setEnabled(false);
    m_voltageTestWidget->setEnabled(false);
}

const QString &QCrioViewWidget::iniPath() const
{
    return m_iniPath;
}

void QCrioViewWidget::setIniPath(const QString &newIniPath)
{
    if (m_iniPath == newIniPath)
        return;
    m_iniPath = newIniPath;
    emit iniPathChanged();
}

QMultiLineTextVisualizer *QCrioViewWidget::crioUDPDebugOutput() const
{
    return m_crioUDPDebugOutput;
}

void QCrioViewWidget::setCrioUDPDebugOutput(QMultiLineTextVisualizer *newCrioUDPDebugOutput)
{
    m_crioUDPDebugOutput = newCrioUDPDebugOutput;
    emit crioUDPDebugOutputChanged();
}



void QCrioViewWidget::setupUi()
{
    QGridLayout *layout = new QGridLayout(this);

    // Initialize and set up all UI components
    m_moduleListModel = new QStringListModel(this);
    m_moduleListLabel = new QLabel(this);
    m_moduleListLabel->setText("modules list:");
    //m_modulesListView           = new QListView(this);
    m_modulesListView             = new QModulesIniListView(m_moduleListModel,m_iniPath,this);
    m_modulesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_modulesLoadingProgressBar = new QProgressBar(this);


    m_ipLabel = new QLabel("Crio Ip", this);
    m_ipEdit  = new QIpAddressEditor(this);

    m_loginLabel = new QLabel("login", this);
    m_loginEdit = new QLineEdit(this);

    m_passwordLabel = new QLabel("password", this);
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_togglePasswordAction = m_passwordEdit->addAction(QIcon(":/images/cacher.png"), QLineEdit::TrailingPosition);
    m_togglePasswordAction->setCheckable(true);


    m_connectButton = new QPushButton(tr("Connect"), this);
    m_serverStateLabel = new QLabel(tr("server status"), this);

    m_startStopServerSwitchButton = new QBetterSwitchButton(" stopped ", " active ", QColor(0xf54444), QColor(0x449ef5), QColor(0x44f547), false, this);
    m_startStopServerSwitchButton->setEnabled(false);

    m_currentTestWidget   = new QReadCurrentTestWidget  (this);
    m_voltageTestWidget   = new QReadVoltageTestWidget  (this);
    m_terminalOutput      = new QMultiLineTextVisualizer(this);
    m_crioUDPDebugOutput  = new QMultiLineTextVisualizer(this);

    // Add widgets to the layout
    layout->addWidget(m_ipLabel                     , 0,0,1,1,Qt::AlignHCenter);
    layout->addWidget(m_loginLabel                  , 0,1,1,1,Qt::AlignHCenter);
    layout->addWidget(m_passwordLabel               , 0,2,1,1,Qt::AlignHCenter);
    layout->addWidget(m_serverStateLabel            , 0,4,1,1, Qt::AlignHCenter);
    layout->addWidget(m_ipEdit                      , 1,0,1,1);
    layout->addWidget(m_loginEdit                   , 1,1,1,1);
    layout->addWidget(m_passwordEdit                , 1,2,1,1);
    layout->addWidget(m_connectButton               , 1,3,1,1);
    layout->addWidget(m_startStopServerSwitchButton , 1,4,1,1, Qt::AlignHCenter);

    layout->addWidget(m_moduleListLabel             , 2,0,1,1,Qt::AlignHCenter);
    layout->addWidget(m_modulesListView             , 3,0,1,1);
    layout->addWidget(m_modulesLoadingProgressBar   , 4,0,1,1);



    layout->addWidget(m_currentTestWidget           , 3,1,1,1);
    layout->addWidget(m_voltageTestWidget           , 3,2,1,1);


    layout->addWidget(m_terminalOutput              , 5,0,1,2);
    layout->addWidget(m_crioUDPDebugOutput          , 5,2,1,3);


    this->setLayout(layout);
}

void QCrioViewWidget::setupConnections() {
    // Toggle password visibility
    connect(m_togglePasswordAction, &QAction::toggled, this, [this](bool checked) {
        m_togglePasswordAction->setIcon(QIcon(checked ? ":/images/oeil.png" : ":/images/cacher.png"));
        m_passwordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
    //Handle connection button click
    connect(m_connectButton, &QPushButton::clicked, this, &QCrioViewWidget::handleConnection);
    //Start/stop server switch button state change
    connect(m_startStopServerSwitchButton, &QBetterSwitchButton::stateChanged, this, &QCrioViewWidget::onServerChangeState);

    connect(m_currentTestWidget->getModulesComboBox(),
            &QComboBox::currentIndexChanged,
            this,
            [this](){
                        this->onFillChannelsWithModules(m_currentTestWidget->getModulesComboBox(),
                                                        m_currentTestWidget->getChannelComboBox(),
                                                        m_currentTestWidget->getChannelComboBox()->currentIndex());
                    });

    connect(m_voltageTestWidget->getModulesComboBox(),
            &QComboBox::currentIndexChanged,
            this,
            [this](){
                        this->onFillChannelsWithModules(m_voltageTestWidget->getModulesComboBox(),
                                                        m_voltageTestWidget->getChannelComboBox(),
                                                        m_voltageTestWidget->getChannelComboBox()->currentIndex());
                    });

}


void QCrioViewWidget::handleConnection()
{
    emit sshConnectionAskedSignal();
}


void QCrioViewWidget::onServerChangeState()
{

    bool On = m_startStopServerSwitchButton->getState();
    if (On)
    {
        QWaitWindow *waitWindow = new QWaitWindow(10000, this);
        waitWindow->setModal(true); // If you want to block interaction with other windows
        waitWindow->setAttribute(Qt::WA_DeleteOnClose); // Ensure the window is deleted automatically when closed
        waitWindow->exec();
    }
    else
    {
       //disconnect all tcp clients
       m_currentTestWidget->onMustDisconnect();
       m_voltageTestWidget->onMustDisconnect();
    }
    emit serverChangeStateSignal(m_startStopServerSwitchButton->getState());

}

