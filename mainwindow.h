#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QGridLayout>
#include <QLabel>
#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QStringListModel>
#include <QRegularExpression>
#include <QListView>
#include <QDir>
#include <QProgressBar>
#include <QMessageBox>
#include <algorithm>
#include "./src/QIpAddressEditor.h"
#include "./src/QSSHCommand.h"
#include "./src/QIniTreeWidget.h"
#include "./src/QtTcpClient.h"
#include "./src/stringUtils.h"
#include "./src/QReadCurrentTestWidget.h"
#include "./src/QBetterSwitchButton.h"
#include "./src/QMultiLineTextVisualizer.h"
#include "./src/QCrioModulesDataExtractor.h"


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
    Ui::MainWindow            *ui              ;
    QVBoxLayout               *mainLayout      = nullptr;
    QTabWidget                *tabWidget       = nullptr;
    QLabel                    *ipLabel         = nullptr;
    QIpAddressEditor          *ipEdit          = nullptr;

    QCrioModulesDataExtractor *moduleExtractor = nullptr;

    QLabel    *loginLabel           = nullptr;
    QLineEdit *loginEdit            = nullptr;

    QLabel    *passwordLabel        = nullptr;
    QLineEdit *passwordEdit         = nullptr;
    QAction   *togglePasswordAction = nullptr;

    QPushButton *connectButton      = nullptr;
    QSSHCommand *sshCommand         = nullptr;
    QString     lastSshCommand      = "";
    bool        fromStartServer     = false;
    bool        fromStopServer      = false;

    QStringList      moduleList;
    QStringList      currentModulesPathList;
    QStringListModel *moduleListModel;
    QLabel           *moduleListLabel;
    QListView        *modulesListView;
    QProgressBar     *modulesLoadingProgressBar;
    int              currentModuleIndex;

    QReadCurrentTestWidget *currentTestWidget;
    QLabel                 *serverStateLabel;
    QBetterSwitchButton    *startStopServerSwitchButton;
    bool                   currentServerState = false;

    QMultiLineTextVisualizer *terminalOutput = nullptr;


    QIniTreeWidget  *iniTreeWidget;
    QtTcpClient     *tcpClient;


    QWidget *createCrioViewTab();
    QWidget *createModbusViewTab();
    QWidget *createGlobalParametersTab();
    QWidget *createDeviceParametersTab();
    QWidget *createMappingTableTab();
    void    createModuleList();



    const int sshPort = 22;
    QString   iniModulesLocalPath;
    void      handleConnection();
    void      downloadModulesDefinitions(int index);
    QString   retriveStringFromListViewIndex(int rowIndex);

private slots:
    //ssh commands signal
    void  onSSHError               (const QString &errorString,const QString &lastCommand);
    void  onLsCommandExecuted      (const QString &output,const QString &lastCommand);
    void  onModuleListRetrived     (const QString &output,const QString &lastCommand);
    void  onModuleIniFileDownloaded(const QString &output,const QString &lastCommand);
    void  onServerGetState         (const bool &isRunning,const QString &lastCommand);
    void  onServerStarted          (const QString &lastCommand);
    void  onServerStoped           (const QString &lastCommand);
    void  onServerStartSuccesfull  (const int &screenSession  , const QString &lastCommand);



    void  onModuleItemDoubleClicked(const QModelIndex &index);
    void  onServerChangeState      ();

};
#endif // MAINWINDOW_H
