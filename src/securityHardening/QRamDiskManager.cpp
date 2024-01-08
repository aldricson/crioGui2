#include "QRamDiskManager.h"
#include <QStorageInfo>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include <QStandardPaths>
#include <windows.h>
#include <winuser.h>
#include <windef.h>
#include <QObject>


QRamDiskManager::QRamDiskManager(QObject *parent) : QObject(parent)
{
    imDiskProcess = new QProcess(this);
    if (!isImDiskInstalled())
    {
        //int response = QMessageBox::question(nullptr,
        //                                     "ImDisk is not installed",
        //                                     "ImDisk provides an extra layer of security.\nDo you wish to install it?",
        //                                     QMessageBox::Yes | QMessageBox::No);

        //if (response == QMessageBox::Yes)
        //{
            writeImDiskInstallerScriptToFile();
        //}
        //else
        //{
        //    // User chose not to install ImDisk
        //    // Handle accordingly
        //}
    }
    if (!checkForNoDrivesEntry())
    {
       // createNoDrivesEntry();
    }
    else
    {
        qInfo()<<"NoDrivesEntry already exists";
    }
}

QRamDiskManager::~QRamDiskManager()
{
    delete imDiskProcess;
}

void QRamDiskManager::writeImDiskInstallerScriptToFile()
{
    // Paths to the resource files
    QString resourceInstallPath = ":/ImDiskTk20231231/install.bat";
    QString resourceCabPath = ":/ImDiskTk20231231/files.cab";
    QString destPath = QCoreApplication::applicationDirPath() + "/Imdisk";

    // Create the directory if it doesn't exist
    QDir dir(destPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    // Function to copy a file from resources to the filesystem
    auto copyResourceToFile = [](const QString &sourcePath, const QString &destPath) {
        QFile inputFile(sourcePath);
        if (inputFile.open(QIODevice::ReadOnly)) {
            QFile outputFile(destPath);
            if (outputFile.open(QIODevice::WriteOnly)) {
                outputFile.write(inputFile.readAll());
                outputFile.close();
            }
            inputFile.close();
        }
    };

    // Copy install script and CAB file
    copyResourceToFile(resourceInstallPath, destPath + "/install.bat");
    copyResourceToFile(resourceCabPath, destPath + "/files.cab");

    // Install ImDisk
    installImDisk();
}





void QRamDiskManager::installImDisk()
{
    QString installScriptPath = QCoreApplication::applicationDirPath() + "/Imdisk/install.bat";

    QProcess installerProcess;
    // Include the /silent parameter in the QStringList
    installerProcess.start("cmd.exe", QStringList() << "/c" << installScriptPath );
    installerProcess.waitForFinished(); // Wait indefinitely until the process finishes

    // Optionally, check the exit status
    if (installerProcess.exitStatus() == QProcess::NormalExit && installerProcess.exitCode() == 0)
    {
        Sleep(1000);
        while (!checkConfigProcessTimeout())
        {
            Sleep(1000);
        }
    }
    else
    {
        // Installation failed
    }
}


bool QRamDiskManager::checkForNoDrivesEntry()
{
    HKEY hKey;
    DWORD dwType = REG_DWORD;
    DWORD dwData;
    DWORD dwDataSize = sizeof(dwData);

    // Open the registry key
    if (RegOpenKeyEx(HKEY_CURRENT_USER,
                     TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer"),
                     0,
                     KEY_READ,
                     &hKey) == ERROR_SUCCESS) {

        // Check for the existence of the NoDrives entry
        if (RegQueryValueEx(hKey,
                            TEXT("NoDrives"),
                            NULL,
                            &dwType,
                            reinterpret_cast<LPBYTE>(&dwData),
                            &dwDataSize) == ERROR_SUCCESS) {
            // Close the registry key
            RegCloseKey(hKey);

            return true; // The NoDrives entry exists
        }

        // Close the registry key
        RegCloseKey(hKey);
    }

    return false; // The NoDrives entry does not exist or failed to open the registry key

}




bool QRamDiskManager::checkConfigProcessTimeout()
{
    // Check if config.exe is running
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "tasklist");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();

    if (!output.contains("config.exe"))
    {
        //QMessageBox::information(nullptr, "Imdisk:", "Successfully installed", QMessageBox::Ok);
        return true;
    }
   return false;
}


int QRamDiskManager::calculateLetterCode(QString aLetter, bool &success)
{
    success = true;

    // Verify the string is not empty and take only the first character
    if (aLetter.isEmpty()) {
        success = false;
        return 0;
    }

    QChar firstChar = aLetter[0].toUpper();

    // Check for 'C' and return 0 with success as false
    if (firstChar == 'C')
    {
        success = false;
        return 0;
    }

    qInfo()<<firstChar<<" "<<firstChar.unicode()<<" "<<(firstChar.unicode() - 'A');
    // Calculate the letter code using powers of 2
    int code = 1 << (firstChar.unicode() - 'A');

    // Check if the letter is within A-Z range
    if (firstChar < 'A' || firstChar > 'Z') {
        success = false;
        return 0;
    }

    return code;
}



