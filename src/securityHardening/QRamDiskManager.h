#ifndef QRAMDISKMANAGER_H
#define QRAMDISKMANAGER_H


#include <QProcess>
#include <QFileInfo>
#include <QTimer>




class QRamDiskManager : public QObject
{
    Q_OBJECT

public:
    explicit QRamDiskManager(QObject *parent = nullptr);
    ~QRamDiskManager();

    QString createRamDisk();
    bool deleteRamDisk(const QString &driveLetter);
    bool checkIfDriveLetterIsFree(const QString &driveLetter);
    QString findDriveLetterByLabel(const QString &diskLabel);
    QString getFirstFreeDriveLetter();
    void  writeImDiskInstallerScriptToFile();
    bool isImDiskInstalled();
    void installImDisk();
    bool checkForNoDrivesEntry();
    int calculateLetterCode(QString aLetter, bool &success);
private:
    QProcess *imDiskProcess;
    QString m_diskLabel = "DATADRILL";
    QString m_firstFreeDriveLetter = "";
    bool    checkConfigProcessTimeout();

private slots:

};

#endif // QRAMDISKMANAGER_H
