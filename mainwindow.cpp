#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    iniModulesLocalPath = QCoreApplication::applicationDirPath()+"/"+"modules"+"/";
    iniModbusSetupPath  = QCoreApplication::applicationDirPath()+"/"+"modbus"+"/";
    moduleExtractor     = new QCrioModulesDataExtractor(this);
    setupTCPClient();
    ui->setupUi(this);
    tabWidget = new QTabWidget(this);

    // Add tabs
    tabWidget->addTab(createCrioViewTab(), tr("CrioView"));
    tabWidget->addTab(createModbusViewTab(), tr("ModbusView"));
    tabWidget->addTab(createGlobalParametersTab(), tr("GlobalParameters"));
    tabWidget->addTab(createDeviceParametersTab(), tr("DeviceParameters"));
    tabWidget->addTab(createMappingTableTab(), tr("MappingTable"));
    setCentralWidget(tabWidget);

}

MainWindow::~MainWindow()
{
    delete ui;
}


QWidget *MainWindow::createCrioViewTab()
{
    QWidget *tab = new QWidget();
    createModuleList();
    setupCurrentReader();
    setupVoltageReader();
    QGridLayout *layout = new QGridLayout(tab);

    ipLabel = new QLabel(this);
    ipLabel->setText("Crio Ip");
    ipEdit  = new QIpAddressEditor(this);

    passwordLabel = new QLabel(this);
    passwordLabel->setText("password");
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password);

    // Create the show/hide password toggle action
    togglePasswordAction = passwordEdit->addAction(QIcon(":/images/cacher.png"), QLineEdit::TrailingPosition);
    togglePasswordAction->setCheckable(true);

    // Connect the action's toggled signal to a lambda that updates the echo mode
    connect(togglePasswordAction, &QAction::toggled, this, [this](bool checked)
    {
        togglePasswordAction->setIcon(QIcon(checked ? ":/images/oeil.png" : ":/images/cacher.png"));
        passwordEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });

    loginLabel= new QLabel   (this);
    loginLabel->setText("login");
    loginEdit = new QLineEdit(this);

    // Set up the connection button
    connectButton = new QPushButton(tr("Connect"), this);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::handleConnection);

    serverStateLabel = new QLabel(tr("server status"),this);
    startStopServerSwitchButton = new QBetterSwitchButton(" stoped ",
                                                          " active ",
                                                          QColor(0xf54444),
                                                          QColor(0x449ef5),
                                                          QColor(0x44f547),
                                                          false);
    startStopServerSwitchButton->setEnabled(false);
    connect (startStopServerSwitchButton, &QBetterSwitchButton::stateChanged,this, &MainWindow::onServerChangeState, Qt::QueuedConnection);

    truthOScope = new QOScope(this);

    terminalOutput = new QMultiLineTextVisualizer(this);

    layout->addWidget(ipLabel          , 0,0,1,1,Qt::AlignHCenter);
    layout->addWidget(loginLabel       , 0,1,1,1,Qt::AlignHCenter);
    layout->addWidget(passwordLabel    , 0,2,1,1,Qt::AlignHCenter);
    layout->addWidget(serverStateLabel , 0,4,1,1, Qt::AlignHCenter);

    layout->addWidget(ipEdit         , 1,0,1,1);
    layout->addWidget(loginEdit      , 1,1,1,1);
    layout->addWidget(passwordEdit   , 1,2,1,1);
    layout->addWidget(connectButton  , 1,3,1,1);
    layout->addWidget(startStopServerSwitchButton , 1,4,1,1, Qt::AlignHCenter);

    layout->addWidget(moduleListLabel , 2,0,1,1,Qt::AlignHCenter);
    layout->addWidget(modulesListView , 3,0,1,1);
    layout->addWidget(modulesLoadingProgressBar , 4,0,1,1);



    layout->addWidget(currentTestWidget           , 3,1,1,1);
    layout->addWidget(voltageTestWidget           , 3,2,1,1);
    layout->addWidget(truthOScope                 , 3,3,1,2);

    layout->addWidget(terminalOutput              , 5,0,1,5);

    currentTestWidget->setTruthOScope(truthOScope);
    // Add widgets to layout as needed
    return tab;
}

