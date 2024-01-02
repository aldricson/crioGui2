#include "QBetterCalendar.h"
#include <QTextCharFormat>


QBetterCalendar::QBetterCalendar(QWidget *parent) : QCalendarWidget(parent)
{
    QColor c = Qt::black;
    QBrush b = QBrush(c);
    QTextCharFormat format = QTextCharFormat();
    format.setBackground(b);
    this->setWeekdayTextFormat(Qt::Sunday   , format);
    this->setWeekdayTextFormat(Qt::Monday   , format);
    this->setWeekdayTextFormat(Qt::Tuesday  , format);
    this->setWeekdayTextFormat(Qt::Wednesday, format);
    this->setWeekdayTextFormat(Qt::Thursday , format);
    this->setWeekdayTextFormat(Qt::Friday   , format);
    this->setWeekdayTextFormat(Qt::Saturday , format);

    this->setVerticalHeaderFormat(NoVerticalHeader);
}

void QBetterCalendar::setBackupFiles(const QMap<QDate, QStringList> &files)
{
    backupFiles = &files;
}


void QBetterCalendar::paintCell(QPainter *painter, const QRect &rect, QDate date) const
{
    QCalendarWidget::paintCell(painter, rect, date);
    painter->save();

    // Check if there are backup files for this date
    bool hasBackup = backupFiles && backupFiles->contains(date);

    // Green rounded border for all cells
    hasBackup ? painter->setPen(QPen(Qt::darkRed, 2)) : painter->setPen(QPen(Qt::darkGreen, 1));
    painter->drawRoundedRect(rect.adjusted(1, 1, -1, -1), 5, 5);

    painter->restore();
}




