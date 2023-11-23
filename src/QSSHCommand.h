#ifndef QSSHCOMMAND_H
#define QSSHCOMMAND_H

#include <QObject>
#include <QProcess>
#include <QCoreApplication>
#include "stringUtils.h"

class QSSHCommand : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sshClient READ sshClient WRITE setSshClient NOTIFY sshClientChanged)
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)
    Q_PROPERTY(int portNum READ portNum WRITE setPortNum NOTIFY portNumChanged)
    Q_PROPERTY(QString keyFile READ keyFile WRITE setKeyFile NOTIFY keyFileChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
public:
    QSSHCommand(QObject *parent = nullptr);


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

    void getModulesDefinitions();
    void downloadModulesDefinitions(QString params);
    void isServerRunning();
    void listFolder(QString path);
    void startServer();
    void stopServer ();


signals:
    void commandExecutedSignal       (const QString &output     , const QString &lastCommand);
    void errorOccurredSignal         (const QString &errorString, const QString &lastCommand);
    void listFileDoneSignal          (const QString &output     , const QString &lastCommand);
    void moduleListRetrievedSignal   (const QString &output     , const QString &lastCommand);
    void moduleDownloadedSignal      (const QString &output     , const QString &lastCommand);
    void serverStateSignal           (const bool &isRunning     , const QString &lastCommand);
    void serverStartedSignal         (const QString &lastCommand);
    void serverStartSuccesfullSignal (const int &screenSession  , const QString &lastCommand);
    void serverStopedSignal          (const QString &lastCommand);

    void sshClientChanged();
    void hostNameChanged();
    void portNumChanged();
    void keyFileChanged();
    void passwordChanged();

    void userNameChanged();

private:
    QString constructSSHCommand(const QString &command, const QString &parameter) const;
    void executeProcess(const QString &command);

    QString m_sshClient;
    QString m_hostName;
    int     m_portNum = 22;
    QString m_userName;
    QString m_password;
    QString m_keyFile;
    QProcess m_process;
    QString lastCommand;

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);


};

#endif // QSSHCOMMAND_H
