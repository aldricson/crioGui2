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
#include <QFile>
#include <QCryptographicHash>
#include <QMutex>


QMutex QSSHCommand::mutex;
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
 * - The lambda function captures 'this' pointer to access class members and Q_EMIT signals from within the lambda body.
 */
QSSHCommand::QSSHCommand(const QString &md5Hash,const QString &executionPath, QObject *parent) : QObject(parent), m_portNum(22)
{
    m_executionPath=executionPath;
    m_hashMd5 = md5Hash;
    connect(&m_process, &QProcess::finished, this, &QSSHCommand::processFinished);
    connect(&m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error) {
        if (error != QProcess::ProcessError::UnknownError)
        {
            Q_EMIT errorOccurredSignal(m_process.errorString(),lastCommand);
        }
    });
}

QString QSSHCommand::encryptData(const QString& dataStr, const QString& keyStr)
{
    QByteArray data = dataStr.toUtf8();
    QByteArray key = keyStr.toUtf8();
    QByteArray result;
    result.reserve(data.size());

    for (int i = 0; i < data.size(); ++i) {
        result.append(data[i] ^ key[i % key.size()]);
    }

    // Convertir le résultat en base64 pour un stockage / transmission sûrs
    return result.toBase64();
}

QString QSSHCommand::decryptData(const QString& encryptedDataStr, const QString& keyStr)
{
    QByteArray encryptedData = QByteArray::fromBase64(encryptedDataStr.toUtf8());
    QByteArray key = keyStr.toUtf8();
    QByteArray result;
    result.reserve(encryptedData.size());

    for (int i = 0; i < encryptedData.size(); ++i) {
        result.append(encryptedData[i] ^ key[i % key.size()]);
    }

    return QString::fromUtf8(result);
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


    QFile inputFile(m_executionPath+"ssh.bat");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QString fileContents = in.readAll(); // Read all contents into a string

        if (fileContents.startsWith("@echo off"))
        {
            QString encrypted = encryptData(fileContents,m_hashMd5);
            qInfo()<<"encrypted file:\n"<<encrypted;
            fileContents = encrypted;

            // Path to the output file in the application directory
            QString outputPath = m_executionPath + "ssh.bat";

            // Open the output file
            QFile outputFile(outputPath);
            if (outputFile.open(QIODevice::WriteOnly))
            {
                QTextStream out(&outputFile);
                out << fileContents; // Write the contents to the output file

            }
            outputFile.close();
        }
    }
    inputFile.close();


    // Check which command was executed last and Q_EMIT the corresponding signal.
    // Each if/else if block handles a different command.
    // Handling 'dir' command - used for listing directory contents.
    if (lastCommand == "dir")
    {
        Q_EMIT listFileDoneSignal(output, lastCommand);
    }
    // Handling 'getModuleList' command - used to retrieve a list of modules.
    else if (lastCommand == "getModuleList")
    {
        Q_EMIT moduleListRetrievedSignal(output, lastCommand);
    }
    // Handling 'downloadModule' command - used for downloading module definitions.
    else if (lastCommand == "downloadModule")
    {
        Q_EMIT moduleDownloadedSignal(output, lastCommand);
    }
    // Handling 'downloadModbusSetup' command - used for downloading Modbus setup.
    else if (lastCommand == "downloadModbusSetup")
    {
        Q_EMIT modbusSetupDownloadedSignal(output, lastCommand);
    }
    else if (lastCommand == "downloadModbusMapping")
    {
        Q_EMIT modbusMappingLoadedSignal(output, lastCommand);
    }
    else if (lastCommand == "uploadModbusMapping")
    {
       Q_EMIT modbusMappingUploadedSignal(output, lastCommand);
    }
    // Handling 'serverState' command - used to check the state of the server.
    else if (lastCommand == "serverState")
    {
        // Check if the server is running or not and Q_EMIT signal accordingly.
        if (output == "Program is not running in any screen session.\nProgram is not running.\n")
        {
            Q_EMIT serverStateSignal(false, lastCommand);
        }
        else if (output.contains("Access denied"))
        {
            Q_EMIT authenticationFailedSignal(lastCommand);
        }
        else
        {
            // Extract the screen session number if the server is running.
            int screenSession = extractNumberFromSessionString(output);
            Q_EMIT serverStartSuccesfullSignal(screenSession, lastCommand);
        }
    }
    // Handling 'startServer' command - used for starting the server.
    else if (lastCommand == "startServer")
    {
        Q_EMIT serverStartedSignal(lastCommand);
    }
    // Handling 'stopServer' command - used for stopping the server.
    else if (lastCommand == "stopServer")
    {
       Q_EMIT serverStopedSignal(lastCommand);
    }
    else if (lastCommand == "totalCPU")
    {
        Q_EMIT totalCPUSignal(output, lastCommand);
    }
    else if (lastCommand == "dataDrillCPU")
    {
        Q_EMIT dataDrillCpuSignal(output, lastCommand);
    }
    else if (lastCommand == "totalHDD")
    {
        Q_EMIT dataDrillTotalHDDSignal(output, lastCommand);
    }
    else if (lastCommand == "dataDrillHDD")
    {
        Q_EMIT dataDrillHDDSignal(output, lastCommand);
    }
    else if (lastCommand == "dataDrillTotalRamUsage")
    {
        Q_EMIT dataDrillTotalRamSignal(output, lastCommand);
    }
    else if (lastCommand == "dataDrillRamUsage")
    {
        Q_EMIT dataDrillRamUsageSignal(output, lastCommand);
    }
    else if (lastCommand == "dataDrillGlobalStats")
    {
        Q_EMIT globalStatSignal(output, lastCommand);
    }
    // Handling other commands - generic signal for command execution.
    else
    {
        Q_EMIT commandExecutedSignal(output, lastCommand);
    }
    // Reset lastCommand for the next execution to ensure clean state.
    lastCommand.clear();
}

