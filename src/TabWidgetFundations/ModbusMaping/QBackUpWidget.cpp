#include "QBackUpWidget.h"
#include <QDir>
#include <QFileInfo>
#include <QCalendarWidget>
#include <QListWidget>
#include <QHeaderView>
#include "BasicWidgets/QBetterCalendar.h"

QBackUpWidget::QBackUpWidget(QWidget *parent)
    : QWidget(parent),
      calendarWidget(new QBetterCalendar(this)),
      listWidget(new QListWidget(this))
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(calendarWidget);
    layout->addWidget(listWidget);
    listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(calendarWidget, &QCalendarWidget::clicked, this, &QBackUpWidget::onDateSelected);
    connect(listWidget, &QListWidget::itemDoubleClicked, this, &QBackUpWidget::onListItemDoubleClicked);
    calendarWidget->setBackupFiles(this->getBackupFiles());
    loadStyleStringFromResource();

}

void QBackUpWidget::analyzeFolder(const QString &folderPath)
{
    QDir dir(folderPath);
    QStringList nameFilter("*.csv");
    QFileInfoList fileList = dir.entryInfoList(nameFilter, QDir::Files);

    for (const QFileInfo &fileInfo : fileList)
    {
        QString fileName = fileInfo.fileName();
        QDateTime fileDateTime = QDateTime::fromString(fileName.left(19), "dd_MM_yyyy_hh_mm_ss");

        if (fileDateTime.isValid())
        {
            backupFiles[fileDateTime.date()].append(fileInfo.absoluteFilePath());
        }
    }

  calendarWidget->update();
}

QMap<QDate, QStringList> &QBackUpWidget::getBackupFiles()
{
   return backupFiles;
}

void QBackUpWidget::onDateSelected(const QDate &date)
{
    loadFilesForDate(date);
}

void QBackUpWidget::onListItemDoubleClicked(QListWidgetItem *anItem)
{
    QString str = anItem->data(Qt::UserRole).toString();
    qInfo()<<str;

    //QTableWidgetItem *item = listWidget->item(anItem, 0);
    //if (item) {
    //    emit fileSelected(item->data(Qt::UserRole).toString());
    //}
}

void QBackUpWidget::loadFilesForDate(const QDate &date)
{
    listWidget->clear();

    if (backupFiles.contains(date))
    {
        const QStringList &files = backupFiles[date];

        for (int i = 0; i < files.size(); ++i)
        {
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(files[i]).fileName());
            item->setData(Qt::UserRole, files[i]);
            listWidget->addItem(item);
        }
    }
}

void QBackUpWidget::loadStyleStringFromResource()
{


   QFile file(":/styles/darkCalendar.scss");
   if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       return;
   }

   QTextStream in(&file);
   QString content = in.readAll();
   file.close();


   this->setStyleSheet(content);

}