QWidget *MainWindow::createModbusViewTab()
{
    modbusSetupViewer = new QModbusSetupViewer(this);
    modbusSetupViewer->setFileName(iniModbusSetupPath+"modbus.ini");

    QWidget *tab = new QWidget();
    QGridLayout *layout = new QGridLayout(tab);
    layout->addWidget(modbusSetupViewer,0,0,1,1);
    return tab;
}

QWidget *MainWindow::createGlobalParametersTab()
{
    QWidget *tab = new QWidget();
    QGridLayout *layout = new QGridLayout(tab);
    // Add widgets to layout as needed
    return tab;
}

QWidget *MainWindow::createDeviceParametersTab()
{
    QWidget *tab = new QWidget();
    QGridLayout *layout = new QGridLayout(tab);
    iniTreeWidget = new QIniTreeWidget(this);
    layout->addWidget(iniTreeWidget, 0,0,1,1,Qt::AlignCenter);
    // Add widgets to layout as needed
    return tab;
}

QWidget *MainWindow::createMappingTableTab()
{
    QWidget *tab = new QWidget();
    QGridLayout *layout = new QGridLayout(tab);

    // Add widgets to layout as needed
    return tab;
}

void MainWindow::createModuleList()
{
    moduleListModel = new QStringListModel(this);
    moduleListLabel = new QLabel(this);
    moduleListLabel->setText("modules list:");
    modulesListView           = new QListView(this);
    modulesListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(modulesListView, &QListView::doubleClicked, this, &MainWindow::onModuleItemDoubleClicked, Qt::QueuedConnection);
    modulesLoadingProgressBar = new QProgressBar(this);
}



void MainWindow::handleConnection()
{
    qDebug()<<"***** Etape 1 *******";
    if (sshCommand != nullptr)
    {

        delete(sshCommand);
        sshCommand = nullptr;
    }
    setupSSHModule();
    connectButton->setEnabled(false);
    sshCommand->isServerRunning();
}

void MainWindow::setupSSHModule()
{
    sshCommand = new QSSHCommand(this);
    // Set the SSH connection details
    sshCommand->setHostName(ipEdit->ipAddress());
    sshCommand->setUserName(loginEdit->text());
    sshCommand->setPassword(passwordEdit->text());
    //sshCommand->setPortNum(22); // Optional if using default SSH port
    connect(sshCommand, &QSSHCommand::listFileDoneSignal           , this , &MainWindow::onLsCommandExecuted       ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::moduleListRetrievedSignal    , this , &MainWindow::onModuleListRetrived      ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::moduleDownloadedSignal       , this , &MainWindow::onModuleIniFileDownloaded ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::errorOccurredSignal          , this , &MainWindow::onSSHError                ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::serverStateSignal            , this , &MainWindow::onServerGetState          ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::serverStartedSignal          , this , &MainWindow::onServerStarted           ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::serverStartSuccesfullSignal  , this , &MainWindow::onServerStartSuccesfull   ,Qt::QueuedConnection);
    connect(sshCommand, &QSSHCommand::serverStopedSignal           , this , &MainWindow::onServerStoped            ,Qt::QueuedConnection);
}

void MainWindow::setupTCPClient()
{
    tcpClient  = new QtTcpClient(this);
}

void MainWindow::setupCurrentReader()
{
    currentTestWidget = new QReadCurrentTestWidget(tcpClient,this);
    currentTestWidget->setEnabled(false);
    connect (currentTestWidget, &QReadCurrentTestWidget::logLastRequest,this,&MainWindow::onCommandServerLogRequest,Qt::QueuedConnection);
    connect (currentTestWidget, &QReadCurrentTestWidget::logLastResponse,this,&MainWindow::onCommandServerLogResponse,Qt::QueuedConnection);
}

void MainWindow::setupVoltageReader()
{
    voltageTestWidget = new QReadVoltageTestWidget(tcpClient,this);
    voltageTestWidget->setEnabled(false);
    connect (voltageTestWidget, &QReadVoltageTestWidget::logLastRequest, this, &MainWindow::onCommandServerLogRequest,Qt::QueuedConnection);
    connect (voltageTestWidget, &QReadVoltageTestWidget::logLastResponse, this, &MainWindow::onCommandServerLogResponse,Qt::QueuedConnection);
}


