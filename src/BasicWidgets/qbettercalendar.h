#ifndef QBETTERCALENDAR_H
#define QBETTERCALENDAR_H
#include <QCalendarWidget>
#include <QPainter>

class QBetterCalendar : public QCalendarWidget
{
public:
    explicit QBetterCalendar(QWidget *parent = nullptr);
    void setBackupFiles(const QMap<QDate, QStringList> &files);
protected:
    void paintCell(QPainter *painter, const QRect &rect, QDate date) const override;

private:
    const QMap<QDate, QStringList> *backupFiles;

};

#endif // QBETTERCALENDAR_H
