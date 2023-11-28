#include "QSSHCommand.h"

QSSHCommand::QSSHCommand(QObject *parent) : QObject(parent), m_portNum(22)
{
    connect(&m_process, &QProcess::finished, this, &QSSHCommand::processFinished);
    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (error != QProcess::ProcessError::UnknownError) {
            emit errorOccurredSignal(m_process.errorString(),lastCommand);
        }
    });
}

void QSSHCommand::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QString output = m_process.readAllStandardOutput();
    if (output.isEmpty()) {
        output = m_process.readAllStandardError();
    }
    if (lastCommand == "dir")
    {
        emit listFileDoneSignal(output,lastCommand);
    }
    if (lastCommand == "getModuleList")
    {
        emit moduleListRetrievedSignal(output,lastCommand);
    }
    else if (lastCommand =="downloadModule")
    {

        emit moduleDownloadedSignal(output,lastCommand);
    }
    else if (lastCommand == "serverState")
    {
        if (output == "Program is not running in any screen session.\nProgram is not running.\n")
        {
           emit serverStateSignal(false,lastCommand);
        }
        else
        {
            int screenSession = extractNumberFromSessionString(output);
            emit serverStartSuccesfullSignal(screenSession,lastCommand);
        }
    }
    else if (lastCommand == "startServer")
    {
        emit serverStartedSignal(lastCommand);
    }
    else if (lastCommand == "stopServer")
    {
       emit serverStopedSignal(lastCommand);
    }
    else
    {
        emit commandExecutedSignal(output,lastCommand);
    }

    // Reset lastCommand for the next execution
    lastCommand.clear();
}

void QSSHCommand::sendCommand(const QString &command, const QString &parameter)
{
    lastCommand = command; // Store the command
    QString commandLine = constructSSHCommand(command, parameter);
    qDebug() << "Executing command:" << commandLine;
    executeProcess(commandLine);
}

QString QSSHCommand::constructSSHCommand(const QString &command, const QString &parameter) const
{
    // Construct the command to call the batch script with the necessary arguments
    QString scriptPath = QCoreApplication::applicationDirPath() + "/ssh.bat";
    QString commandLine = QString("%1 %2 %3 %4 %5 %6")
        .arg(scriptPath,
             m_hostName,
             m_userName,
             m_password,
             QString::number(m_portNum),
             command + " " + parameter);
    return commandLine;
}


void QSSHCommand::executeProcess(const QString &command)
{
    m_process.start("cmd", QStringList() << "/c" << command);
}

bool QSSHCommand::getWithLibSSH2() const
{
    return m_withLibSSH2;
}

void QSSHCommand::setWithLibSSH2(bool newWithLibSSH2)
{
    if (m_withLibSSH2 == newWithLibSSH2)
        return;
    m_withLibSSH2 = newWithLibSSH2;
    emit withLibSSH2Changed();
}

const QString &QSSHCommand::userName() const
{
    return m_userName;
}

void QSSHCommand::setUserName(const QString &newUserName)
{
    if (m_userName == newUserName)
        return;
    m_userName = newUserName;
    emit userNameChanged();
}

void QSSHCommand::getModulesDefinitions()
{
    if (!m_withLibSSH2)
    {
        sendCommand("getModuleList", "");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::downloadModulesDefinitions(QString params)
{
    if (!m_withLibSSH2)
    {
        sendCommand("downloadModule", params);
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::isServerRunning()
{
    sendCommand("serverState","");
}

void QSSHCommand::listFolder(QString path)
{
    if (!m_withLibSSH2)
    {
        sendCommand("dir",path);
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::startServer()
{
    if (!m_withLibSSH2)
    {
        sendCommand("startServer","");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::stopServer()
{
    if (!m_withLibSSH2)
    {
        sendCommand("stopServer");
    }
    else
    {
        //TODO
    }
}

const QString &QSSHCommand::password() const
{
    return m_password;
}

void QSSHCommand::setPassword(const QString &newPassword)
{
    if (m_password == newPassword)
        return;
    m_password = newPassword;
    emit passwordChanged();
}



const QString &QSSHCommand::keyFile() const
{
    return m_keyFile;
}

void QSSHCommand::setKeyFile(const QString &newKeyFile)
{
    if (m_keyFile == newKeyFile)
        return;
    m_keyFile = newKeyFile;
    emit keyFileChanged();
}

int QSSHCommand::portNum() const
{
    return m_portNum;
}

void QSSHCommand::setPortNum(int newPortNum)
{
    if (m_portNum == newPortNum)
        return;
    m_portNum = newPortNum;
    emit portNumChanged();
}

const QString &QSSHCommand::hostName() const
{
    return m_hostName;
}

void QSSHCommand::setHostName(const QString &newHostName)
{
    if (m_hostName == newHostName)
        return;
    m_hostName = newHostName;
    emit hostNameChanged();
}

const QString &QSSHCommand::sshClient() const
{
    return m_sshClient;
}

void QSSHCommand::setSshClient(const QString &newSshClient)
{
    if (m_sshClient == newSshClient)
        return;
    m_sshClient = newSshClient;
    emit sshClientChanged();
}