//************ CRIO *******************


void MainWindow::downloadModulesDefinitions(int index)
{
    QString fileName = retriveStringFromListViewIndex(index);
    QString params = "/home/dataDrill/"+fileName+" "+iniModulesLocalPath+fileName;
    qDebug()<<params;
    sshCommand->downloadModulesDefinitions(params);

}

QString MainWindow::retriveStringFromListViewIndex(int rowIndex)
{
    // Get the model associated with the ListView
    QAbstractItemModel* model = modulesListView->model();
    // Check if the model is valid
    if (model)
    {
        // Define the index to retrieve
        QModelIndex index = model->index(rowIndex, 0); // Replace rowIndex with the desired index
        // Check if the index is valid
        if (index.isValid()) {
            // Retrieve the text of the item at the specified index
            QVariant itemData = model->data(index, Qt::DisplayRole);

            // Check if the itemData is valid and contains text
            if (itemData.isValid() && itemData.canConvert<QString>())
            {
                QString text = itemData.toString();
                // Now 'text' contains the text of the item at the specified index
                qDebug() << "Text at index" << rowIndex << ":" << text;
                return text;
            }
            else
            {
                qDebug() << "Invalid item data or not a string.";
            }
        }
        else
        {
            qDebug() << "Invalid index.";
            return "";
        }
    }
    else
    {
        qDebug() << "Invalid model.";
        return "";
    }
  return "";
}

void MainWindow::onSSHError(const QString &errorString, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    qDebug().noquote() << "Error:" << errorString;
    qDebug()<<"last command:" << lastCommand;
}

void MainWindow::onLsCommandExecuted(const QString &output, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    terminalOutput->addLastCommand(lastCommand);
    terminalOutput->addLastOutput("fileList:\n"+output);
}

void MainWindow::onModuleListRetrived(const QString &output, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    terminalOutput->addLastCommand(lastCommand);
    moduleList = output.split("\n", Qt::SkipEmptyParts);
    modulesLoadingProgressBar->setMaximum(moduleList.count()-1);
    modulesLoadingProgressBar->setMinimum(0);
    modulesLoadingProgressBar->setValue(0);
    // Sort the list based on the number in the filename
    sortQStringListBySuffix(moduleList,"_");
    moduleListModel->setStringList(moduleList);
    modulesListView->setModel(moduleListModel);
    currentModuleIndex = 0;
    terminalOutput->addLastOutput("succes");

    currentModulesPathList.clear();
    voltageModulesPathList.clear();
    for (int i=0;i<moduleList.count();++i)
    {
        if  (moduleList[i].contains("NI9208"))
        {
            currentModulesPathList.append(iniModulesLocalPath+moduleList[i]);
            qInfo()<<iniModulesLocalPath<<moduleList[i]<<" added to currentModulesPathList";
        }
        else if (moduleList[i].contains("NI9239"))
        {
           voltageModulesPathList.append(iniModulesLocalPath+moduleList[i]);
           qInfo()<<iniModulesLocalPath<<moduleList[i]<<" added to voltageModulesPathList";
        }
    }
    downloadModulesDefinitions(currentModuleIndex);
}



void MainWindow::onModuleIniFileDownloaded(const QString &output, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    terminalOutput->addLastCommand(lastCommand);
    terminalOutput->addLastOutput(output);
    if (output.contains("100%"))
    {
        modulesLoadingProgressBar->setValue(modulesLoadingProgressBar->value() + 1);
        modulesLoadingProgressBar->update();

        if (currentModuleIndex < moduleList.count() - 1)
        {
            // Not yet downloaded all modules, continue downloading
            currentModuleIndex++;
            downloadModulesDefinitions(currentModuleIndex);
        }
        else
        {
            // All modules are downloaded
            startStopServerSwitchButton->setEnabled(true);
            startStopServerSwitchButton->update();
            if (startStopServerSwitchButton->getState())
            {
                moduleExtractor->extractCurrentModules(currentModulesPathList,
                                                       currentTestWidget->getModulesComboBox(),
                                                       currentTestWidget->getChannelComboBox());

                moduleExtractor->extractVoltageModules(voltageModulesPathList,
                                                       voltageTestWidget->getModulesComboBox(),
                                                       voltageTestWidget->getChannelComboBox());


                tcpClient->connectToServer(ipEdit->ipAddress(),commandPort);
            }


        }
    }
}


