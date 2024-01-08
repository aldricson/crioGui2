#ifndef QSSHCOMMAND_H
#define QSSHCOMMAND_H

#include <QObject>
#include <QProcess>
#include <QCoreApplication>
#include <QMutex>
#include <QMutexLocker>
#include "stringUtils.h"

class QSSHCommand : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString sshClient   READ sshClient      WRITE setSshClient   NOTIFY sshClientChanged   )
    Q_PROPERTY(QString hostName    READ hostName       WRITE setHostName    NOTIFY hostNameChanged    )
    Q_PROPERTY(int     portNum     READ portNum        WRITE setPortNum     NOTIFY portNumChanged     )
    Q_PROPERTY(QString keyFile     READ keyFile        WRITE setKeyFile     NOTIFY keyFileChanged     )
    Q_PROPERTY(QString password    READ password       WRITE setPassword    NOTIFY passwordChanged    )
    Q_PROPERTY(QString userName    READ userName       WRITE setUserName    NOTIFY userNameChanged    )
    Q_PROPERTY(bool    withSsh2    READ getWithLibSSH2 WRITE setWithLibSSH2 NOTIFY withLibSSH2Changed )
    Q_PROPERTY(QString lastCommand READ getLastCommand WRITE setLastCommand NOTIFY lastCommandChanged )

public:
    QSSHCommand(const QString &md5Hash, const QString &executionPath, QObject *parent = nullptr);


    void sendCommand(const QString &command, const QString &parameter = QString());

    const QString &sshClient() const;
    void setSshClient(const QString &newSshClient);

    const QString &hostName() const;
    void setHostName(const QString &newHostName);

    int portNum() const;
    void setPortNum(int newPortNum);

    const QString &keyFile() const;
    void setKeyFile(const QString &newKeyFile);

    const QString &password() const;
    void setPassword(const QString &newPassword);

    const QString &userName() const;
    void setUserName(const QString &newUserName);

    bool getWithLibSSH2() const;
    void setWithLibSSH2(bool newWithLibSSH2);

    void getModulesDefinitions      ()                           ;
    void downloadModulesDefinitions (QString params)             ;
    void downloadModbusSetup        (QString iniModbusSetupPath) ;
    void downloadMappingSetup       (QString modbusMappingPath)  ;
    void uploadMappingSetup         (QString modbusMappingPath)  ;
    void isServerRunning            ()                           ;
    void listFolder                 (QString path)               ;
    void startServer                ()                           ;
    void stopServer                 ()                           ;
    void getTotalCPU                ()                           ;
    void getDataDrillCPU            ()                           ;
    void getCrioHDDUsage            ()                           ;
    void getDataDrillHDDUsage       ()                           ;
    void getCrioTotalRamUsage       ()                           ;
    void getDataDrillRamUsage       ()                           ;
    void getCrioGlobalStats         ()                           ;
    bool checkIntegrity             ()                           ;
    const QString &getLastCommand() const;
    void setLastCommand(const QString &newLastCommand);

signals:
    void commandExecutedSignal       ( const QString &output        , const QString &lastCommand );
    void errorOccurredSignal         ( const QString &errorString   , const QString &lastCommand );
    void listFileDoneSignal          ( const QString &output        , const QString &lastCommand );
    void moduleListRetrievedSignal   ( const QString &output        , const QString &lastCommand );
    void moduleDownloadedSignal      ( const QString &output        , const QString &lastCommand );
    void modbusSetupDownloadedSignal ( const QString &output        , const QString &lastCommand );
    void modbusMappingLoadedSignal   ( const QString &output        , const QString &lastCommand );
    void modbusMappingUploadedSignal ( const QString &output        , const QString &lastCommand );
    void totalCPUSignal              ( const QString &output        , const QString &lastCommand );
    void dataDrillCpuSignal          ( const QString &output        , const QString &lastCommand );
    void dataDrillTotalHDDSignal     ( const QString &output        , const QString &lastCommand );
    void dataDrillHDDSignal          ( const QString &output        , const QString &lastCommand );
    void dataDrillTotalRamSignal     ( const QString &output        , const QString &lastCommand );
    void dataDrillRamUsageSignal     ( const QString &output        , const QString &lastCommand );
    void globalStatSignal            ( const QString &output        , const QString &lastCommand );
    void serverStateSignal           ( const bool    &isRunning     , const QString &lastCommand );
    void serverStartedSignal         ( const QString &lastCommand                                );
    void serverStartSuccesfullSignal ( const int     &screenSession , const QString &lastCommand );
    void serverStopedSignal          ( const QString &lastCommand);
    void authenticationFailedSignal  ( const QString &lastCommand);

    void sshClientChanged () ;
    void hostNameChanged  () ;
    void portNumChanged   () ;
    void keyFileChanged   () ;
    void passwordChanged  () ;

    void userNameChanged();
    void withLibSSH2Changed();

    void lastCommandChanged(const QString &lastCommand);

private:

    static QMutex mutex; // Static mutex shared by all instances
    QString constructSSHCommand(const QString &command, const QString &parameter) const;
    void executeProcess(const QString &command);

    QString  m_sshClient     = ""    ;
    QString  m_hostName      = ""    ;
    QString  m_hashMd5       = ""    ;
    QString  m_executionPath = ""    ;

    QString  m_userName      = ""    ;
    QString  m_password      = ""    ;
    QString  m_keyFile       = ""    ;
    int      m_portNum       = 22    ;
    QProcess m_process               ;
    QString  lastCommand     = ""    ;
    bool     m_withLibSSH2   = false ;

    QString encryptData(const QString& dataStr, const QString& keyStr);
    QString decryptData(const QString& encryptedDataStr, const QString& keyStr);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);


};

#endif // QSSHCOMMAND_H
