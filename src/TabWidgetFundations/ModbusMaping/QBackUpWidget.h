#ifndef QBACKUPWIDGET_H
#define QBACKUPWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QDate>
#include <QStringList>
#include <QListWidgetItem>

class QBetterCalendar;
class QListWidget   ;
class QHeaderView    ;


class QBackUpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QBackUpWidget(QWidget *parent = nullptr);
    void analyzeFolder(const QString &folderPath);

    QMap<QDate, QStringList>& getBackupFiles();

signals:
    void fileSelected(const QString &filePath);

private slots:
    void onDateSelected(const QDate &date);
    void onListItemDoubleClicked(QListWidgetItem *anItem);

private:
    QBetterCalendar *calendarWidget;
    QListWidget *listWidget;
    QMap<QDate, QStringList> backupFiles;
    void loadStyleStringFromResource();
    void loadFilesForDate(const QDate &date);
};

#endif // QBACKUPWIDGET_H
