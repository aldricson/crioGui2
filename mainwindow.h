#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QAction>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QStringListModel>
#include <QRegularExpression>

#include <QDir>

#include <algorithm>
#include "./src/NetWorking/QSSHCommand.h"
#include "./src/NetWorking/QtTcpClient.h"
#include "./src/BasicWidgets/QBetterSwitchButton.h"
#include "./src/BasicWidgets/QMultiLineTextVisualizer.h"
#include "./src/stringUtils.h"
#include "./src/Extractors/QCrioModulesDataExtractor.h"
#include "./src/TabWidgets/QModbusSetupViewer.h"
#include "./src/TabWidgets/QCrioViewWidget.h"
#include "./src/TabWidgets/QMappingViewerWidget.h"

class QProgressBar            ;
class QMessageBox             ;
class QPushButton             ;
class QLabel                  ;
class QTabWidget              ;
class QTCPDebugClient         ;
class QDeviceParametersWidget ;
class QGlobalParametersWidget ;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private:

    //visual placing objects
    Ui::MainWindow            *ui                            ;
    QVBoxLayout               *mainLayout           = nullptr;
    //this class is in charge to intercept crio udp debug datagrams
    QTCPDebugClient           *m_crioDebugger       = nullptr;
    QTabWidget                *tabWidget            = nullptr;
    //this class is in charge to extract modules information from ini files
    QCrioModulesDataExtractor *moduleExtractor      = nullptr;
    //this class handle all the basic widgets for the user to connect and test the crio
    QCrioViewWidget           *crioViewTab          = nullptr;
    //this class handle all the basic widgets to deal with modules hardware parameters
    QDeviceParametersWidget   *devicesTab           = nullptr;
    //this class handle all the basic widget to deal with Crio statistics
    QGlobalParametersWidget   *globalTab            = nullptr;
    //this class handle all the basic widgets to deal with the crio modbus layer
    QModbusSetupViewer        *modbusSetupViewer    = nullptr;
    //this class handle all the basic widgets to deal with the modbus mapping
    QMappingViewerWidget      *modbusMappingViewer  = nullptr;
    //this class handle all the ssh communication through putty tools and later sshlib2
    QSSHCommand *sshCommand         = nullptr;
    //to track the last command sent
    QString     lastSshCommand      = "";
    //Some flags
    bool        fromStartServer     = false;
    bool        fromStopServer      = false;
    bool        currentServerState  = false; //false when the dataDrill server is not running (down) / true if it's running (up)
    //to pass parameters to all objects we use
    void        setupSSHModule();
    void        setupCurrentReader();
    void        setupVoltageReader();
    //TO keep track of parameters about the loaded modules and file path
    QStringList      moduleList             ;
    QStringList      currentModulesPathList ;
    QStringList      voltageModulesPathList ;
    int              currentModuleIndex     ;



   // QIniTreeWidget  *iniTreeWidget;



    QWidget *createCrioViewTab         ();
    QWidget *createModbusViewTab       ();
    QWidget *createGlobalParametersTab ();
    QWidget *createDeviceParametersTab ();
    QWidget *createMappingTableTab     ();

    //default ssh port
    const int     sshPort     = 22  ;
    //default TCP/IP command port
    const quint16 commandPort = 8222;

    QString   iniModulesLocalPath = ""                     ;
    QString   iniModbusSetupPath  = ""                     ;
    QString   modbusMappingPath   = ""                     ;
    void      handleConnection               ()            ;
    void      downloadModulesDefinitions     (int index)   ;
    QString   retriveStringFromListViewIndex (int rowIndex);

private slots:
    //this slot is triggered when sshCommand fails
    void  onSSHError               (const QString &errorString , const QString &lastCommand);

    //this slot is triggered when sshCommand get the response to a "dir" query though this command in ssh. bat
    //plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd %PARAMETER1%; ls"
    void  onLsCommandExecuted      (const QString &output      , const QString &lastCommand);

    //this slots is triggered when sshCommand get the response to a "getModuleList" query though this command in ssh. bat
    //plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; ls | grep \"^NI.*\.ini$\""
    void  onModuleListRetrived     (const QString &output      , const QString &lastCommand);

    //this slot is triggered when sshCommand get the response to a "downloadModule" query though this command in ssh. bat
    //pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
    void  onModuleIniFileDownloaded(const QString &output      , const QString &lastCommand);

    //this slot is triggered when sshCommand get the response to a "downloadModbusSetup" query though this command in ssh. bat
    //pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
    void onModubusParamFileDownloaded(const QString &output    , const QString &lastCommand);

    //this slot is triggered when sshCommand get the response to a "downloadModbusSetup" query though this command in ssh. bat
    //pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
    void onModbusMappingFileDownloaded(const QString &output    , const QString &lastCommand);

    //this slot is triggered when sshCommand command get the response to a "downloadModule" module query though this command in ssh. bat
    //pscp -P %PORT% -pw %PASS% %USER%@%HOST%:%PARAMETER1% %PARAMETER2%
    void  onServerGetState         (const bool    &isRunning   , const QString &lastCommand);

    //triggered when the server start sequence is initiated with a "downloadModule" module query though this command in ssh. bat
    //plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; sh dataDrillStart.sh"
    void  onServerStarted          (const QString &lastCommand);

    //triggered when the server stop sequence is initiated with a "downloadModule" module query though this command in ssh. bat
    //plink -ssh %USER%@%HOST% -P %PORT% -pw %PASS% "cd /home/dataDrill; sh dataDrillStop.sh"
    void  onServerStoped           (const QString &lastCommand);

    //triggered when the start sequence is finished successfully
    void  onServerStartSuccesfull  (const int     &screenSession , const QString &lastCommand);

    void onAccessDenied  (const QString &lastCommand);

    void onCommandServerLogRequest  (const QString &request);
    void onCommandServerLogResponse (const QString &response);
    void onCommanServerLogError     (const QString &error);
    //triggered by the udp listener when the crio send a dbug udp packet
    void onCrioDebugMessage (const QString &message);



    void  onModuleListUpdated(const QModelIndex &index);
    void  onServerChangeState      (bool isOn);


};
#endif // MAINWINDOW_H
