#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    iniModulesLocalPath = QCoreApplication::applicationDirPath()+"/"+"modules"+"/";
    iniModbusSetupPath  = QCoreApplication::applicationDirPath()+"/"+"modbus"+"/";
    moduleExtractor         = new QCrioModulesDataExtractor(this);

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

//This is the first slot triggered when we click on the connect button
void MainWindow::handleConnection()
{
    //1) we check if the object already exists, if yes...
    if (sshCommand != nullptr)
    {
        //destroy it
        delete(sshCommand);
        sshCommand = nullptr;
    }

    //Then we can create it and connect all the slots
    setupSSHModule();
    crioViewTab->connectButton()->setEnabled(false);
    sshCommand->isServerRunning();
}

void MainWindow::setupSSHModule()
{
    sshCommand = new QSSHCommand(this);
    // Set the SSH connection details from the user interface
    sshCommand->setHostName(crioViewTab->ipEdit()->ipAddress());
    sshCommand->setUserName(crioViewTab->loginEdit()->text());
    sshCommand->setPassword(crioViewTab->passwordEdit()->text());
    //sshCommand->setPortNum(22); // Optional if using default SSH port

                        //sent when a "cd /whatever/folder; ls" request
                        //has completed
    connect(sshCommand, &QSSHCommand::listFileDoneSignal           , this , &MainWindow::onLsCommandExecuted       ,Qt::QueuedConnection);

                        //sent when list the ini files
                        //for devices modules has completed
    connect(sshCommand, &QSSHCommand::moduleListRetrievedSignal    , this , &MainWindow::onModuleListRetrived      ,Qt::QueuedConnection);

                        //sent when modules ini files are
                        //effectiveley downloaded
    connect(sshCommand, &QSSHCommand::moduleDownloadedSignal       , this , &MainWindow::onModuleIniFileDownloaded ,Qt::QueuedConnection);

                        //sent when modbus.ini is
                        //effectively downloaded
    connect(sshCommand, &QSSHCommand::modbusSetupDownloadedSignal  , this, &MainWindow::onModubusParamFileDownloaded, Qt::QueuedConnection);

                        //Guess this one...
    connect(sshCommand, &QSSHCommand::errorOccurredSignal          , this , &MainWindow::onSSHError                ,Qt::QueuedConnection);

                        //sent when a request for server state
                        //(up or down) has a completed
    connect(sshCommand, &QSSHCommand::serverStateSignal            , this , &MainWindow::onServerGetState          ,Qt::QueuedConnection);

                        //sent when the server starting sequence begin
    connect(sshCommand, &QSSHCommand::serverStartedSignal          , this , &MainWindow::onServerStarted           ,Qt::QueuedConnection);

                        //sent when the server starting squence
                        //is succesfully finished
    connect(sshCommand, &QSSHCommand::serverStartSuccesfullSignal  , this , &MainWindow::onServerStartSuccesfull   ,Qt::QueuedConnection);

                        //sent whan the server has been stoped
    connect(sshCommand, &QSSHCommand::serverStopedSignal           , this , &MainWindow::onServerStoped            ,Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QWidget *MainWindow::createCrioViewTab()
{
    // This will create an instance of the QCrioViewWidget
    crioViewTab = new QCrioViewWidget(this);
    // Any additional configurations or connections specific to MainWindow can be done here
    setupCurrentReader();
    setupVoltageReader();
    //this to show the inifile in a specialized widget if we double click an item
    connect(crioViewTab->modulesListView(), &QListView::doubleClicked, this, &MainWindow::onModuleItemDoubleClicked, Qt::QueuedConnection);
    connect (crioViewTab, &QCrioViewWidget::sshConnectionAskedSignal,this,&MainWindow::handleConnection,Qt::QueuedConnection);
    return crioViewTab;

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


void MainWindow::setupCurrentReader()
{

    crioViewTab->currentTestWidget()->setEnabled(false);
    connect (crioViewTab->currentTestWidget(), &QReadCurrentTestWidget::logLastRequest, this ,&MainWindow::onCommandServerLogRequest, Qt::QueuedConnection);
    connect (crioViewTab->currentTestWidget(), &QReadCurrentTestWidget::logLastResponse,this ,&MainWindow::onCommandServerLogResponse,Qt::QueuedConnection);
    connect (crioViewTab->currentTestWidget(), &QReadCurrentTestWidget::logLastError,   this, &MainWindow::onCommanServerLogError,    Qt::QueuedConnection);
}



void MainWindow::setupVoltageReader()
{
    crioViewTab->voltageTestWidget()->setEnabled(false);
    connect (crioViewTab->voltageTestWidget(), &QReadVoltageTestWidget::logLastRequest,  this, &MainWindow::onCommandServerLogRequest,  Qt::QueuedConnection);
    connect (crioViewTab->voltageTestWidget(), &QReadVoltageTestWidget::logLastResponse, this, &MainWindow::onCommandServerLogResponse, Qt::QueuedConnection);
    connect (crioViewTab->voltageTestWidget(), &QReadVoltageTestWidget::logLastError,    this, &MainWindow::onCommanServerLogError,     Qt::QueuedConnection);
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
    QAbstractItemModel* model = crioViewTab->modulesListView()->model();
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
    crioViewTab->terminalOutput()->addLastCommand(lastCommand);
    crioViewTab->terminalOutput()->addLastOutput("fileList:\n"+output);
}

void MainWindow::onModuleListRetrived(const QString &output, const QString &lastCommand)
{
    qInfo()<<"enter onModuleListRetrived";
    lastSshCommand = lastCommand;
    crioViewTab->terminalOutput()->addLastCommand(lastCommand);
    moduleList = output.split("\n", Qt::SkipEmptyParts);
    //reset the progress bar min/max/value
    crioViewTab->onResetModuleLoadProgressBar(0,moduleList.count()-1,0);
    // Sort the list based on the number in the filename
    sortQStringListBySuffix(moduleList,"_");
    //update the interface withe the module list
    crioViewTab->onUpdateModelList(moduleList);
    //reset lists and trackig index
    currentModuleIndex = 0;
    currentModulesPathList.clear();
    voltageModulesPathList.clear();
    //sort each module by type (analogic current, voltage and so on)
    //and fill dedicated lists for easier search later
    for (int i=0;i<moduleList.count();++i)
    {
        if  (moduleList[i].contains("NI9208"))
        {
            currentModulesPathList.append(iniModulesLocalPath+moduleList[i]);
        }
        else if (moduleList[i].contains("NI9239"))
        {
           voltageModulesPathList.append(iniModulesLocalPath+moduleList[i]);
        }
    }
    //then we can download all the ini files
    downloadModulesDefinitions(currentModuleIndex);
}



void MainWindow::onModuleIniFileDownloaded(const QString &output, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    crioViewTab->terminalOutput()->addLastCommand(lastCommand);
    crioViewTab->terminalOutput()->addLastOutput(output);
    if (output.contains("100%"))
    {
        crioViewTab->modulesLoadingProgressBar()->setValue(crioViewTab->modulesLoadingProgressBar()->value() + 1);
        crioViewTab->modulesLoadingProgressBar()->update();

        if (currentModuleIndex < moduleList.count() - 1)
        {
            // Not yet downloaded all modules, continue downloading
            currentModuleIndex++;
            downloadModulesDefinitions(currentModuleIndex);
        }
        else
        {
            // All modules are downloaded, let's update the user interface
            crioViewTab->onUpdateControlsAfterModulesDownloaded(moduleExtractor,currentModulesPathList,voltageModulesPathList,commandPort);
            sshCommand->downloadModbusSetup(iniModbusSetupPath);
        }
    }
}

void MainWindow::onModubusParamFileDownloaded(const QString &output, const QString &lastCommand)
{
    lastSshCommand = lastCommand;
    crioViewTab->terminalOutput()->addLastCommand(lastCommand);
    crioViewTab->terminalOutput()->addLastOutput(output);
    modbusSetupViewer->setFileName(iniModbusSetupPath+"modbus.ini");
    modbusSetupViewer->loadFromFile();

}


void MainWindow::onModuleItemDoubleClicked(const QModelIndex &index)
{
  if (index.isValid())
  {
          QString fileName = iniModulesLocalPath + crioViewTab->moduleListModel()->data(index, Qt::DisplayRole).toString();
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
   //enable or disable some controls in reaction to the server state
   crioViewTab->onUpdateControlsInReactionToServerState(lastCommand,isRunning);
   //make the user unable to stop the server until the modules are effectively downloaded
   crioViewTab->startStopServerSwitchButton()->setEnabled(false);
   if (fromStartServer && isRunning)
   {
       fromStartServer = false;
      //TODO
   }
   else if (fromStopServer && isRunning)
   {
       //if stop server was asked and the state is still running then the stop script failed
       crioViewTab->onUpdateControlsInReactionOfaStopFailure();
       //so the next time we are here, it's not to verify the stoped state
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
    crioViewTab->terminalOutput()->addLastCommand (lastCommand);
    crioViewTab->terminalOutput()->addLastOutput("server starting");
    sshCommand->isServerRunning();
}

void MainWindow::onServerStoped(const QString &lastCommand)
{
    lastSshCommand =  lastCommand;
    crioViewTab->terminalOutput()->addLastOutput("server stoped");
    crioViewTab->currentTestWidget()->setEnabled(false);
    crioViewTab->voltageTestWidget()->setEnabled(false);

}

void MainWindow::onServerStartSuccesfull(const int &screenSession, const QString &lastCommand)
{
    // the server has started succesfully
    //keep track of the last command
    lastSshCommand =  lastCommand;
    //this will update the user interface
    crioViewTab->onServerStartSuccesfull(screenSession,lastCommand);
    //clear the module list
    moduleList.clear();
    //and get the module list, when the module list is executed sshCommand will trigger onModuleListRetrived signal with the output
    sshCommand->getModulesDefinitions();
    fromStartServer = false;
    fromStopServer  = false;
}

void MainWindow::onCommandServerLogRequest(const QString &request)
{
    crioViewTab->terminalOutput()->addLastCommand(request);
}

void MainWindow::onCommandServerLogResponse(const QString &response)
{
    crioViewTab->terminalOutput()->addLastOutput(response);
}

void MainWindow::onCommanServerLogError(const QString &error)
{
    crioViewTab->terminalOutput()->addLastError(error);
}

void MainWindow::onServerChangeState()
{

    if (crioViewTab->startStopServerSwitchButton()->getState())
    {
        sshCommand->startServer();
    }
    else
    {
        sshCommand->stopServer();
    }
}