void QSSHCommand::sendCommand(const QString &command, const QString &parameter)
{
    QMutexLocker locker(&mutex); // Lock the mutex for the scope of this function
    if (checkIntegrity())
    {
       lastCommand = command; // Store the command
       QString commandLine = constructSSHCommand(command, parameter);
       if (lastCommand=="downloadModbusMapping" || lastCommand == "uploadModbusMapping" )
       {
           qInfo()<<commandLine;
       }
       executeProcess(commandLine);
    }
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
    QString scriptPath = m_executionPath + "ssh.bat";
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
    Q_EMIT lastCommandChanged(lastCommand);
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
    Q_EMIT withLibSSH2Changed();
}

const QString &QSSHCommand::userName() const
{
    return m_userName;
}

void QSSHCommand::setUserName(const QString &newUserName)
{
    qInfo()<<"user name set:"<<newUserName;
    m_userName = newUserName;
    Q_EMIT userNameChanged();
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

void QSSHCommand::uploadMappingSetup(QString modbusMappingPath)
{
    if (!m_withLibSSH2)
    {
        QString params = modbusMappingPath+"mapping.csv"+" /home/dataDrill/mapping.csv";
        sendCommand("uploadModbusMapping", params);
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

void QSSHCommand::getTotalCPU()
{
    if (!m_withLibSSH2)
    {
        sendCommand("totalCPU");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getDataDrillCPU()
{
    if (!m_withLibSSH2)
    {
        sendCommand("dataDrillCPU");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getCrioHDDUsage()
{
    if (!m_withLibSSH2)
    {
        sendCommand("totalHDD");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getDataDrillHDDUsage()
{
    if (!m_withLibSSH2)
    {
        sendCommand("dataDrillHDD");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getCrioTotalRamUsage()
{
    if (!m_withLibSSH2)
    {
        sendCommand("dataDrillTotalRamUsage");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getDataDrillRamUsage()
{
    if (!m_withLibSSH2)
    {
        sendCommand("dataDrillRamUsage");
    }
    else
    {
        //TODO
    }
}

void QSSHCommand::getCrioGlobalStats()
{
    if (!m_withLibSSH2)
    {
        sendCommand("dataDrillGlobalStats");
    }
    else
    {
        //TODO
    }
}

bool QSSHCommand::checkIntegrity()
{
    QFile inputFile(m_executionPath+"ssh.bat");
    if (inputFile.open(QIODevice::ReadOnly))
    {
        QTextStream in(&inputFile);
        QString fileContents = in.readAll(); // Read all contents into a string

        if (!fileContents.startsWith("@echo off"))
        {
            QString decrypted = decryptData(fileContents,m_hashMd5);
            qInfo()<<"decrypted file:\n"<<decrypted;
            fileContents = decrypted;

            // Open the output file
            QFile outputFile2(m_executionPath+"ssh.bat");
            if (outputFile2.open(QIODevice::WriteOnly))
            {
                QTextStream out2(&outputFile2);
                out2 << fileContents; // Write the contents to the output file

            }
            outputFile2.close();
        }

        // Calculate MD5 hash
        QByteArray hash = QCryptographicHash::hash(fileContents.toUtf8(), QCryptographicHash::Md5);
        QString currentMd5 = QString::fromUtf8(hash.toHex());
        if (currentMd5!=m_hashMd5)
        {
            //somebody tries to hack the crio ssh.bat
            //Should we let it like this ? No... let's silently replace it

            // Path to the resource file in the resource system
             QString resourcePath = ":/scripts/ssh.bat"; // Update with actual path in your resource file

             // Open the resource file
             QFile inputFile2(resourcePath);
             if (inputFile2.open(QIODevice::ReadOnly))
             {
                 QTextStream in(&inputFile2);
                 QString fileContents2 = in.readAll(); // Read all contents into a string

                 // Path to the output file in the application directory
                 QString outputPath = m_executionPath + "ssh.bat";

                 // Open the output file
                 QFile outputFile2(outputPath);
                 if (outputFile2.open(QIODevice::WriteOnly))
                 {
                     QTextStream out2(&outputFile2);
                     out2 << fileContents2; // Write the contents to the output file

                 }
                 outputFile2.close();

             }
             inputFile2.close();
            return false;
        }
        else
        {
              inputFile.close();
              return true;
        }



    }
    else
    {
        inputFile.close();
        return false;
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
    Q_EMIT passwordChanged();
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
    Q_EMIT keyFileChanged();
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
    Q_EMIT portNumChanged();
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
    Q_EMIT hostNameChanged();
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
    Q_EMIT sshClientChanged();
}