QString QRamDiskManager::findDriveLetterByLabel(const QString &diskLabel)
{
    wchar_t volumeName[MAX_PATH];
    DWORD volumeSerialNumber;
    DWORD maximumComponentLength;
    DWORD fileSystemFlags;
    wchar_t fileSystemNameBuffer[MAX_PATH];

    wchar_t drive[4] = L" :\\";
    for (wchar_t letter = L'A'; letter <= L'Z'; ++letter) {
        drive[0] = letter;
        if (GetVolumeInformationW(
                drive,
                volumeName, MAX_PATH,
                &volumeSerialNumber,
                &maximumComponentLength,
                &fileSystemFlags,
                fileSystemNameBuffer, MAX_PATH)) {

            QString currentLabel = QString::fromWCharArray(volumeName);
            qInfo()<<currentLabel;
            if (currentLabel == diskLabel)
            {
                return QString::fromWCharArray(drive);
            }
        }
    }

    return QString(); // Return empty string if not found
}

QString QRamDiskManager::createRamDisk()
{
    QString existingDisk=findDriveLetterByLabel(m_diskLabel);
    if (!existingDisk.isEmpty())
    {
        //ram disk already created return it's path
        qInfo()<<"ram disk already created with path: "<<existingDisk;
        return existingDisk;
    }

    m_firstFreeDriveLetter = getFirstFreeDriveLetter();
    if (m_firstFreeDriveLetter.isEmpty())
    {
        QMessageBox::information(nullptr, "No place for ram drive", "Switching to unsecure mode", QMessageBox::Ok);
        return QCoreApplication::applicationDirPath();
    }

    QString content;

    QFile file(":/scripts/ramDisk.bat");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qInfo()<<"generating command line";
        QTextStream stream(&file);
        content = stream.readAll();
        file.close();
        content.replace("%REPLACEME%",m_firstFreeDriveLetter);
        bool ok;
        int code = calculateLetterCode(m_firstFreeDriveLetter,ok);
        if (ok)
        {
            qInfo()<<"generating the hidding of ram disk";
            content.replace("%LETTERCODE%",QString::number(code));
        }
        else
        {
            qInfo()<<"generating the hidding of ram disk failed";
            content.replace("%LETTERCODE%",QString::number(0));
        }
    }

    // Save the modified content to a new file
    QString appDir = QCoreApplication::applicationDirPath();
    QString newFilePath = appDir + "/ramDisk.bat"; // Change the file name as needed

    QFile newFile(newFilePath);
    if (newFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&newFile);
        out << content;
        newFile.close();
        imDiskProcess->start("cmd", QStringList() << "/c" << "ramDisk.bat");
        bool success = imDiskProcess->waitForFinished();
        if (success)
        {
            SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 100, NULL);
            SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)"Environment", SMTO_ABORTIFHUNG, 100, NULL);
            return m_firstFreeDriveLetter;
        }
        else
        {
            QMessageBox::information(nullptr, "Failed to execute command", "Switching to unsecure mode", QMessageBox::Ok);
            return QCoreApplication::applicationDirPath();
        }
    }
    else
    {
        return QCoreApplication::applicationDirPath();
    }

}


bool QRamDiskManager::deleteRamDisk(const QString &driveLetter)
{
    // Command to remove the RAM disk
    QString command = QString("imdisk -D -m %1").arg(driveLetter);

    imDiskProcess->start("cmd", QStringList() << "/c" << command);
    return imDiskProcess->waitForFinished();
}

bool QRamDiskManager::checkIfDriveLetterIsFree(const QString &driveLetter)
{
    QFileInfo checkDrive(driveLetter + ":\\");
    return !checkDrive.exists();
}

QString QRamDiskManager::getFirstFreeDriveLetter()
{
    const QString possibleDriveLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    foreach (QChar letter, possibleDriveLetters) {
        QString drive = QString(letter) + ":\\";
        QFileInfo checkDrive(drive);
        if (!checkDrive.exists()) {
            return QString(letter) + ":";
        }
    }
    return QString(); // Return an empty string if no free drive letter is found
}

bool QRamDiskManager::isImDiskInstalled()
{
    QProcess imDiskProcess;
    imDiskProcess.start("imdisk", QStringList() << "--version");
    imDiskProcess.waitForFinished(); // Wait for the command to finish

    QString output = imDiskProcess.readAllStandardOutput();
    return output.contains("Control program for the ImDisk Virtual Disk Driver");
}