void MainWindow::onModuleItemDoubleClicked(const QModelIndex &index)
{
  if (index.isValid())
  {
          QString fileName = iniModulesLocalPath + moduleListModel->data(index, Qt::DisplayRole).toString();
          qInfo()<<fileName;
          iniTreeWidget->loadFromFile(fileName);

          // Switch to the DeviceParameters tab
          for (int i = 0; i < tabWidget->count(); ++i)
          {
              if (tabWidget->tabText(i) == "DeviceParameters")
              {
                  tabWidget->setCurrentIndex(i);
                  break;
              }
          }
  }
}

void MainWindow::onServerGetState(const bool &isRunning, const QString &lastCommand)
{
   lastSshCommand = lastCommand;
   terminalOutput->addLastCommand(lastCommand);
   currentTestWidget->setEnabled(isRunning);
   voltageTestWidget->setEnabled(isRunning);
   if (startStopServerSwitchButton->getState()!=isRunning)
   {
       startStopServerSwitchButton->setEnabled(true);
       startStopServerSwitchButton->setState(isRunning);
       startStopServerSwitchButton->update();
       startStopServerSwitchButton->setEnabled(false);

   }
   startStopServerSwitchButton->setEnabled(false);
   if (fromStartServer && isRunning)
   {
      qDebug()<<"Server started";
      fromStartServer = false;
      //TODO
   }
   else if (fromStopServer && isRunning)
   {
       terminalOutput->addLastError("failed to stop server");
       currentTestWidget->setEnabled(false);
       voltageTestWidget->setEnabled(false);
       fromStopServer = false;
   }
   else
   {
     sshCommand->getModulesDefinitions();
   }
}

void MainWindow::onServerStarted(const QString &lastCommand)
{
    lastSshCommand =  lastCommand;
    fromStartServer = true;
    terminalOutput->addLastCommand (lastCommand);
    terminalOutput->addLastOutput("server starting");
    sshCommand->isServerRunning();
}

void MainWindow::onServerStoped(const QString &lastCommand)
{
    lastSshCommand =  lastCommand;
    terminalOutput->addLastOutput("server stoped");
    currentTestWidget->setEnabled(false);
    voltageTestWidget->setEnabled(false);

}

void MainWindow::onServerStartSuccesfull(const int &screenSession, const QString &lastCommand)
{
    lastSshCommand =  lastCommand;
    terminalOutput->addLastCommand (lastCommand);
    terminalOutput->addLastOutput("Server started successfully");
    QMessageBox::information(nullptr, "Server Status", "Server started successfully", QMessageBox::Ok);
    startStopServerSwitchButton->setEnabled(true);
    startStopServerSwitchButton->blockSignals(true);
    startStopServerSwitchButton->setState(true);
    startStopServerSwitchButton->blockSignals(false);
    startStopServerSwitchButton->update();
    moduleList.clear();
    sshCommand->getModulesDefinitions();
    fromStartServer = false;
    fromStopServer  = false;
    currentTestWidget->setEnabled(true);
    voltageTestWidget->setEnabled(true);
    serverStateLabel->setText("server status:\n running on\nsession"+QString::number(screenSession));
}

void MainWindow::onCommandServerLogRequest(const QString &request)
{
    terminalOutput->addLastCommand(request);
}

void MainWindow::onCommandServerLogResponse(const QString &response)
{
    terminalOutput->addLastOutput(response);
}

void MainWindow::onServerChangeState()
{

    if (startStopServerSwitchButton->getState())
    {
        sshCommand->startServer();
    }
    else
    {
        sshCommand->stopServer();
    }
}



