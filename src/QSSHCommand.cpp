/**
 * @file QSSHCommand.cpp
 * @brief Implementation of the QSSHCommand class for executing SSH commands.
 *
 * @class QSSHCommand
 * @extends QObject
 *
 * The QSSHCommand class provides a way to execute SSH commands using QProcess.
 * It encapsulates the details of constructing and executing SSH commands and handles the output and errors.
 *
 * Key functionalities and methods include:
 * - Executing SSH commands using a batch script and handling their output and errors.
 * - Emitting signals based on the output of executed commands.
 * - Providing a flexible interface for setting SSH connection details.
 * - Handling various SSH-related tasks such as checking server state, downloading files, and managing server start/stop.
 *
 * @note This class abstracts the complexity of executing SSH commands and interpreting their output, making it easier to perform SSH operations from the main application.
 *
 * Detailed method documentation:
 * - processFinished(): Called when a command execution is completed; processes the output and emits appropriate signals.
 * - sendCommand(): Constructs and sends an SSH command for execution.
 * - constructSSHCommand(): Constructs the full command line string for executing an SSH command using a batch script.
 * - executeProcess(): Executes the given command line using QProcess.
 * - setHostName(), setUserName(), setPassword(), etc.: Setters for SSH connection details.
 * - getModulesDefinitions(), downloadModulesDefinitions(), startServer(), stopServer(), etc.: Methods to perform specific SSH tasks.
 *
 * The class also includes signals that are emitted when specific SSH tasks are completed, providing a way for other parts of the application to respond to SSH command results.
 */


#include "QSSHCommand.h"


/**
 * @brief Constructor for the QSSHCommand class.
 *
 * @param parent The parent QObject (default is nullptr).
 *
 * @details
 * - Initializes the QSSHCommand object with the default SSH port number (22).
 * - Connects the 'finished' signal of the QProcess (m_process) to the 'processFinished' method of this class.
 *   When the QProcess completes the execution of a command, 'processFinished' will be called.
 * - Connects the 'errorOccurred' signal of the QProcess to a lambda function.
 *   The lambda function is used to check if an error occurred during the execution of the process.
 *   If an error occurs (other than an unknown error), the 'errorOccurredSignal' is emitted with details of the error and the last executed command.
 * - The lambda function captures 'this' pointer to access class members and emit signals from within the lambda body.
 */
QSSHCommand::QSSHCommand(QObject *parent) : QObject(parent), m_portNum(22)
{
    connect(&m_process, &QProcess::finished, this, &QSSHCommand::processFinished);
    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (error != QProcess::ProcessError::UnknownError) {
            emit errorOccurredSignal(m_process.errorString(),lastCommand);
        }
    });
}


/**
 * @brief Called when the QProcess instance finishes executing a command.
 *
 * This method handles the output from the executed command and emits appropriate signals
 * based on the command that was executed.
 *
 * @param exitCode The exit code of the process.
 * @param exitStatus The exit status of the process.
 *
 * @details
 * - First attempts to read the standard output of the command.
 * - If there's no standard output, it reads the standard error instead.
 * - Uses the lastCommand variable to determine which command was executed last.
 * - Emits a specific signal corresponding to the executed command with the output.
 * - For the 'serverState' command, it checks if the server is running and emits the signal with the server state.
 * - Resets the lastCommand variable at the end to maintain a clean state for the next command execution.
 */
void QSSHCommand::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    // Read the standard output of the finished process.
    QString output = m_process.readAllStandardOutput();
    // If the standard output is empty, read the standard error instead.
    if (output.isEmpty()) {
        output = m_process.readAllStandardError();
    }
    // Check which command was executed last and emit the corresponding signal.
    // Each if/else if block handles a different command.
    // Handling 'dir' command - used for listing directory contents.
    if (lastCommand == "dir")
    {
        emit listFileDoneSignal(output, lastCommand);
    }
    // Handling 'getModuleList' command - used to retrieve a list of modules.
    else if (lastCommand == "getModuleList")
    {
        emit moduleListRetrievedSignal(output, lastCommand);
    }
    // Handling 'downloadModule' command - used for downloading module definitions.
    else if (lastCommand == "downloadModule")
    {
        emit moduleDownloadedSignal(output, lastCommand);
    }
    // Handling 'downloadModbusSetup' command - used for downloading Modbus setup.
    else if (lastCommand == "downloadModbusSetup")
    {
        emit modbusSetupDownloadedSignal(output, lastCommand);
    }
    else if (lastCommand == "downloadModbusMapping")
    {
        emit modbusMappingLoadedSignal(output, lastCommand);
    }
    // Handling 'serverState' command - used to check the state of the server.
    else if (lastCommand == "serverState")
    {
        qInfo() << "Server state received: " << output;
        // Check if the server is running or not and emit signal accordingly.
        if (output == "Program is not running in any screen session.\nProgram is not running.\n")
        {
            emit serverStateSignal(false, lastCommand);
        }
        else
        {
            // Extract the screen session number if the server is running.
            int screenSession = extractNumberFromSessionString(output);
            emit serverStartSuccesfullSignal(screenSession, lastCommand);
        }
    }
    // Handling 'startServer' command - used for starting the server.
    else if (lastCommand == "startServer")
    {
        emit serverStartedSignal(lastCommand);
    }
    // Handling 'stopServer' command - used for stopping the server.
    else if (lastCommand == "stopServer")
    {
       emit serverStopedSignal(lastCommand);
    }
    // Handling other commands - generic signal for command execution.
    else
    {
        emit commandExecutedSignal(output, lastCommand);
    }
    // Reset lastCommand for the next execution to ensure clean state.
    lastCommand.clear();
}

void QSSHCommand::sendCommand(const QString &command, const QString &parameter)
{
    lastCommand = command; // Store the command
    QString commandLine = constructSSHCommand(command, parameter);
    executeProcess(commandLine);
}


/**
 * @brief Constructs a command line string for executing an SSH command.
 *
 * @param command The SSH command to be executed.
 * @param parameter The additional parameter for the SSH command.
 * @return QString The constructed command line for execution.
 *
 * @details
 * - Constructs a command line string to be used with QProcess for executing SSH commands.
 * - The command line includes the path to a batch script (`ssh.bat`), host name, user name, password, port number, and the actual SSH command with its parameter.
 * - The batch script (`ssh.bat`) is assumed to be located in the application's directory.
 * - The command and parameter are concatenated to form the full SSH command.
 * - This method centralizes the construction of the SSH command line, ensuring consistency and ease of maintenance.
 */
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

const QString &QSSHCommand::getLastCommand() const
{
    return lastCommand;
}

void QSSHCommand::setLastCommand(const QString &newLastCommand)
{
    if (lastCommand == newLastCommand)
        return;
    lastCommand = newLastCommand;
    emit lastCommandChanged(lastCommand);
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

void QSSHCommand::downloadModbusSetup(QString iniModbusSetupPath)
{
    if (!m_withLibSSH2)
    {
        QString params = "/home/dataDrill/modBus.ini "+iniModbusSetupPath+"modBus.ini";
        sendCommand("downloadModbusSetup", params);
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::downloadMappingSetup(QString modbusMappingPath)
{
    if (!m_withLibSSH2)
    {
        QString params = "/home/dataDrill/mapping.csv "+modbusMappingPath+"mapping.csv";
        sendCommand("downloadModbusMapping", params);
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::isServerRunning()
{
    if (!m_withLibSSH2)
    {
        sendCommand("serverState","");
    }
    else
    {
        //TODO
    }


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


